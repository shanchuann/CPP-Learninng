#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<time.h>   // 用于time()函数，生成随机数种子
#include<stdlib.h> // 用于srand()和rand()函数，生成随机数
using namespace std;
/*******************************************************************************************************************
* 缺省参数（Default Parameters）
* 定义：缺省参数是指在函数声明或定义时，为函数的参数指定一个默认值。当函数调用时，若未显式传递该参数，则使用默认值；
*      若显式传递，则覆盖默认值。
*
* 1. 核心规则
*    1.1 缺省参数必须从右向左依次设置：函数参数列表中，若某个参数有默认值，则其右侧所有参数必须也有默认值（不能跳过左侧参数设右侧）。
*        - 正确示例：void fun(int a, int b=20, int c=30)（从右向左设置）
*        - 错误示例：void fun2(int a=10, int b, int c=30)（左侧a有默认值，右侧b无，违反规则）
*    1.2 缺省参数不能重复指定：同一参数的默认值只能在“函数声明”或“函数定义”中选择一处设置，不能两处同时指定。
*        - 常见场景：头文件（.h）中声明函数时指定默认值，源文件（.cpp）中定义函数时不重复指定。
*    1.3 默认值的有效性：默认值可以是常量、全局变量、或返回值为对应类型的函数（动态默认值），但不能是局部变量（生命周期不匹配）。
*
* 2. 常见使用场景
*    2.1 固定默认值：参数默认值为常量，适用于大多数调用场景下参数值固定的情况。
*    2.2 动态默认值：参数默认值由函数返回，适用于默认值需要动态生成的场景（如随机数）。
*    2.3 结合逗号表达式：利用逗号表达式的“取最后一个表达式值”特性，为参数传递复合逻辑的默认值。
*******************************************************************************************************************/

// 2.2 动态默认值：生成1~100的随机数（作为缺省参数的动态默认值）
int Random() {
    // 注意：若每次调用Random()都初始化种子，短时间内调用可能生成相同随机数（time()精度为秒）
    // 优化建议：可将srand()放在main函数开头，仅初始化一次
    srand((unsigned int)time(NULL));
    return rand() % 100 + 1;
}

// 2.2 动态默认值的缺省参数函数
void def_param_dynamic(int a = 10, int b = Random(), int c = 30) {
    cout << "----- 动态默认值的缺省参数 -----" << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl; // b的默认值由Random()动态生成
    cout << "c = " << c << endl;
    cout << endl;
}

// 1.1 基础缺省参数（从右向左依次设置）
void def_param_basic(int a, int b = 20, int c = 30) {
    cout << "----- 基础缺省参数（从右向左设置） -----" << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << endl;
}

// 错误示例：缺省参数未从右向左设置（编译报错）
// void def_param_error(int a = 10, int b, int c = 30) {
//     cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
// }

// 2.3 结合逗号表达式的缺省参数
void def_param_comma(int a = 0, int b = 0, int c = 0, int d = 0, int e = 0) {
    cout << "----- 结合逗号表达式的缺省参数 -----" << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl; // 逗号表达式(1,2,3)取最后一个值3
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;
    cout << "e = " << e << endl;
    cout << endl;
}

// 1.2 缺省参数的声明与定义（示例说明）
// 场景：头文件（Default_parameters_func.h）中的声明（指定默认值）
// void def_param_declare(int a, int b = 50); 
// 场景：源文件（当前文件）中的定义（不重复指定默认值）
#include"Default_parameters_func.h"
void def_param_declare(int a, int b) {
    cout << "----- 缺省参数的声明与定义（仅声明指定默认值） -----" << endl;
    cout << "a = " << a << ", b = " << b << endl;
    cout << endl;
}

#if 0
int main() {
    // 1. 基础缺省参数调用（从右向左省略参数）
    def_param_basic(10);          // 仅传a，b用20、c用30
    def_param_basic(10, 200);     // 传a和b，c用30
    def_param_basic(10, 200, 300);// 传全部参数，覆盖默认值

    // 2. 动态默认值的缺省参数调用
    def_param_dynamic(1, 2, 3);   // 传全部参数，覆盖默认值
    def_param_dynamic(1, 2);      // 传a和b，c用30
    def_param_dynamic(1);         // 传a，b用Random()、c用30
    def_param_dynamic();          // 全部用默认值（a=10，b=随机数，c=30）

    // 3. 结合逗号表达式的缺省参数调用
    def_param_comma(1, (1, 2, 3), 4); // b取逗号表达式结果3，d、e用0

    // 4. 声明与定义分离的缺省参数调用（假设头文件已声明b的默认值为50）
    def_param_declare(100);       // 仅传a，b用默认值0
    def_param_declare(100, 500);  // 传全部参数，覆盖默认值

    return 0;
}
#endif