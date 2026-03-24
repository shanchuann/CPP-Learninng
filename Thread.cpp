#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <string>
#include <utility>
// Linux原生pthread锁相关头文件，仅Linux环境生效
#ifdef __linux__
#include <pthread.h>
#include <unistd.h>
#endif

/*******************************************************************************************************************
* C++11 多线程核心知识点总结
* C++11 首次引入标准多线程库，封装了跨平台的线程、锁、条件变量，替代了平台原生API（如Linux pthread）。
* 核心内容包括：std::thread 线程基础、各类互斥锁、RAII锁管理器、条件变量与线程同步、Linux原生兼容。
*
* 一、核心特性
* 1. std::thread：管理内核线程，禁止拷贝，仅支持移动；
* 2. 锁机制：提供多种互斥锁，满足不同场景需求；
* 3. RAII锁：自动加锁解锁，避免死锁，是锁的最佳使用方式；
* 4. 条件变量：配合 unique_lock，实现线程间的高效同步（如生产者消费者模型）；
* 5. 跨平台兼容：同时支持Windows/Linux/macOS，Linux下可与pthread原生API混合使用。
*
* 二、核心组件
* 1. std::thread：线程管理类，负责创建、回收、控制线程；
* 2. 互斥锁：
*    - std::mutex：基础互斥锁，同一时间仅一个线程可加锁；
*    - std::recursive_mutex：递归互斥锁，同一线程可重复加锁；
*    - std::timed_mutex：定时互斥锁，支持超时等待加锁；
*    - std::shared_mutex：读写锁，读多写少场景专用；
* 3. RAII锁：
*    - std::lock_guard：简单RAII锁，构造自动加锁，析构自动解锁；
*    - std::unique_lock：灵活RAII锁，支持延迟加锁、提前解锁、转移所有权；
* 4. std::condition_variable：条件变量，实现线程间的等待/唤醒机制。
*
* 三、核心接口
* 1. std::thread：
*    - 构造：支持普通函数、lambda、仿函数，参数默认按值传递，引用需用 std::ref/std::cref 包装；
*    - join()：等待线程结束，回收内核资源（必须调用，否则资源泄漏）；
*    - detach()：分离线程，内核资源由系统自动回收（分离后无法再控制，需保证线程安全）；
*    - get_id()：获取线程ID；
* 2. 互斥锁：
*    - lock()：加锁，阻塞直到获取锁；
*    - unlock()：解锁，必须与lock()成对出现；
*    - try_lock()：尝试加锁，成功返回true，失败返回false，不阻塞；
* 3. RAII锁：
*    - std::lock_guard<std::mutex> lock(mtx)：构造自动加锁；
*    - std::unique_lock<std::mutex> lock(mtx, std::defer_lock)：延迟加锁；
* 4. 条件变量：
*    - wait(lock)：释放锁并阻塞，直到被唤醒；
*    - wait(lock, pred)：释放锁并阻塞，直到pred为true（自动规避虚假唤醒）；
*    - notify_one()：唤醒一个等待的线程；
*    - notify_all()：唤醒所有等待的线程。
*******************************************************************************************************************/

// 模块1：线程基础操作
// 1.1 线程基础入口函数
void threadBaseFunc(int val) {
    std::cout << "【基础线程】线程ID：" << std::this_thread::get_id()
        << "，传入参数：" << val << std::endl;
}

// 1.2 线程参数传递-引用传递（必须用std::ref/std::cref包装）
void threadRefFunc(int& val) {
    val = 200;
    std::cout << "【引用传递】线程内修改参数完成" << std::endl;
}

// 1.3 线程参数传递-移动语义（避免大对象拷贝）
void threadMoveFunc(std::string str) {
    std::cout << "【移动语义】线程接收字符串长度：" << str.size() << std::endl;
}

// 线程基础演示函数
void threadBaseDemo() {
    std::cout << "\n 线程基础演示 " << std::endl;
    // 1. 基础线程创建与join回收
    std::thread t1(threadBaseFunc, 10);
    std::cout << "主线程获取t1线程ID：" << t1.get_id() << std::endl;
    t1.join(); // 必须调用，回收内核资源

    // 2. 引用传递（必须用std::ref包装，因为std::thread默认按值传参）
    int num = 100;
    std::thread t2(threadRefFunc, std::ref(num));
    t2.join();
    std::cout << "[引用传递]主线程参数最终值：" << num << std::endl;

    // 3. 移动语义传参（避免大对象拷贝，提高性能）
    std::string bigStr = "这是一个长字符串，用于测试移动语义";
    std::thread t3(threadMoveFunc, std::move(bigStr));
    t3.join();
    std::cout << "[移动语义]移动后原字符串长度：" << bigStr.size() << std::endl;

    // 4. 线程禁止拷贝，仅支持移动
    std::thread t4(threadBaseFunc, 20);
    std::thread t5 = std::move(t4); // 移动构造，t4变为空态
    t5.join();
    // t4.join(); // 错误：移动后的空态线程调用join会触发运行时错误
}

// 模块2：C++11各类锁机制
// 全局锁对象
std::mutex baseMtx;
std::recursive_mutex recMtx;
std::timed_mutex timedMtx;
std::shared_mutex rwMtx;
int sharedData = 0;

// 2.1 基础互斥锁std::mutex演示
void mutexBaseFunc(int id) {
    baseMtx.lock();
    std::cout << "[基础互斥锁]线程" << id << "进入临界区" << std::endl;
    sharedData++;
    std::cout << "[基础互斥锁]线程" << id << "修改共享数据为：" << sharedData << std::endl;
    baseMtx.unlock(); // 必须与lock成对出现，保证锁的对称性
}

// 2.2 递归锁std::recursive_mutex演示
void recursiveFunc(int depth) {
    if (depth <= 0) return;
    recMtx.lock(); // 同一线程可重复加锁
    std::cout << "[递归锁]递归深度：" << depth << "，加锁成功" << std::endl;
    recursiveFunc(depth - 1);
    recMtx.unlock(); // 加锁次数与解锁次数必须一致
}

// 2.3 定时锁std::timed_mutex演示
void timedLockFunc() {
    // 尝试加锁，最多等待1秒
    if (timedMtx.try_lock_for(std::chrono::seconds(1))) {
        std::cout << "[定时锁]获取锁成功" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        timedMtx.unlock();
    }
    else {
        std::cout << "[定时锁]等待超时，未获取到锁" << std::endl;
    }
}

// 2.4 读写锁std::shared_mutex读线程
void readFunc(int id) {
    rwMtx.lock_shared(); // 加共享读锁，多个线程可同时加锁
    std::cout << "[读写锁]读线程" << id << "读取数据：" << sharedData << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    rwMtx.unlock_shared();
}

// 2.5 读写锁std::shared_mutex写线程
void writeFunc() {
    rwMtx.lock(); // 加独占写锁，阻塞所有读写操作
    sharedData++;
    std::cout << "[读写锁]写线程修改数据为：" << sharedData << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    rwMtx.unlock();
}

// 锁机制演示函数
void lockDemo() {
    std::cout << "\n 锁机制演示 " << std::endl;
    sharedData = 0;

    // 1. 基础互斥锁演示
    std::thread m1(mutexBaseFunc, 1);
    std::thread m2(mutexBaseFunc, 2);
    m1.join();
    m2.join();

    // 2. 递归锁演示
    std::thread r1(recursiveFunc, 3);
    r1.join();

    // 3. 定时锁演示
    std::thread t1(timedLockFunc);
    std::thread t2(timedLockFunc);
    t1.join();
    t2.join();

    // 4. 读写锁演示
    sharedData = 0;
    std::thread w(writeFunc);
    std::thread r2(readFunc, 1);
    std::thread r3(readFunc, 2);
    w.join();
    r2.join();
    r3.join();
}

// 模块3：RAII锁管理器
// 3.1 lock_guard演示（简单临界区保护，推荐优先使用）
void lockGuardFunc(int id) {
    // 构造自动加锁，离开作用域析构自动解锁，保证锁的对称性，避免死锁
    std::lock_guard<std::mutex> lock(baseMtx);
    std::cout << "[lock_guard]线程" << id << "进入临界区" << std::endl;
    sharedData++;
    std::cout << "[lock_guard]线程" << id << "修改数据为：" << sharedData << std::endl;
    // 无需手动unlock，离开作用域自动解锁
}

// 3.2 unique_lock演示（灵活临界区保护，适合复杂场景）
void uniqueLockFunc(int id) {
    // 延迟加锁，构造时不加锁
    std::unique_lock<std::mutex> lock(baseMtx, std::defer_lock);
    std::cout << "[unique_lock]线程" << id << "准备加锁" << std::endl;
    lock.lock(); // 手动加锁
    sharedData++;
    std::cout << "[unique_lock]线程" << id << "修改数据为：" << sharedData << std::endl;
    lock.unlock(); // 手动提前解锁
    std::cout << "[unique_lock]线程" << id << "提前释放锁" << std::endl;
    // 析构时会自动检查锁状态，避免重复解锁
}

// RAII锁演示函数
void raiiLockDemo() {
    std::cout << "\n RAII锁演示 " << std::endl;
    sharedData = 0;

    // lock_guard演示
    std::thread lg1(lockGuardFunc, 1);
    std::thread lg2(lockGuardFunc, 2);
    lg1.join();
    lg2.join();

    // unique_lock演示
    std::thread ul1(uniqueLockFunc, 1);
    std::thread ul2(uniqueLockFunc, 2);
    ul1.join();
    ul2.join();
}

// 模块4：条件变量与生产者消费者模型
std::queue<int> msgQueue;
std::mutex condMtx;
std::condition_variable cond;
const int MAX_QUEUE_SIZE = 5;

// 生产者线程
void producer() {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(condMtx);
        // 队列满时等待，while循环规避虚假唤醒（操作系统层面的问题）
        while (msgQueue.size() >= MAX_QUEUE_SIZE) {
            std::cout << "[生产者]队列已满，等待..." << std::endl;
            cond.wait(lock);
        }
        msgQueue.push(i);
        std::cout << "[生产者]生产数据：" << i
            << "，队列大小：" << msgQueue.size() << std::endl;
        cond.notify_one(); // 唤醒一个消费者
    }
}

// 消费者线程
void consumer() {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(condMtx);
        // 队列空时等待，while循环规避虚假唤醒
        while (msgQueue.empty()) {
            std::cout << "[消费者]队列已空，等待..." << std::endl;
            cond.wait(lock);
        }
        int data = msgQueue.front();
        msgQueue.pop();
        std::cout << "[消费者]消费数据：" << data
            << "，队列大小：" << msgQueue.size() << std::endl;
        cond.notify_one(); // 唤醒一个生产者
    }
}

// 条件变量演示函数
void conditionVarDemo() {
    std::cout << "\n 条件变量演示 " << std::endl;
    std::thread pro(producer);
    std::thread con(consumer);
    pro.join();
    con.join();
}

// 模块5：Linux原生pthread锁演示（仅Linux生效）
#ifdef __linux__
pthread_mutex_t pthreadMtx;
int pthreadSharedData = 0;

// Linux原生线程入口函数
void* pthreadThreadFunc(void* arg) {
    pthread_mutex_lock(&pthreadMtx);
    pthreadSharedData++;
    printf("【Linux原生锁】线程%ld修改数据为：%d\n", pthread_self(), pthreadSharedData);
    pthread_mutex_unlock(&pthreadMtx);
    return NULL;
}

// Linux原生锁演示
void linuxPthreadDemo() {
    std::cout << "\n Linux原生锁演示 " << std::endl;
    pthread_mutex_init(&pthreadMtx, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, pthreadThreadFunc, NULL);
    pthread_create(&t2, NULL, pthreadThreadFunc, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&pthreadMtx);
}
#endif

#if 0
int main() {
    // 依次运行所有演示模块
    threadBaseDemo();    // 线程基础操作
    lockDemo();          // 各类锁机制
    raiiLockDemo();      // RAII锁管理器
    conditionVarDemo();  // 条件变量与生产者消费者模型

    // Linux环境下运行原生锁演示
#ifdef __linux__
    linuxPthreadDemo();
#endif

    return 0;
}
#endif