#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
/*******************************************************************************************************************
* 函数重载（Function Overloading）
* 定义：在同一作用域内，允许存在多个同名函数，只要它们的**参数列表不同**（参数个数、参数类型、参数顺序不同）。
*      注意：返回值类型不同不能作为函数重载的依据（仅返回值不同的同名函数会被视为重复定义）。
*
* 核心规则：
* 1. 重载的判定仅依赖参数列表（函数签名），与返回值类型、参数名无关。
* 2. 参数列表不同的三种形式：
*    - 参数个数不同；
*    - 参数类型不同；
*    - 参数顺序不同（适用于不同类型的参数）。
* 3. const/volatile修饰符对重载的影响：
*    - 当参数为**引用或指针**时，const/volatile修饰会导致函数签名不同，可构成重载；
*    - 当参数为**按值传递**时，const/volatile修饰不会改变函数签名（因形参是实参的副本），不能构成重载。
*******************************************************************************************************************/

// 2.1 参数类型不同的重载
void Function_Overloadingfunc(int a) {
    std::cout << "func with int: " << a << std::endl;
}
void Function_Overloadingfunc(double a) {
    std::cout << "func with double: " << a << std::endl;
}

// 2.2 参数个数不同的重载
void Function_Overloadingfunc(int a, int b) {
    std::cout << "func with two int: " << a << ", " << b << std::endl;
}

// 2.3 参数顺序不同的重载（适用于不同类型参数）
void Function_Overloadingprint(int a, double b) {
    std::cout << "print(int, double): " << a << ", " << b << std::endl;
}
void Function_Overloadingprint(double a, int b) {
    std::cout << "print(double, int): " << a << ", " << b << std::endl;
}

// 3.1 const修饰引用参数：可构成重载（引用的const属性不同）
void Function_Overloadingprint(int& a) {
    std::cout << "print(int&): " << a << std::endl;
}
void Function_Overloadingprint(const int& a) {
    std::cout << "print(const int&): " << a << std::endl;
}

// 3.2 const修饰按值传递参数：不能构成重载（函数签名相同，编译报错）
// 错误示例：以下两个函数会被视为重复定义
// void show(int a) { cout << "show(int): " << a << endl; }
// void show(const int a) { cout << "show(const int): " << a << endl; }

#if 0
int main() {
    using namespace std;
    cout << "----- 参数类型不同的重载 -----" << endl;
    Function_Overloadingfunc(10);       // 匹配 func(int)
    Function_Overloadingfunc(10.5);     // 匹配 func(double)
    cout << endl;

    cout << "----- 参数个数不同的重载 -----" << endl;
    Function_Overloadingfunc(10, 20);   // 匹配 func(int, int)
    cout << endl;

    cout << "----- 参数顺序不同的重载 -----" << endl;
    Function_Overloadingprint(10, 20.5);  // 匹配 print(int, double)
    Function_Overloadingprint(20.5, 10);  // 匹配 print(double, int)
    cout << endl;

    cout << "----- const修饰引用参数的重载 -----" << endl;
    int x = 10;
    const int y = 20;
    Function_Overloadingprint(x);       // 匹配 print(int&)（x是可修改的左值）
    Function_Overloadingprint(y);       // 匹配 print(const int&)（y是const左值）
    Function_Overloadingprint(12);      // 匹配 print(const int&)（12是右值，只能绑定const引用）
    cout << endl;

    return 0;
}
#endif