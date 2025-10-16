#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<typeinfo> // 用于typeid()获取类型信息
using namespace std;
/*******************************************************************************************************************
* C++类型推导（Type Deduction）
* 类型推导是C++11及后续标准引入的核心特性，通过auto和decltype两种关键字实现，无需显式指定类型即可推导变量/表达式的类型，
* 简化代码编写，提高灵活性（尤其在模板和复杂类型场景）。
*
* 一、auto类型推导
* 定义：auto根据变量的**初始化值**自动推导类型，核心是“靠初始化值确定类型”，因此auto变量必须初始化。
*
* 核心规则：
* 1. 必须初始化：auto变量无初始化值时无法推导类型（编译报错）。
* 2. 单一类型推导：同一行用auto声明多个变量时，所有变量必须能推导为同一类型（否则编译报错）。
* 3. const属性处理：
*    - 忽略“顶层const”（变量本身的const）：const int ca=10; auto a=ca; → a推导为int。
*    - 保留“底层const”（指针/引用指向对象的const）：const int ca=10; auto b=&ca; → b推导为const int*。
* 4. 引用/指针推导：
*    - auto后加&：推导为引用类型（auto& c=ca; → c为const int&）。
*    - auto后加*：推导为指针类型（auto* p=&b; → p为int*，前提是初始化值为指针）。
* 5. 扩展场景：
*    - 函数参数（C++14起支持）：auto作为函数参数时，实际是模板参数（函数变为泛型函数）。
*    - new表达式：auto* p=new int(20); 或 auto s=new auto(30); → 均推导为int*。
*
* 二、decltype类型推导
* 定义：decltype根据**变量或表达式**的类型推导，核心是“直接获取表达式的类型”，无需初始化（但表达式需合法）。
*
* 核心规则：
* 1. 推导变量类型：decltype(变量名) → 完全保留变量的类型（包括const和引用）。
*    - int x=10; decltype(x) a; → a为int；int& rx=x; decltype(rx) b=x; → b为int&。
* 2. 推导表达式类型：
*    - 若表达式是“变量名加括号”（如decltype((x))）：推导为该变量的引用类型（int&）。
*    - 若表达式是普通运算（如x+y）：推导为运算结果的类型（int）。
* 3. 保留所有属性：decltype会保留表达式的const、volatile和引用属性，不做任何忽略。
*    - const int& rc=x; decltype(rc) rb=x; → rb为const int&。
* 4. 推导函数返回值：decltype(函数名(参数类型)) → 推导函数的返回值类型（无需调用函数）。
*    - int add(int a,int b); decltype(add(0,0)) z=0; → z为int。
*
* 三、auto与decltype的核心区别
* | 特性                | auto                          | decltype                      |
* |---------------------|-------------------------------|-------------------------------|
* | 推导依据            | 变量的初始化值                | 变量或表达式本身              |
* | 是否需要初始化      | 必须初始化（否则无法推导）    | 无需初始化（表达式合法即可）  |
* | const属性处理       | 忽略顶层const，保留底层const  | 完全保留所有const属性         |
* | 引用类型推导        | 需显式加&才推导为引用         | 自动根据表达式类型推导引用     |
* | 表达式括号影响      | 无影响（仅看初始化值类型）    | (变量名)会导致推导为引用       |
*******************************************************************************************************************/

// 1. auto类型推导示例
// 1.1 auto基础推导（普通类型、const、指针、引用）
void auto_basic() {
    cout << "----- auto基础推导 -----" << endl;
    const int ca = 10;    // 顶层const变量
    int x = 20;
    int& rx = x;          // 引用变量

    // 普通推导：忽略顶层const
    auto a = ca;          // a推导为int（忽略ca的顶层const）
    cout << "auto a = ca → type: " << typeid(a).name() << " (值: " << a << ")" << endl;

    // 指针推导：保留底层const
    auto b = &ca;         // b推导为const int*（&ca是const int*，底层const保留）
    cout << "auto b = &ca → type: " << typeid(b).name() << " (指向值: " << *b << ")" << endl;

    // 引用推导：显式加&，保留const
    auto& c = ca;         // c推导为const int&（绑定ca）
    cout << "auto& c = ca → type: " << typeid(c).name() << " (值: " << c << ")" << endl;

    // 指针符号*的推导
    auto* p = &x;         // p推导为int*（&x是int*，与auto*匹配）
    auto q = p;           // q推导为int*（auto自动识别指针类型）
    cout << "auto* p = &x → type: " << typeid(p).name() << " (指向值: " << *p << ")" << endl;
    cout << "auto q = p → type: " << typeid(q).name() << " (指向值: " << *q << ")" << endl;

    // 错误示例：auto不能推导多种类型（注释掉避免编译报错）
    // auto err = 5, err2 = 10.5; // 错误：auto只能推导一种类型（int和double冲突）
    cout << endl;
}

// 1.2 auto在new表达式和多变量声明中的使用
void auto_new_multi() {
    cout << "----- auto在new和多变量声明中 -----" << endl;
    // new表达式中的auto
    auto* p1 = new int(30);    // p1推导为int*
    auto p2 = new auto(40);    // p2推导为int*（new auto根据初始化值推导）
    cout << "auto* p1 = new int(30) → 指向值: " << *p1 << endl;
    cout << "auto p2 = new auto(40) → 指向值: " << *p2 << endl;

    // 多变量声明：必须推导为同一类型
    auto* p3 = &p1, a = p1;     // p3推导为int**（&p1是int**），a推导为int（auto*仅约束p3，a独立）
    cout << "auto* p3 = &p1, a = 5 → p3类型: " << typeid(p3).name() << ", a类型: " << typeid(a).name() << endl;

    // 释放内存
    delete p1;
    delete p2;
    p1 = nullptr;
    p2 = nullptr;
    cout << endl;
}

// 1.3 auto作为函数参数（C++14起支持，本质是泛型函数）
void auto_func_param(auto x) {
    cout << "auto函数参数接收值: " << x << " (类型: " << typeid(x).name() << ")" << endl;
}

void auto_function() {
    cout << "----- auto作为函数参数 -----" << endl;
    auto_func_param(10);       // x推导为int
    auto_func_param(10.5);     // x推导为double
    auto_func_param("hello");  // x推导为const char*
    cout << endl;
}

// 2. decltype类型推导示例
// 2.1 decltype推导变量类型（保留const和引用）
void decltype_var() {
    cout << "----- decltype推导变量类型 -----" << endl;
    int x = 10;
    const int& rx = x;    // const引用
    int& rrx = x;         // 普通引用

    // 推导普通变量：保留原始类型
    decltype(x) a;        // a推导为int（x是int）
    a = 20;
    cout << "decltype(x) a → type: " << typeid(a).name() << " (值: " << a << ")" << endl;

    // 推导引用变量：保留引用属性
    decltype(rx) b = x;   // b推导为const int&（rx是const int&）
    cout << "decltype(rx) b → type: " << typeid(b).name() << " (值: " << b << ")" << endl;

    // 推导引用的引用：折叠为普通引用（C++引用折叠规则）
    decltype(rrx) c = x;  // c推导为int&（rrx是int&）
    cout << "decltype(rrx) c → type: " << typeid(c).name() << " (值: " << c << ")" << endl;
    cout << endl;
}

// 2.2 decltype推导表达式类型（括号影响、运算结果）
void decltype_expr() {
    cout << "----- decltype推导表达式类型 -----" << endl;
    int x = 10, y = 20;

    // 推导普通运算表达式：结果为int
    decltype(x + y) z;    // x+y是int，z推导为int
    z = 30;
    cout << "decltype(x + y) z → type: " << typeid(z).name() << " (值: " << z << ")" << endl;

    // 推导“变量加括号”：结果为引用（(x)是左值表达式）
    decltype((x)) rx = x; // (x)是左值，rx推导为int&
    rx = 100;             // 修改rx即修改x
    cout << "decltype((x)) rx → type: " << typeid(rx).name() << " (x值变为: " << x << ")" << endl;

    // 推导指针表达式：结果为指针类型
    int* p = &x;
    decltype(&x) q = p;   // &x是int*，q推导为int*
    cout << "decltype(&x) q → type: " << typeid(q).name() << " (指向值: " << *q << ")" << endl;
    cout << endl;
}

// 2.3 decltype推导函数返回值类型
int type_add(int a, int b) { return a + b; } // 函数返回int

void decltype_func_ret() {
    cout << "----- decltype推导函数返回值 -----" << endl;
    // 推导函数返回值类型（无需调用函数，仅需参数类型合法）
    decltype(type_add(0, 0)) ret;  // type_add(0,0)返回int，ret推导为int
    ret = type_add(5, 3);
    cout << "decltype(type_add(0,0)) ret → type: " << typeid(ret).name() << " (type_add(5,3)= " << ret << ")" << endl;
    cout << endl;
}

// 3. auto与decltype的区别对比示例
void auto_vs_decltype() {
    cout << "----- auto与decltype区别对比 -----" << endl;
    const int ca = 10;

    // 1. const属性处理差异
    auto a = ca;          // auto忽略顶层const → int
    decltype(ca) b = 20;  // decltype保留const → const int
    cout << "auto a = ca → type: " << typeid(a).name() << " (无const)" << endl;
    cout << "decltype(ca) b → type: " << typeid(b).name() << " (保留const)" << endl;

    // 2. 引用推导差异
    auto& c = ca;         // auto需显式加& → const int&
    decltype((ca)) d = ca;// decltype自动推导引用 → const int&（(ca)是左值）
    cout << "auto& c = ca → type: " << typeid(c).name() << " (显式加&)" << endl;
    cout << "decltype((ca)) d → type: " << typeid(d).name() << " (自动推导引用)" << endl;
}

#if 0
int main() {
    // 调用auto相关示例
    auto_basic();         // auto基础推导
    auto_new_multi();     // auto在new和多变量中
    auto_function();      // auto作为函数参数

    // 调用decltype相关示例
    decltype_var();       // decltype推导变量
    decltype_expr();      // decltype推导表达式
    decltype_func_ret();  // decltype推导函数返回值

    // 对比auto与decltype
    auto_vs_decltype();

    return 0;
}
#endif