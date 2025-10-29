#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

/*******************************************************************************************************************
* 名字粉碎（Name Mangling）
* 定义：名字粉碎是C++编译器为支持函数重载而采用的一种技术。编译器会将函数名与参数类型、参数个数、调用约定等信息
*      组合成一个唯一的标识符（被称为“粉碎后的名字”），确保在链接阶段能区分不同的重载函数。
*
* 核心作用：
* 1. 解决函数重载的标识问题：C++允许同一作用域内存在同名但参数列表不同的函数，名字粉碎通过生成唯一标识符区分它们。
* 2. 支持类型安全的链接：确保链接器能准确匹配函数调用与函数定义（不同参数列表的函数对应不同的粉碎后名字）。
*
* 关键特性：
* 1. 粉碎规则依赖编译器：不同编译器（如MSVC、GCC、Clang）的名字粉碎规则不同，导致跨编译器的二进制兼容性较差。
* 2. 包含的关键信息：
*    - 函数名；
*    - 参数类型与个数；
*    - 调用约定（如__cdecl、__stdcall、__fastcall等）；
*    - 命名空间（若有）；
*    - 类名（若为成员函数）。
* 3. 与C语言的区别：C语言不支持函数重载，因此不进行名字粉碎（函数名保持原样），可通过extern "C"声明C风格函数。
*******************************************************************************************************************/

// 1. 函数重载与名字粉碎示例（不同参数列表导致不同的粉碎后名字）
// 声明4个重载函数（仅参数列表不同）
//bool Namefuna(int a, int b);       // 粉碎后名字示例：?Namefuna@@YA_NHH@Z（MSVC编译器）
//void Namefuna(int a);              // 粉碎后名字示例：?Namefuna@@YAXH@Z
//void Namefuna(double a, int b);    // 粉碎后名字示例：?Namefuna@@YAXNH@Z
//void Namefuna(int* a);             // 粉碎后名字示例：?Namefuna@@YAXPAH@Z

// 2. 调用约定对名字粉碎的影响
// 调用约定规定了函数调用时的栈管理、参数传递方式等，会被纳入名字粉碎规则
bool __stdcall funa(int a, int b) {  // __stdcall：Windows API常用约定，调用方不清理栈，函数自身清理
    return true;
}
void __cdecl funb(int a, int b) {    // __cdecl：C/C++默认约定，调用方清理栈，支持可变参数（如printf）
    // 实现
	std::cout << "funb called with a = " << a << ", b = " << b << std::endl;
}
void __fastcall func(int a, int b) { // __fastcall：快速调用，部分参数通过寄存器传递（而非栈），提高效率
    // 实现
    std::cout << "func called with a = " << a << ", b = " << b << std::endl;
}

#if 0
int main() {
    using namespace std;
    cout << "----- 名字粉碎与函数重载 -----" << endl;
    int a = 10;
    // 以下调用会触发编译器生成对应函数的粉碎后名字，用于链接阶段匹配
    // Namefuna(a);          // 匹配 void Namefuna(int)
    // Namefuna(a, 20);      // 匹配 bool Namefuna(int, int)
    // Namefuna(3.14, 30);   // 匹配 void Namefuna(double, int)
    // Namefuna(&a);         // 匹配 void Namefuna(int*)

    cout << "\n----- 调用约定对名字粉碎的影响 -----" << endl;
    funa(1, 2);   // 调用 __stdcall 约定的函数，粉碎后名字包含该约定信息
    funb(3, 4);   // 调用 __cdecl 约定的函数
    func(5, 6);   // 调用 __fastcall 约定的函数

    return 0;
}
#endif