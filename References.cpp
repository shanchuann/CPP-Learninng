#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<assert.h>
using namespace std;
/*******************************************************************************************************************
* 引用（Reference）
* 引用是C++中一种特殊的变量类型，本质是已存在变量的别名，与被引用变量共享同一块内存空间。
*
* 1. 引用的定义
*    定义格式：类型& 引用变量名 = 被引用变量名;
*    特点：
*    - 引用必须在定义时初始化，不能先定义后赋值
*    - 引用一旦初始化，就永久绑定到被引用变量，不能再指向其他变量
*    - 没有空引用，必须引用一个已存在的有效变量
*    - 没有二级引用（不存在引用的引用）
*
* 2. 引用的基本使用
*    通过引用操作等同于操作被引用的变量，修改引用的值会直接修改原变量的值
*
* 3. 引用与函数
*    3.1 引用作为函数参数：可直接修改实参的值，避免值传递的拷贝开销，语法比指针更简洁
*    3.2 与值传递、指针传递的对比：
*        - 值传递：函数内修改的是形参副本，不影响实参
*        - 引用传递：函数内修改引用即修改实参，无需解引用操作
*        - 指针传递：需通过解引用修改实参，需检查指针是否为空
*
* 4. const引用
*    4.1 定义：const 类型& 引用变量名 = 被引用变量名;
*    4.2 特点：
*        - 不能通过const引用修改被引用变量的值
*        - 可以引用普通变量、常变量（const变量）和字面量
*        - 普通变量可被普通引用和const引用同时引用，但常变量只能被const引用引用
*
* 5. 数组的引用
*    定义格式：类型(&引用变量名)[数组大小] = 数组名;
*    注意：可以引用整个数组，但不能定义元素为引用的数组
*
* 6. 指针的引用
*    定义格式：类型*& 引用变量名 = 指针变量名;
*    用于对指针本身进行引用，可通过引用修改指针的指向
*
* 7. 右值引用（C++11特性）
*    定义格式：类型&& 引用变量名 = 右值表达式;
*    用于绑定右值（临时对象、字面量等），常用于实现移动语义和完美转发
* 
* 8. 指针与引用的区别
*    - 指针是存储地址的变量，引用是变量的别名
*    - 指针可空，引用不可空；指针可改指向，引用不可改绑定
*    - 指针需解引用访问值，引用可直接访问；指针占内存，引用不占额外内存
*    - 支持多级指针，不支持多级引用
*******************************************************************************************************************/

// 1. 引用的基本使用
void ref_basic() {
    cout << "----- 引用的基本使用 -----" << endl;
    int a = 10;												// a的值为10
    int& r = a; // r是a的引用（别名）							// r和a共享内存，值均为10
    cout << "a = " << a << ", r = " << r << endl;

    r = 20; // 修改r的值，实际修改a的值							// r和a的值均变为20
    cout << "a = " << a << ", r = " << r << endl;

    int b = 30;
    r = b; // 将b的值赋给r，实际是将b的值赋给a					// a和r的值变为30，b仍为30
    cout << "a = " << a << ", r = " << r << ", b = " << b << endl;

    // int& r2; // 错误：引用必须初始化

    int& r3 = a; // r3也是a的引用								// r3、r和a值均为30
    cout << "a = " << a << ", r3 = " << r3 << endl;
    cout << endl;
}

// 2. 引用作为函数参数（与值传递、指针传递对比）
// 值传递：无法修改实参
void ReferencevalSwap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

// 引用传递：直接修改实参，无需解引用
void ReferencerefSwap(int& a, int& b) { // 引用必须绑定有效变量，无需检查空值
    int temp = a;
    a = b;
    b = temp;
}

// 指针传递：需解引用，需检查指针有效性
void ReferenceptrSwap(int* a, int* b) {
    assert(a != nullptr && b != nullptr); // 检查指针非空
    int temp = *a;
    *a = *b;
    *b = temp;
}

void ref_in_function() {
    cout << "----- 引用在函数中的使用 -----" << endl;
    int a = 10, b = 20;
    cout << "Before swap: a = " << a << ", b = " << b << endl;

    ReferencevalSwap(a, b);
    cout << "After valSwap: a = " << a << ", b = " << b << endl; // 实参未改变

    ReferencerefSwap(a, b);
    cout << "After refSwap: a = " << a << ", b = " << b << endl; // 实参交换

    ReferenceptrSwap(&a, &b);
    cout << "After ptrSwap: a = " << a << ", b = " << b << endl; // 实参交换（交换回来）
    cout << endl;
}

// 3. const引用
void const_ref() {
    cout << "----- const引用 -----" << endl;
    int a = 10;
    int& ra = a; // 普通引用
    ra += 10; // 通过普通引用修改a的值
    cout << "a = " << a << ", ra = " << ra << endl; // a=20, ra=20

    const int& rca = a; // 常量引用
    // rca += 10; // 错误：不能通过const引用修改值
    a += 10; // 可直接修改原变量，const引用值也会变化
    cout << "a = " << a << ", rca = " << rca << endl; // a=30, rca=30

    const int c = 100; // 常变量
    // int& rc = c; // 错误：常变量不能被普通引用引用
    const int& rcc = c; // 正确：常变量必须被const引用引用
    cout << "c = " << c << ", rcc = " << rcc << endl;

    // const引用可以引用字面量（编译器会创建临时变量）
    const int& ref_literal = 1000;
    cout << "ref_literal = " << ref_literal << endl;
    cout << endl;
}

// 4. 数组的引用
void array_ref() {
    cout << "----- 数组的引用 -----" << endl;
    const int N = 5;
    int arr[N] = { 1, 2, 3, 4, 5 };

    // 引用数组元素
    int& ref_elem = arr[0]; // 引用数组首元素
    ref_elem += 10; // 修改首元素的值
    cout << "arr[0] = " << arr[0] << ", ref_elem = " << ref_elem << endl; // arr[0]=11

    // 引用整个数组
    int(&ref_arr)[N] = arr; // 定义数组的引用
    cout << "引用整个数组遍历：";
    for (int i = 0; i < N; i++) {
        cout << ref_arr[i] << " "; // 通过数组引用访问元素
    }
    cout << endl << endl;
}

// 5. 指针的引用
void ptr_ref() {
    cout << "----- 指针的引用 -----" << endl;
    int a = 20;
    int* p = &a; // 指针p指向a
    int*& ref_p = p; // ref_p是指针p的引用

    cout << "a = " << a << ", *p = " << *p << ", *ref_p = " << *ref_p << endl; // 均为20

    int b = 30;
    ref_p = &b; // 通过指针引用修改指针p的指向（现在p指向b）
    cout << "a = " << a << ", *p = " << *p << ", *ref_p = " << *ref_p << endl; // *p和*ref_p为30
    cout << endl;
}

// 6. 右值引用（简单示例，C++11特性）
void rvalue_ref() {
    cout << "----- 右值引用（C++11） -----" << endl;
    int a = 10;
    int& lref = a; // 左值引用（绑定左值）
    // int& lref2 = 10; // 错误：普通引用不能绑定右值

    int&& rref = 10; // 右值引用（绑定右值）
    cout << "rref = " << rref << endl;

    rref = 20; // 右值引用可修改绑定的临时值
    cout << "rref = " << rref << endl;

    int&& rref2 = move(a); // 通过move将左值转为右值
    cout << "rref2 = " << rref2 << ", a = " << a << endl;
    cout << endl;
}

#if 0
int main() {
    ref_basic();         // 引用的基本使用
    ref_in_function();   // 引用在函数中的使用
    const_ref();         // const引用
    array_ref();         // 数组的引用
    ptr_ref();           // 指针的引用
    rvalue_ref();        // 右值引用示例
    return 0;
}
#endif