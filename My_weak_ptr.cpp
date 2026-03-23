#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <iomanip>  // 用于 boolalpha

/*******************************************************************************************************************
* C++ std::weak_ptr 核心知识点总结
* weak_ptr 是为解决 shared_ptr 循环引用问题设计的**弱引用智能指针**
* 核心特性：
* 1. 不持有资源所有权，**不会增加强引用计数**，仅增加弱引用计数
* 2. 不影响资源的生命周期，资源由 shared_ptr 强引用决定
* 3. 安全访问：必须通过 lock() 转换为 shared_ptr 才能使用资源
* 4. 核心接口：
*    - use_count()：获取当前强引用计数
*    - expired()：判断资源是否已销毁（过期）
*    - lock()：返回 shared_ptr，资源有效则指向资源，无效则为空
*******************************************************************************************************************/

class Test {
public:
    Test() { std::cout << "Test对象创建" << std::endl; }
    ~Test() { std::cout << "Test对象销毁" << std::endl; }
    void show() { std::cout << "Test对象方法调用" << std::endl; }
};
#if 0
int main() {
    // 1. 创建shared_ptr（强引用，管理资源），初始化weak_ptr（弱引用，观测资源）
    std::shared_ptr<Test> sp = std::make_shared<Test>();
    std::weak_ptr<Test> wp(sp);

    // 2. 查看强引用计数：weak_ptr 不增加强引用计数，结果为 1
    std::cout << "当前强引用计数：" << wp.use_count() << std::endl;

    // 3. 安全访问资源：先判断是否过期，再通过 lock() 转为 shared_ptr
    if (!wp.expired()) {
        std::shared_ptr<Test> tmp = wp.lock();
        tmp->show();
    }

    // 4. 释放原shared_ptr，强引用计数归零，资源自动销毁
    sp.reset();
    std::cout << "是否已过期：" << std::boolalpha << wp.expired() << std::endl;

    // 5. 资源过期后，lock() 返回空指针，无法访问资源
    std::shared_ptr<Test> fail_sp = wp.lock();
    if (!fail_sp) {
        std::cout << "资源已销毁，无法访问" << std::endl;
    }

    return 0;
}
#endif