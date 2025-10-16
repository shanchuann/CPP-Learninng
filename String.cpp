#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring> // 用于C风格字符串操作（如验证c_str()结果）
using namespace std;
/*******************************************************************************************************************
* C++ std::string 核心操作与类型转换
* std::string是C++标准库提供的字符串类，封装了字符串的存储、修改、查找等功能，解决了C风格字符串（char*）易溢出、操作繁琐的问题。
* 核心内容包括：C风格字符串与std::string的双向转换、std::string常用成员函数、字符串运算符重载。
*
* 一、C风格字符串与std::string的双向转换
* 1. C风格字符串 → std::string（隐式转换，简单直接）
*    - 语法：std::string 变量名 = C风格字符串（const char* 类型）;
*    - 原理：std::string的构造函数支持接收const char*参数，自动拷贝字符到string内部缓冲区，无需手动管理内存。
*
* 2. std::string → C风格字符串（显式调用成员函数c_str()）
*    - 语法：const char* 指针名 = string对象.c_str();
*    - 关键注意事项：
*      - c_str()返回的是指向string内部缓冲区的const char*指针，**不可修改**（修改会导致未定义行为）；
*      - 指针的有效性依赖于原string对象：当string对象被修改（如拼接、清空）或销毁时，指针立即失效；
*      - 若需修改转换后的C风格字符串，需手动拷贝c_str()结果到新的char数组（如用strcpy()）。
*
* 二、std::string常用成员函数
* 1. 查找与替换
*    - find(const string& sub)：查找子串sub在当前string中**首次出现的起始位置**，返回size_t类型（未找到返回string::npos）；
*    - replace(size_t pos, size_t len, const string& rep)：从pos位置开始，替换len个字符为字符串rep。
*
* 2. 拼接与扩展
*    - append(const string& str)：在当前string末尾追加字符串str；
*    - 简化写法：operator+=（直接用+=追加字符串，语法更简洁，功能与append()等效）。
*
* 3. 清空与判空
*    - clear()：清空string中的所有字符，使其变为空字符串（长度为0）；
*    - empty()：判断string是否为空（返回bool，空则true，非空则false），常与clear()搭配使用。
*
* 三、std::string运算符重载（简化操作）
* 1. operator+：拼接两个string对象或string与C风格字符串，返回新的string对象（原对象不修改）；
* 2. operator+=：在当前string末尾追加字符串（修改原对象，效率比operator+高，无需创建临时对象）；
* 3. 其他常用运算符：==（判断字符串相等）、!=（判断不等）、<（字典序比较）等，支持直观的字符串比较。
*******************************************************************************************************************/

// 1. 场景1：C风格字符串与std::string的双向转换
void str_conversion() {
    cout << "----- C风格字符串与std::string双向转换 -----" << endl;

    // 1.1 C风格字符串 → std::string（隐式转换）
    const char* c_str = "Hello C"; // C风格字符串（const char*）
    string s = c_str;              // 直接赋值，自动转换为std::string
    cout << "C风格字符串转std::string：" << s << endl;

    // 1.2 std::string → C风格字符串（调用c_str()）
    const char* new_c_str = s.c_str(); // 获取string内部的C风格字符串指针
    cout << "std::string转C风格字符串：" << new_c_str << endl;

    // 注意：c_str()返回的指针不可修改（const修饰），且依赖原string对象
    // char* modify_ptr = s.c_str(); // 错误：不能将const char*赋值给char*
    // 若需修改，需手动拷贝到char数组
    char modifiable_arr[20];
    strcpy(modifiable_arr, s.c_str()); // 拷贝c_str()结果到可修改数组
    modifiable_arr[5] = '!';          // 修改数组（合法）
    cout << "手动拷贝后可修改的C风格字符串：" << modifiable_arr << "\n" << endl;
}

// 2. 场景2：std::string的查找与替换
void str_find_replace() {
    cout << "----- std::string查找与替换 -----" << endl;

    string s = "I love C++, C++ is easy!";
    cout << "原始字符串：" << s << endl;

    // 2.1 查找子串"C++"的首次位置（find()返回size_t类型）
    size_t pos = s.find("C++"); // 查找成功返回起始索引，失败返回string::npos
    if (pos != string::npos) {  // 判断是否找到子串
        cout << "子串\"C++\"首次出现位置：" << pos << endl;

        // 2.2 替换子串：从pos位置开始，替换3个字符（"C++"长度为3）为"C plus plus"
        s.replace(pos, 3, "C plus plus");
        cout << "替换后字符串：" << s << "\n" << endl;
    }
    else {
        cout << "未找到子串\"C++\"" << "\n" << endl;
    }
}

// 3. 场景3：std::string的拼接、清空与判空
void str_append_clear() {
    cout << "----- std::string拼接、清空与判空 -----" << endl;

    // 3.1 拼接字符串：append()与operator+=
    string s = "Hello";
    s.append(" World"); // 用append()追加字符串
    cout << "append(\" World\")后：" << s << endl;

    s += "!"; // 用operator+=追加（简化写法，与append()等效）
    cout << "operator+=（\"!\"）后：" << s << endl;

    // 3.2 清空与判空
    s.clear(); // 清空字符串，变为空串
    if (s.empty()) { // 判断是否为空
        cout << "clear()后字符串：\"" << s << "\"（空字符串）" << "\n" << endl;
    }
}

// 4. 场景4：std::string的operator+拼接（创建新对象）
void str_operator_plus() {
    cout << "----- std::string operator+拼接 -----" << endl;

    string s1 = "Hello";
    string s2 = "World";

    // 用operator+拼接：s1 + " " + s2，返回新的string对象s3
    string s3 = s1 + " " + s2;
    cout << "s1（" << s1 << "） + \" \" + s2（" << s2 << "） = " << s3 << endl;

    // 注意：operator+不修改原对象，原s1、s2保持不变
    cout << "拼接后原s1：" << s1 << "，原s2：" << s2 << endl;
}

#if 0
int main() {
    str_conversion();         // C风格字符串与std::string转换
    str_find_replace();       // 查找与替换
    str_append_clear();       // 拼接、清空与判空
    str_operator_plus();      // operator+拼接
    return 0;
}
#endif