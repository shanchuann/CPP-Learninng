#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <mutex>
#include <atomic>

/*******************************************************************************************************************
* C++ 单例模式（Singleton）核心知识点总结
* 单例模式是常用的创建型设计模式，确保一个类在程序全局**仅有一个实例**，并提供统一的全局访问点。
* 核心内容包括：单例核心规则、饿汉式（立即初始化）、懒汉式（延迟初始化）、线程安全实现、双检锁优化方案。
*
* 一、单例模式核心规则
* 1. 构造函数私有化：禁止外部通过 new/直接调用构造创建对象；
* 2. 禁用拷贝与赋值：通过 = delete 显式删除拷贝构造、赋值运算符，杜绝生成新实例；
* 3. 静态访问接口：提供 public 静态成员函数作为唯一获取实例的入口；
* 4. 全局唯一性：整个程序生命周期内仅存在一个实例对象。
*
* 二、饿汉式单例（Eager Initialization）
* 1. 定义：程序启动/类加载时**立即创建实例**，无论是否使用；
* 2. 两种实现：全局静态成员变量、函数内局部静态变量（C++11）；
* 3. 优点：实现简单、天然线程安全、无锁开销；
* 4. 缺点：提前分配内存，不适合资源占用大的对象。
*
* 三、懒汉式单例（Lazy Initialization）
* 1. 定义：**首次调用时才创建实例**，延迟加载，节约内存；
* 2. 基础版本：无锁保护，多线程下会重复创建实例（线程不安全）；
* 3. 优化版本：双检锁（DCL）+ 原子变量 + 互斥锁，实现线程安全。
*
* 四、线程安全实现方案
* 1. 饿汉式（推荐）：C++11 后局部静态变量由编译器保证线程安全；
* 2. 双检锁懒汉式：兼顾延迟加载与线程安全，适合高性能场景；
* 3. 基础懒汉式：仅适用于单线程环境，禁止多线程使用。
*
* 五、关键语法特性
* 1. static 成员变量：类内声明，类外初始化，存储于全局数据区；
* 2. std::mutex：互斥锁保护临界区，防止多线程竞争创建实例；
* 3. std::atomic：原子操作保证指针读写原子性，优化双检锁效率；
* 4. = delete：显式禁用拷贝/赋值，强制保证单例唯一性。
*******************************************************************************************************************/

// ===================== 1. 懒汉式 - 双检锁 线程安全 =====================
class Singleton_DCL {
private:
    Singleton_DCL(int val = 0) : value(val) {
        std::cout << "Singleton_DCL 构造函数被调用" << std::endl;
    }
    Singleton_DCL(const Singleton_DCL&) = delete;
    Singleton_DCL& operator=(const Singleton_DCL&) = delete;
    ~Singleton_DCL() {
        std::cout << "Singleton_DCL 析构函数被调用" << std::endl;
    }

    static std::atomic<Singleton_DCL*> ps;
    static std::mutex mtx;
    int value;

public:
    static Singleton_DCL* getInstance() {
        Singleton_DCL* tmp = ps.load(std::memory_order_acquire);
        if (tmp == nullptr) {
            std::lock_guard<std::mutex> lk(mtx);
            tmp = ps.load(std::memory_order_relaxed);
            if (tmp == nullptr) {
                tmp = new Singleton_DCL(10);
                ps.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }

    void show() {
        std::cout << "Singleton_DCL: " << value << std::endl;
    }
};

std::atomic<Singleton_DCL*> Singleton_DCL::ps{ nullptr };
std::mutex Singleton_DCL::mtx;

// ===================== 2. 懒汉式 - 基础版本 线程不安全 =====================
class Singleton_Lazy_NoneSafe {
private:
    Singleton_Lazy_NoneSafe(int val = 0) : value(val) {
        std::cout << "Singleton_Lazy_NoneSafe 构造函数被调用" << std::endl;
    }
    Singleton_Lazy_NoneSafe(const Singleton_Lazy_NoneSafe&) = delete;
    Singleton_Lazy_NoneSafe& operator=(const Singleton_Lazy_NoneSafe&) = delete;
    ~Singleton_Lazy_NoneSafe() {
        std::cout << "Singleton_Lazy_NoneSafe 析构函数被调用" << std::endl;
    }

    static Singleton_Lazy_NoneSafe* ps;
    int value;

public:
    static Singleton_Lazy_NoneSafe* getInstance() {
        if (ps == nullptr) {
            ps = new Singleton_Lazy_NoneSafe(10);
        }
        return ps;
    }

    void show() {
        std::cout << "Singleton_Lazy_NoneSafe: " << value << std::endl;
    }
};

Singleton_Lazy_NoneSafe* Singleton_Lazy_NoneSafe::ps = nullptr;

// ===================== 3. 饿汉式 - 局部静态变量 C++11线程安全 =====================
class Singleton_Hungry_Local {
private:
    Singleton_Hungry_Local(int val = 0) : value(val) {
        std::cout << "Singleton_Hungry_Local 构造函数被调用" << std::endl;
    }
    Singleton_Hungry_Local(const Singleton_Hungry_Local&) = delete;
    Singleton_Hungry_Local& operator=(const Singleton_Hungry_Local&) = delete;
    ~Singleton_Hungry_Local() {
        std::cout << "Singleton_Hungry_Local 析构函数被调用" << std::endl;
    }

    int value;

public:
    static Singleton_Hungry_Local& getInstance() {
        static Singleton_Hungry_Local s(10);
        return s;
    }

    void show() {
        std::cout << "Singleton_Hungry_Local: " << value << std::endl;
    }
};

// ===================== 4. 饿汉式 - 全局静态变量 线程安全 =====================
class Singleton_Hungry_Global {
private:
    Singleton_Hungry_Global(int val = 0) : value(val) {
        std::cout << "Singleton_Hungry_Global 构造函数被调用" << std::endl;
    }
    Singleton_Hungry_Global(const Singleton_Hungry_Global&) = delete;
    Singleton_Hungry_Global& operator=(const Singleton_Hungry_Global&) = delete;
    ~Singleton_Hungry_Global() {
        std::cout << "Singleton_Hungry_Global 析构函数被调用" << std::endl;
    }

    static Singleton_Hungry_Global s;
    int value;

public:
    static Singleton_Hungry_Global& getInstance() {
        return s;
    }

    void show() {
        std::cout << "Singleton_Hungry_Global: " << value << std::endl;
    }
};

Singleton_Hungry_Global Singleton_Hungry_Global::s(10);

// ===================== 测试函数 =====================
void testDCL() {
    std::cout << "\n=== 测试 双检锁懒汉式（线程安全）===" << std::endl;
    Singleton_DCL* ins1 = Singleton_DCL::getInstance();
    Singleton_DCL* ins2 = Singleton_DCL::getInstance();
    ins1->show();
    ins2->show();
}

void testLazyNoneSafe() {
    std::cout << "\n=== 测试 基础懒汉式（线程不安全）===" << std::endl;
    Singleton_Lazy_NoneSafe* ins1 = Singleton_Lazy_NoneSafe::getInstance();
    Singleton_Lazy_NoneSafe* ins2 = Singleton_Lazy_NoneSafe::getInstance();
    ins1->show();
    ins2->show();
}

void testHungryLocal() {
    std::cout << "\n=== 测试 饿汉式（局部静态）===" << std::endl;
    Singleton_Hungry_Local& ins1 = Singleton_Hungry_Local::getInstance();
    Singleton_Hungry_Local& ins2 = Singleton_Hungry_Local::getInstance();
    ins1.show();
    ins2.show();
}

void testHungryGlobal() {
    std::cout << "\n=== 测试 饿汉式（全局静态）===" << std::endl;
    Singleton_Hungry_Global& ins1 = Singleton_Hungry_Global::getInstance();
    Singleton_Hungry_Global& ins2 = Singleton_Hungry_Global::getInstance();
    ins1.show();
    ins2.show();
}

#if 0
int main() {
    testDCL();
    testLazyNoneSafe();
    testHungryLocal();
    testHungryGlobal();

    return 0;
}
#endif