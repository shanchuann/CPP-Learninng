#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
/*******************************************************************************************************************
* nullptr（空指针常量）
* 定义：nullptr是C++11标准引入的**专门表示空指针的常量**，其类型为std::nullptr_t。
*      它的核心作用是解决C语言中NULL的歧义问题，明确区分“空指针”和“整数0”，尤其在函数重载场景中效果显著。
*
* 一、为什么需要nullptr？（NULL的缺陷）
* 在C和C++中，NULL的定义存在差异：
* - C语言：#define NULL ((void*)0) → 明确是void*类型的空指针；
* - C++语言：#define NULL 0 → 为了兼容非void*指针（C++不允许void*直接隐式转换为其他指针类型），将NULL定义为整数0。
*
* 这种差异导致C++中使用NULL时的歧义问题：
* 当函数存在“int参数”和“指针参数”的重载时，传递NULL会优先匹配int参数（因NULL本质是0），而非预期的指针参数，违背空指针的使用意图。
*
* 二、nullptr的核心特性
* 1. 类型专属：nullptr的类型是std::nullptr_t，而非整数或任意指针类型，但可隐式转换为**任意指针类型**（如int*、char*、自定义类指针等）。
* 2. 无歧义匹配：在函数重载中，nullptr会优先匹配指针类型的参数，而非整数类型参数，彻底解决NULL的歧义问题。
* 3. 不可赋值给非指针：nullptr不能赋值给int、double等非指针变量（编译报错），确保仅用于指针场景，类型安全性更高。
* 4. 与NULL的兼容性： nullptr可直接用于所有原本使用NULL的场景（如判断指针是否为空），且功能更精准。
*
* 三、常见使用场景
* 1. 函数重载：明确传递空指针，匹配指针类型的重载函数；
* 2. 指针初始化：初始化指针变量为“空”状态（替代NULL）；
* 3. 指针判空：判断指针是否指向有效地址（与==、!=运算符兼容）；
* 4. 模板或泛型：在泛型代码中传递空指针，避免类型歧义。
*******************************************************************************************************************/

// 示例：函数重载场景（对比NULL与nullptr的差异）
// 1. 重载1：接收int类型参数
void Nullptrfunc(int a) {
    cout << "调用int参数版本：int a = " << a << endl;
}

// 2. 重载2：接收char*指针类型参数
void Nullptrfunc(char* p) {
    if (p == nullptr) { // 使用nullptr判断指针是否为空
        cout << "调用char*参数版本：char* p = nullptr" << endl;
    }
    else {
        cout << "调用char*参数版本：char* p = " << p << endl;
    }
}

// 场景1：nullptr在函数重载中的无歧义匹配
void nullptr_overload_demo() {
    cout << "----- nullptr与NULL在函数重载中的对比 -----" << endl;
    // 1. 传递整数0：匹配int参数版本（符合预期）
    Nullptrfunc(0);
    // 2. 传递NULL：因C++中NULL是0，匹配int参数版本（歧义问题：本意是传递空指针，却匹配整数）
    Nullptrfunc(NULL);
    // 3. 传递nullptr：匹配char*参数版本（无歧义，符合空指针意图）
    Nullptrfunc(nullptr);
    cout << endl;
}

// 场景2：nullptr用于指针初始化与判空
void nullptr_init_check() {
    cout << "----- nullptr用于指针初始化与判空 -----" << endl;
    // 1. 用nullptr初始化指针（替代NULL）
    char* p1 = nullptr;
    int* p2 = nullptr;
    // int num = nullptr; // 错误：nullptr不能赋值给非指针变量

    // 2. 用nullptr判断指针是否为空
    if (p1 == nullptr) {
        cout << "p1（char*）是空指针" << endl;
    }
    if (p2 != nullptr) {
        cout << "p2（int*）是有效指针" << endl;
    }
    else {
        cout << "p2（int*）是空指针" << endl;
    }
}

#if 0
int main() {
    nullptr_overload_demo(); // 演示函数重载中的歧义解决
    nullptr_init_check();    // 演示指针初始化与判空
    return 0;
}
#endif