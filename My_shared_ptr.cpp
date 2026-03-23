#define _CRT_SECURE_NO_WARNINGS
#include <memory>
#include <atomic>
#include <iostream>
#include <thread>
#include <utility>
using namespace std;

/*******************************************************************************************************************
* C++ 手写共享智能指针 My_Shared_Ptr 核心知识点总结
* 对应 std::shared_ptr 实现原理：**共享式智能指针**，多个指针可共享同一资源，通过**原子引用计数**管理生命周期。
* 核心特性：引用计数、共享所有权、自动释放资源、多线程安全、支持自定义删除器、支持动态数组。
*
* 一、核心设计原则
* 1. 共享所有权：多个 My_Shared_Ptr 管理同一个资源，通过引用计数记录持有者数量；
* 2. 原子计数：引用计数使用 std::atomic 保证多线程安全，无数据竞争；
* 3. 自动释放：当计数减为 0 时，自动调用删除器释放资源+销毁控制块；
* 4. 拷贝/移动：支持拷贝（计数+1）、移动（转移所有权，计数不变）；
* 5. 自定义删除器：适配内存、文件、句柄等任意资源，支持数组特化 delete[]。
*
* 二、核心组件
* 1. My_RefCount：引用计数控制块，存储原子强/弱引用计数、资源指针；
* 2. My_Shared_Deleter：通用删除器+数组特化删除器，负责资源释放；
* 3. My_Shared_Ptr：主体类，封装指针、控制块、删除器，提供标准接口。
*
* 三、核心接口
* 1. 构造/析构/拷贝/移动：管理资源所有权；
* 2. use_count()：获取当前引用计数；
* 3. reset()：释放当前资源，重新管理新资源；
* 4. swap()：交换两个智能指针的资源；
* 5. 运算符重载：*、->、[]（数组）、bool 类型转换。
*******************************************************************************************************************/

// 1. 通用删除器+数组特化删除器
template <typename _Ty>
struct My_Shared_Deleter {
    // 默认删除器：负责销毁由 new 分配的单个对象
    // 说明：该删除器无状态，调用时应该与资源的分配方式匹配（new -> delete）
    // 如果用户需要管理其它资源（如 FILE*、socket），可以传入自定义 Deleter 替代该实现
    void operator()(_Ty* ptr) const { delete ptr; }
};

// 数组特化，调用delete[]释放动态数组
template <typename _Ty>
struct My_Shared_Deleter<_Ty[]> {
    // 数组特化删除器：针对 new T[] 分配的数组必须使用 delete[] 释放
    // 注意：不要把 delete 与 new[]/new 混用，否则会导致未定义行为
    void operator()(_Ty* ptr) const { delete[] ptr; }
};

// 2. 原子引用计数控制块（多线程安全）
template <typename _Ty>
class My_RefCount {
private:
    _Ty* _Ptr;
    atomic<int> _Uses;   // 强引用计数，原子操作
    atomic<int> _Weaks;  // 弱引用计数，适配weak_ptr
public:
    // 控制块构造：保存资源指针并初始化引用计数
    // 语义：如果 ptr 非空，则表示至少有一个强引用（由创建 shared_ptr 的构造方负责）
    My_RefCount(_Ty* ptr = nullptr) : _Ptr(ptr), _Uses(0), _Weaks(0) {
        if (_Ptr) _Uses = 1;
    }
    // 计数递增
    // 增加强/弱引用计数；使用原子自增保证多线程下的正确性
    void Incref() { ++_Uses; }
    void Incwref() { ++_Weaks; }
    // 计数递减，返回当前值
    // 减少计数并返回递减后的值
    // 注意：这里使用前缀 -- 操作，返回的是递减之后的新值
    int Decref() { return --_Uses; }
    int Decwref() { return --_Weaks; }
    // 获取计数与对象指针
    // load() 读取当前计数，默认为 memory_order_seq_cst 的严格内存序
    int use_count() const { return _Uses.load(); }
    _Ty* get() const { return _Ptr; }
};

// 3. shared_ptr主体类实现
template <class _Ty, class Deleter = My_Shared_Deleter<_Ty>>
class My_Shared_Ptr {
public:
    using pointer = _Ty*;
    using element_type = _Ty;
    using deleter_type = Deleter;
private:
    pointer _Ptr;
    My_RefCount<_Ty>* _RefBlock;
    deleter_type _Deleter;
public:
    // 默认空构造
    My_Shared_Ptr() : _Ptr(nullptr), _RefBlock(nullptr) {}
    // 裸指针构造
    explicit My_Shared_Ptr(pointer p) : _Ptr(p), _RefBlock(nullptr) {
        // 如果传入非空裸指针，则为其创建一个新的控制块并设置初始引用计数
        if (_Ptr) _RefBlock = new My_RefCount<_Ty>(_Ptr);
    }
    // 拷贝构造：共享资源，计数+1
    My_Shared_Ptr(const My_Shared_Ptr& other) {
        // 复制构造：共享同一控制块，原子增加强引用计数
        _Ptr = other._Ptr;
        _RefBlock = other._RefBlock;
        if (_RefBlock) _RefBlock->Incref();
    }
    // 移动构造：转移所有权，不改动计数
    My_Shared_Ptr(My_Shared_Ptr&& other) noexcept {
        // 移动构造：直接窃取资源指针和控制块指针，避免修改计数
        _Ptr = other._Ptr;
        _RefBlock = other._RefBlock;
        // 将来源对象置空，避免在其析构时释放资源
        other._Ptr = nullptr;
        other._RefBlock = nullptr;
    }
    // 析构：计数-1，归零则释放资源
    ~My_Shared_Ptr() {
        // 析构时减少强引用计数；若下降到 0，该控制块负责释放实际资源并删除控制块
        if (_RefBlock && _RefBlock->Decref() == 0) {
            _Deleter(_Ptr);
            delete _RefBlock;
        }
    }
    // 拷贝赋值
    My_Shared_Ptr& operator=(const My_Shared_Ptr& other) {
        if (this != &other) {
            // 先释放当前资源
            if (_RefBlock && _RefBlock->Decref() == 0) {
                // 当前对象是最后一个持有者，释放资源并删除控制块
                _Deleter(_Ptr);
                delete _RefBlock;
            }
            // 切换到 other 管理的资源并增加计数
            _Ptr = other._Ptr;
            _RefBlock = other._RefBlock;
            if (_RefBlock) _RefBlock->Incref();
        }
        return *this;
    }
    // 移动赋值
    My_Shared_Ptr& operator=(My_Shared_Ptr&& other) noexcept {
        if (this != &other) {
            // 释放当前持有资源（如果是最后一个持有者则删除之）
            if (_RefBlock && _RefBlock->Decref() == 0) {
                _Deleter(_Ptr);
                delete _RefBlock;
            }
            // 窃取 other 的资源与控制块，并将 other 置空
            _Ptr = other._Ptr;
            _RefBlock = other._RefBlock;
            other._Ptr = nullptr;
            other._RefBlock = nullptr;
        }
        return *this;
    }
    // 辅助接口
    // 返回当前强引用计数，若没有控制块则为 0
    long use_count() const { return _RefBlock ? _RefBlock->use_count() : 0; }
    // 获取被管理的裸指针
    pointer get() const { return _Ptr; }
    void reset(pointer p = nullptr) {
        // 释放当前持有的资源（如果需要）并为新的裸指针创建控制块
        if (_RefBlock && _RefBlock->Decref() == 0) {
            _Deleter(_Ptr);
            delete _RefBlock;
        }
        _Ptr = p;
        _RefBlock = p ? new My_RefCount<_Ty>(p) : nullptr;
    }
    void swap(My_Shared_Ptr& other) {
        std::swap(_Ptr, other._Ptr);
        std::swap(_RefBlock, other._RefBlock);
    }
    explicit operator bool() const { return _Ptr != nullptr; }
    // 指针运算符重载
    // 解引用与箭头操作符，提供与裸指针类似的访问方式
    _Ty& operator*() const { return *_Ptr; }
    pointer operator->() const { return _Ptr; }
};

// 测试函数：验证 My_Shared_Ptr 的基本语义（拷贝/移动/重置/线程安全）
void test_my_shared_ptr() {
    cout << "--- test_my_shared_ptr start ---\n";

    // 构造并检查 use_count
    My_Shared_Ptr<int> p1(new int(42));
    cout << "p1 use_count=" << p1.use_count() << ", *p1=" << *p1 << "\n";

    // 拷贝构造：共享所有权，计数增加
    My_Shared_Ptr<int> p2 = p1;
    cout << "after copy: p1 use_count=" << p1.use_count() << ", p2 use_count=" << p2.use_count() << "\n";

    // 通过副本修改值，主副本可见
    *p2 = 100;
    cout << "after modify: *p1=" << *p1 << ", *p2=" << *p2 << "\n";

    // 移动构造：p3 接管 p2，p2 置空
    My_Shared_Ptr<int> p3 = std::move(p2);
    cout << "after move: p2 use_count=" << p2.use_count() << ", p3 use_count=" << p3.use_count() << "\n";

    // reset 将释放原有资源并建立新资源
    p3.reset();
    cout << "after reset p3 use_count=" << p3.use_count() << ", p3 bool=" << (bool)p3 << "\n";

    p3.reset(new int(7));
    cout << "p3 assigned new: p3 use_count=" << p3.use_count() << ", *p3=" << *p3 << "\n";

    // swap 交换两个 shared_ptr 管理的资源
    p1.swap(p3);
    cout << "after swap: p1 use_count=" << p1.use_count() << ", *p1=" << *p1
        << "; p3 use_count=" << p3.use_count() << ", *p3=" << *p3 << "\n";

    // 多线程场景：多个线程拷贝并使用 shared_ptr，验证引用计数的原子性
    My_Shared_Ptr<int> pmulti(new int(55));
    auto worker = [pmulti]() mutable {
        for (int i = 0; i < 1000; ++i) {
            My_Shared_Ptr<int> local = pmulti; // 拷贝会增加计数
        }
        cout << "worker done local use_count=" << pmulti.use_count() << "\n";
        };
    thread t1(worker);
    thread t2(worker);
    t1.join();
    t2.join();
    cout << "after threads pmulti use_count=" << pmulti.use_count() << "\n";

    cout << "--- test_my_shared_ptr end ---\n";
}
#if 0
int main() {
    test_my_shared_ptr();
	return 0;
}
#endif