#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>

/*******************************************************************************************************************
* 命名空间（Namespace）与作用域（Scope）
*
* 一、命名空间
* 定义：命名空间是C++中用于组织代码、避免命名冲突的机制，可将函数、类、变量等封装在一个独立的作用域内，
*      通过命名空间名访问其成员，解决不同模块中同名标识符的冲突问题。
*
* 核心特性：
* 1. 基本定义：用namespace关键字声明，可包含函数、变量、类等成员。
* 2. 可扩展性：同一命名空间可分多个块定义（甚至在不同文件中），最终会合并为一个完整的命名空间。
* 3. 嵌套命名空间：命名空间内部可嵌套其他命名空间，形成层次结构，适合大型项目的模块化组织。
* 4. 命名空间别名：通过namespace 别名 = 原命名空间; 简化长嵌套命名空间的访问。
* 5. 访问控制：
*    - 显式访问：命名空间名::成员名（如Math::add）；
*    - using声明：using 命名空间::成员名; 仅在当前作用域引入单个成员；
*    - using指令：using namespace 命名空间; 引入整个命名空间的所有成员（需谨慎使用，避免冲突）。
*
* 二、作用域
* 定义：作用域是标识符（变量、函数等）可见并可被访问的范围，超出范围后标识符不可见。
* 常见作用域类型：
* 1. 全局作用域：定义在所有函数、类、命名空间外部的标识符，在整个程序中可见（除非被局部标识符隐藏）。
* 2. 局部作用域：定义在函数内部的标识符，仅在该函数内可见。
* 3. 块作用域：定义在{}内的标识符（如if、for、自定义块），仅在该块内可见。
* 4. 类作用域：类的成员（成员变量、成员函数），需通过类对象或类名访问（静态成员）。
* 5. 命名空间作用域：命名空间内的标识符，需通过命名空间访问或通过using声明/指令引入。
*******************************************************************************************************************/

// 1. 基本命名空间定义与使用
namespace Math {
    // 命名空间内的函数
    int add(int a, int b) { return a + b; }
    int mul(int a, int b) { return a * b; }
}

// 2. 命名空间的扩展（分块定义，最终合并）
// 第一块：声明命名空间A及成员Namef
namespace A {
    void Namef() {
        std::cout << "Function Namef in namespace A" << std::endl;
    }
}

// 第二块：扩展命名空间A，添加成员Nameg（同一命名空间可拆分定义）
namespace A {
    void Nameg() {
        std::cout << "Function Nameg in namespace A" << std::endl;
    }
}

// 3. 嵌套命名空间（层次化组织）
namespace Company {          // 公司级命名空间
    namespace Project {      // 项目级命名空间
        namespace Utils {    // 工具模块命名空间
            namespace Log {  // 日志子模块命名空间
                // 嵌套命名空间内的函数
                void info(const std::string& msg) {
                    std::cout << "[INFO] " << msg << std::endl;
                }
            }
        }
    }
}

// 3.1 命名空间别名（简化长嵌套命名空间的访问）
namespace Log = Company::Project::Utils::Log;  // 为长命名空间定义别名


// 4. using声明的使用（在特定作用域内引入命名空间成员）
void calc() {
    std::cout << "\n----- using声明示例 -----" << std::endl;
    // using声明：仅在calc()函数内引入Math::add，可直接使用add
    using Math::add;
    std::cout << "add(2, 3) = " << add(2, 3) << std::endl;  // 等价于Math::add

    // 未引入Math::mul，需显式通过命名空间访问
    std::cout << "Math::mul(2, 3) = " << Math::mul(2, 3) << std::endl;
}


// 5. 不同类型的作用域示例
// 5.1 全局作用域：定义在所有函数/类外部
int Global_scope = 10;  // 全局变量

// 5.2 类作用域：类的成员仅在类内部或通过对象访问
class ClassScope {
private:
    int Class_scope;  // 类作用域变量（仅类内部可见）
public:
    void setValue(int val) {
        Class_scope = val;  // 类内部可直接访问类作用域成员
    }
};

// 5.3 局部作用域与块作用域
void scopeDemo() {
    std::cout << "\n----- 作用域示例（全局、局部、块） -----" << std::endl;
    // 局部作用域：函数内部定义，仅在scopeDemo()内可见
    int Local_scope = 20;

    // 块作用域：{}内定义，仅在该块内可见
    {
        int Block_scope = 30;  // 块作用域变量
        std::cout << "Block_scope: " << Block_scope << std::endl;  // 块内可访问
        std::cout << "Local_scope（块内访问）: " << Local_scope << std::endl;  // 可访问外部局部变量
        std::cout << "Global_scope（块内访问）: " << Global_scope << std::endl;  // 可访问全局变量
    }

    // 块外不可访问Block_scope
    // cout << Block_scope << endl;  // 编译错误：未声明的标识符

    std::cout << "Local_scope（块外访问）: " << Local_scope << std::endl;  // 局部作用域内可访问
    std::cout << "Global_scope（函数内访问）: " << Global_scope << std::endl;  // 可访问全局变量
}

#if 0
int main() {
    using namespace std;
    // 访问扩展后的命名空间A
    cout << "----- 命名空间扩展示例 -----" << endl;
    A::Namef();  // 调用A中的Namef
    A::Nameg();  // 调用A中的Nameg（扩展的成员）

    // 访问嵌套命名空间（通过别名）
    cout << "\n----- 嵌套命名空间与别名示例 -----" << endl;
    Log::info("Program start");  // 等价于Company::Project::Utils::Log::info

    // 调用包含using声明的函数
    calc();

    // 演示不同作用域
    scopeDemo();

    return 0;
}
#endif