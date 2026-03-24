#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

/*******************************************************************************************************************
* C++ Lambda 表达式 核心知识点总结
* Lambda 是C++11引入的**匿名函数**，用于快速定义临时函数对象，简化代码、适配STL算法、实现闭包。
* 核心特性：匿名性、闭包（捕获外部变量）、语法简洁、可与STL算法无缝结合，C++14/17持续增强。
*
* 一、基础语法（C++11）
* [捕获列表] (参数列表) mutable -> 返回值类型 { 函数体 }
* 1. 捕获列表：必选，定义如何捕获外部变量（值/引用/this）；
* 2. 参数列表：可选，同普通函数参数；
* 3. mutable：可选，解除值捕获变量的只读属性；
* 4. 返回值类型：可选，自动推导；
* 5. 函数体：必选，执行逻辑。
*
* 二、捕获规则（核心）
* 1. []：不捕获任何变量；
* 2. [var]：值捕获var，只读（mutable可修改副本）；
* 3. [&var]：引用捕获var，可直接修改原变量；
* 4. [=]：值捕获所有外部变量；
* 5. [&]：引用捕获所有外部变量；
* 6. [this]：捕获当前对象指针，访问成员变量/函数；
* 7. [*this]：C++17，拷贝捕获当前对象（避免悬空）。
*
* 三、版本特性
* 1. C++11：基础Lambda、值/引用/this捕获、闭包；
* 2. C++14：泛型Lambda(auto参数)、表达式捕获(移动捕获)；
* 3. C++17：*this拷贝捕获、constexpr Lambda。
*
* 四、核心用途
* 1. STL算法（for_each/count_if/sort）；
* 2. 临时回调函数、线程入口；
* 3. 替代简单仿函数，简化代码。
*******************************************************************************************************************/

// 共用自定义Int类（多个示例依赖）
class Int {
private:
    int value;
public:
    Int(int x = 0) : value(x) {}
    Int operator+(const Int& it) const { return Int(this->value + it.value); }
    int Getvalue() const { return value; }
};

// 重载输出运算符
std::ostream& operator<<(std::ostream& out, const Int& it) {
    return out << it.Getvalue();
}

// 1. 基础语法与返回值
void demo_basic_syntax() {
    std::cout << "===== 基础语法示例 =====" << std::endl;
    // 完整语法：捕获列表+参数+返回值+函数体
    auto f = [](int a) -> int { return a + 1; };
    std::cout << "f(1) = " << f(1) << std::endl;

    // 自动推导返回值（省略->int）
    auto f1 = [](int a) { return a + 1; };
    // 省略空参数列表
    auto f2 = [] { return 1; };
    std::cout << "f2() = " << f2() << std::endl;

    // 合法推导：普通返回值
    auto x1 = [](int i) { return i; };
    // 非法：初始化列表无法自动推导返回值（注释避免编译报错）
    // auto x2 = [] { return {1,2}; };
}

// 2. 值捕获示例
void demo_value_capture() {
    std::cout << "\n 值捕获示例 " << std::endl;
    Int c = 6872;
    Int d = 5032;
    // 值捕获：拷贝外部变量，Lambda内部是副本，外部修改不影响内部
    auto Add = [c, d](Int a, Int b) -> Int {
        std::cout << "c=" << c << " d=" << d << std::endl;
        return a + b;
        };
    d = 20; // 外部修改，Lambda内的d副本不变
    std::cout << "1+2=" << Add(1, 2) << std::endl;
}

// 3. 捕获变量常性规则
void demo_const_property() {
    std::cout << "\n 捕获变量常性示例 " << std::endl;
    int num = 10;

    // 默认值捕获：变量只读，不可修改
    auto err_func = [num] {
        // num += 5; 编译报错：值捕获变量默认const
        std::cout << "值捕获只读num：" << num << std::endl;
        };
    err_func();

    // mutable关键字：解除值捕获的只读属性（修改的是副本）
    // 注意：使用 mutable 时如果没有参数列表也应显式写出空参数列表 ()
    auto mutable_func = [num]() mutable {
        num += 5;
        std::cout << "mutable修改后副本num：" << num << std::endl;
        };
    mutable_func();
    std::cout << "外部原变量num：" << num << std::endl;

    // 引用捕获：直接修改原变量，无const限制
    auto ref_func = [&num] {
        num += 5;
        std::cout << "引用捕获修改后num：" << num << std::endl;
        };
    ref_func();
}

// 4. 引用捕获示例
void demo_ref_capture() {
    std::cout << "\n 引用捕获示例 " << std::endl;
    Int c = 6872;
    Int d = 5032;
    // 引用捕获：直接操作原变量，内部修改会同步到外部
    auto Add = [&c, &d](Int a, Int b) -> Int {
        c = 68725032;
        d = c;
        return a + b;
        };
    std::cout << "1+2=" << Add(1, 2) << std::endl;
    std::cout << "c=" << c << " d=" << d << std::endl;
}

// 5. this指针捕获示例
class TestClass {
private:
    int m_val;
public:
    TestClass(int val) : m_val(val) {}
    void testThisCapture() {
        std::cout << "\n this指针捕获示例 " << std::endl;
        int local_param = 20;

        // 显式this捕获：访问类成员变量/函数
        auto func1 = [this]() {
            std::cout << "显式this捕获，成员变量：" << m_val << std::endl;
            };
        func1();

        // 隐式[=]捕获：自动包含this，可访问成员+局部变量
        auto func2 = [=]() {
            std::cout << "隐式[=]捕获，成员变量：" << m_val << std::endl;
            std::cout << "隐式[=]捕获，局部变量：" << local_param << std::endl;
            };
        func2();

        // 修改成员变量（this捕获无const限制）
        auto func3 = [this]() mutable {
            m_val += 10;
            std::cout << "mutable修改后成员变量：" << m_val << std::endl;
            };
        func3();

        // C++17 [*this] 拷贝捕获：拷贝对象副本，不影响原对象
        auto func4 = [*this]() {
            Int temp(m_val);
            std::cout << "拷贝捕获修改副本：" << m_val + 50 << std::endl;
            };
        func4();
        std::cout << "最终原实例成员变量：" << m_val << std::endl;
    }
};

// 6. C++14 表达式捕获（移动捕获）
void demo_expr_capture() {
    std::cout << "\n C++14 表达式捕获示例 " << std::endl;
    auto important = std::make_unique<Int>(1);
    // 表达式捕获：支持初始化、移动语义（解决unique_ptr无法值捕获的问题）
    auto add = [v1 = 1, v2 = std::move(important)](Int x, Int y) -> Int {
        return x + y + v1 + (*v2);
        };
    std::cout << "表达式捕获计算结果：" << add(3, 4) << std::endl;
}

// 7. C++14 泛型Lambda
void demo_generic_lambda() {
    std::cout << "\n 泛型Lambda示例 " << std::endl;
    // auto参数：自动推导类型，支持任意类型参数
    auto add = [](auto x, auto y) {
        return x + y;
        };
    std::cout << "int 1+2 = " << add(1, 2) << std::endl;
    std::cout << "double 1.1+1.2 = " << add(1.1, 1.2) << std::endl;
}

// 8. 可变Lambda（mutable）
void demo_mutable_lambda() {
    std::cout << "\n 可变Lambda示例 " << std::endl;
    int v = 5;
    // mutable：允许修改值捕获的副本
    auto ff = [v]()mutable { return ++v; };
    v = 0;
    std::cout << "值捕获mutable修改：" << ff() << std::endl;

    int v2 = 5;
    // 引用捕获：直接修改原变量，无需mutable
    auto ff2 = [&v2] { return ++v2; };
    v2 = 0;
    std::cout << "引用捕获修改：" << ff2() << std::endl;
}

// 9. 类成员函数中的Lambda
int g_max = 10;
class Object {
    int value;
public:
    Object(int x = 0) : value(x) {}
    void func(int a, int b) {
        // 全局变量：无需捕获，直接使用
        auto x0 = []() -> int { return g_max; };
        // 值捕获：捕获this、a、b
        auto x2 = [=]() -> int {
            int x = value;
            return x + a + g_max;
            };
        // 引用捕获：直接修改全局变量+成员变量
        auto x3 = [&]() -> int {
            g_max = 100;
            value += 10;
            return g_max + value;
            };
    }
};

// 10. Lambda类型与存储
void demo_type_storage() {
    std::cout << "\n Lambda类型与存储示例 " << std::endl;
    int x = 0;
    // std::function：存储任意可调用对象（推荐）
    std::function<int(int)> f1 = [](int a) { return a; };
    x = f1(10);
    std::cout << "function调用结果：" << x << std::endl;

    // 无捕获Lambda：可隐式转换为普通函数指针
    using func_t = int(*)(int);
    func_t f = [](int a) -> int { return a; };
    std::cout << "函数指针调用结果：" << f(20) << std::endl;
}

// 11. 延迟调用问题（值捕获陷阱）
void demo_delay_call() {
    std::cout << "\n 延迟调用示例 " << std::endl;
    int a = 0;
    // 值捕获：定义时拷贝变量，延迟调用不会获取最新值
    auto funa = [=] { return a; };
    a += 100; // 外部修改，Lambda内副本不变
    std::cout << "延迟调用结果：" << funa() << std::endl;
}

// 12. 配合STL算法（最常用场景）
void demo_stl_algorithm() {
    std::cout << "\n STL算法配合示例 " << std::endl;
    std::vector<int> v = { 1,2,3,4,5,6 };
    int even_count = 0;
    // for_each + Lambda 遍历
    std::for_each(v.begin(), v.end(), [&even_count](int val) {
        if (!(val & 1)) ++even_count;
        });
    std::cout << "偶数个数：" << even_count << std::endl;

    // count_if + Lambda 条件统计
    int count = std::count_if(v.begin(), v.end(), [](int x) {
        return x > 5 && x < 10;
        });
    std::cout << "5-10之间元素个数：" << count << std::endl;
}

// 13. Lambda与仿函数对比
// 传统仿函数
class CountEven {
    int& count_;
public:
    CountEven(int& count) : count_(count) {}
    void operator()(int val) const {
        if (val % 2 == 0) ++count_;
    }
};
void demo_functor_compare() {
    std::cout << "\n===== 仿函数与Lambda对比示例 =====" << std::endl;
    std::vector<int> ar = { 1,2,3,4,5,6,7,8 };
    int even_count = 0;

    // 传统仿函数写法
    std::for_each(ar.begin(), ar.end(), CountEven(even_count));
    std::cout << "仿函数统计偶数：" << even_count << std::endl;

    // Lambda写法：更简洁、代码内联
    even_count = 0;
    std::for_each(ar.begin(), ar.end(), [&even_count](int val) {
        if (val % 2 == 0) ++even_count;
        });
    std::cout << "Lambda统计偶数：" << even_count << std::endl;
}

#if 0
int main() {
    // 运行所有Lambda核心示例
    demo_basic_syntax();
    demo_value_capture();
    demo_const_property();
    demo_ref_capture();

    TestClass obj(100);
    obj.testThisCapture();

    demo_expr_capture();
    demo_generic_lambda();
    demo_mutable_lambda();
    demo_type_storage();
    demo_delay_call();
    demo_stl_algorithm();
    demo_functor_compare();

    return 0;
}
#endif