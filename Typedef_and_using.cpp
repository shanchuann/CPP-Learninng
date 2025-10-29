#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>

/*******************************************************************************************************************
* C++11 using别名（Using Alias）
* 定义：using别名是C++11引入的类型/模板别名机制，通过`using 别名 = 目标类型/模板`语法，为已有类型或模板定义简洁别名，
*      功能上兼容并扩展了传统的typedef，尤其在模板别名场景中解决了typedef的局限性。
*
* 核心分类与规则：
* 一、普通类型别名（替代typedef）
* 1. 语法：using 别名 = 已有类型;
*    - 与typedef功能完全等效，但语法更直观（“赋值式”写法，符合变量定义习惯）。
* 2. 支持的目标类型：
*    - 基础类型（如unsigned int、char*）；
*    - 数组类型（如int[10]、char[20]）；
*    - 指针/引用类型（如char*、const int&）；
*    - 自定义类型（如类、结构体）。
* 3. 与typedef的对比：
*    - typedef：typedef 已有类型 别名; （“后缀式”写法，复杂类型易读性差）；
*    - using：using 别名 = 已有类型; （“前缀式”写法，逻辑更清晰，尤其嵌套类型）。

* 二、模板别名（C++11新增，typedef无法实现）
* 1. 语法：template<模板参数列表> using 别名 = 目标模板类型;
*    - 核心作用：为模板定义“带参数的别名”，实现模板类型的局部固化或简化，解决typedef无法模板化的问题。
* 2. 支持的目标模板类型：
*    - 模板化数组类型（如T[N]，T为类型参数，N为非类型参数）；
*    - STL容器模板（如vector<T>、map<K,V>）；
*    - 自定义模板类型。
* 3. 关键特性：
*    - 模板参数列表需包含目标模板所需的参数（类型参数用class/typename，非类型参数用具体类型如int）；
*    - 使用别名时需显式传递模板参数，生成具体类型（如Array<int,10> 等价于 int[10]）。

* 三、using别名的核心优势
* 1. 语法直观：“别名=目标类型”的写法符合日常认知，比typedef的“目标类型 别名”更易读，尤其复杂类型（如函数指针、多维数组）；
* 2. 支持模板别名：这是typedef的核心短板——typedef无法为模板定义别名，而using可轻松实现模板化的类型别名；
* 3. 兼容性强：完全兼容typedef的所有场景，可无缝替换typedef，且在代码重构时更易维护；
* 4. 可读性高：嵌套类型或模板类型的别名定义更清晰，例如`using StrVec = vector<string>`比`typedef vector<string> StrVec`更直观。
*******************************************************************************************************************/

// 1. 场景1：普通类型别名（替代typedef，基础类型、指针、数组）
void using_basic_type_alias() {
    using namespace std;
    cout << "----- 普通类型别名（替代typedef） -----" << endl;

    // 1.1 基础类型别名：unsigned int → UINT
    using UINT = unsigned int;
    // typedef unsigned int UINT; // 与上面using语句完全等效
    UINT num = 100;
    cout << "UINT（unsigned int）类型变量：" << num << endl;

    // 1.2 指针类型别名：char* → PCHAR
    using PCHAR = char*;
    // typedef char* PCHAR; // 与上面using语句完全等效
    PCHAR str_ptr = new char[20];
    strcpy(str_ptr, "Hello Using Alias");
    cout << "PCHAR（char*）类型指针：" << str_ptr << endl;

    // 1.3 数组类型别名：int[10] → IntArray10
    using IntArray10 = int[10];
    // typedef int IntArray10[10]; // 与上面using语句完全等效
    IntArray10 arr = { 1,2,3,4,5,6,7,8,9,10 };
    cout << "IntArray10（int[10]）类型数组：";
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << "\n" << endl;

    // 释放指针内存
    delete[] str_ptr;
    str_ptr = nullptr;
}

// 2. 场景2：模板别名（C++11新增，typedef无法实现）
// 2.1 定义模板别名：为“T类型、N大小的数组”定义模板别名Array
template<class T, int N> // T：类型模板参数；N：非类型模板参数（需为常量表达式）
using Array = T[N];       // 模板别名：Array<T,N> 等价于 T[N]

// 2.2 使用模板别名生成具体类型
void using_template_alias() {
    using namespace std;
    cout << "----- 模板别名（C++11新增） -----" << endl;

    // 2.2.1 生成“int类型、10大小”的数组类型：Array<int,10> → int[10]
    Array<int, 10> int_arr = { 0,1,2,3,4,5,6,7,8,9 };
    cout << "Array<int,10>（int[10]）数组：";
    for (int i = 0; i < 10; i++) {
        cout << int_arr[i] << " ";
    }
    cout << endl;

    // 2.2.2 生成“char类型、20大小”的数组类型：Array<char,20> → char[20]
    Array<char, 20> char_arr;
    strcpy(char_arr, "Hello, World!"); // 字符串长度13，小于20，安全
    cout << "Array<char,20>（char[20]）字符串：" << char_arr << endl;

    // 2.2.3 模板别名的灵活性：快速切换类型/大小
    Array<double, 5> double_arr = { 1.1, 2.2, 3.3, 4.4, 5.5 };
    cout << "Array<double,5>（double[5]）数组：";
    for (int i = 0; i < 5; i++) {
        cout << double_arr[i] << " ";
    }
    cout << "\n" << endl;
}

// 3. 场景3：using与typedef的可读性对比（复杂类型示例）
void using_vs_typedef_readability() {
    using namespace std;
    cout << "----- using与typedef可读性对比（复杂类型） -----" << endl;

    // 示例：为“返回值int、参数为(int, char*)的函数指针”定义别名
    // typedef写法（后缀式，需从右向左解析，易混淆）
    typedef int (*FuncPtr_Typedef)(int, char*);
    // using写法（赋值式，直接对应“别名=目标类型”，更直观）
    using FuncPtr_Using = int (*)(int, char*);

    // 验证：两种别名完全等效，可互换使用
    auto test_func = [](int a, char* b) -> int {
        return a + strlen(b);
        };
    FuncPtr_Typedef fp1 = test_func;
    FuncPtr_Using fp2 = test_func;

    char test_str[] = "test";
    cout << "FuncPtr_Typedef调用结果：" << fp1(5, test_str) << endl; // 5+4=9
    cout << "FuncPtr_Using调用结果：" << fp2(5, test_str) << endl;   // 结果一致
}

#if 0
int main() {
    using_basic_type_alias();         // 普通类型别名演示（替代typedef）
    using_template_alias();           // 模板别名演示（C++11新增）
    using_vs_typedef_readability();   // using与typedef可读性对比
    return 0;
}
#endif