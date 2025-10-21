#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
/*******************************************************************************************************************
* C++ 构造函数与析构函数（Constructor & Destructor）
* 构造函数和析构函数是类的特殊成员函数，分别负责对象的初始化和资源清理，是面向对象中管理对象生命周期的核心机制。
* 同时，所有非静态成员函数都隐含一个this指针，用于指向当前调用函数的对象，区分成员变量与局部变量。
*
* 一、构造函数（Constructor）
* 定义：构造函数是与类同名的特殊成员函数，在**创建对象时自动调用**，核心作用是初始化对象的成员变量（如分配内存、赋值初始值）。
*
* 核心特性与分类：
* 1. 基本语法：
*    - 函数名与类名完全一致，无返回值（连void都不写）；
*    - 可重载（支持多个参数列表不同的构造函数）。
* 2. 常见类型：
*    - 无参构造函数：无参数，若未手动定义任何构造函数，编译器会自动生成“默认构造函数”（空实现）；
*      若手动定义了其他构造函数，默认构造函数需显式定义（否则无法创建无参对象）。
*    - 带参构造函数：含参数，用于创建对象时直接初始化成员变量（避免创建后再调用setter）；
*    - 默认构造函数：包括“编译器自动生成的无参构造”和“手动定义的无参构造”，满足“无参创建对象”的需求。
* 3. 关键注意事项：
*    - 构造函数不能被显式调用（由编译器在对象创建时自动触发）；
*    - 若类包含动态内存（如new分配的数组），需在构造函数中完成内存分配。
*
* 二、析构函数（Destructor）
* 定义：析构函数是与类同名、前缀加~的特殊成员函数，在**对象销毁时自动调用**（如离开作用域、delete对象），核心作用是清理资源（如释放动态内存、关闭文件）。
*
* 核心特性：
* 1. 基本语法：
*    - 函数名：~类名（与类名相同，前缀加~）；
*    - 无参数、无返回值；
*    - 不可重载（一个类只能有一个析构函数）。
* 2. 核心作用：
*    - 释放构造函数中分配的动态内存（避免内存泄漏）；
*    - 清理其他资源（如关闭文件句柄、释放网络连接）。
* 3. 关键注意事项：
*    - 若类无动态资源（仅基础类型成员），编译器会自动生成默认析构函数（空实现），无需手动定义；
*    - 动态内存匹配：new对应delete，new[]对应delete[]（析构函数中必须严格匹配，否则内存泄漏或崩溃）。
*
* 三、this指针（隐含的成员函数参数）
* 定义：this指针是一个隐含在**非静态成员函数**中的常量指针（Type* const this），指向当前调用该函数的对象。
*
* 核心作用：
* 1. 区分成员变量与局部变量：当成员变量与函数参数同名时，用this->成员变量表示对象的成员（如this->row = r）；
* 2. 隐含传递：调用成员函数时，编译器自动将当前对象的地址作为this指针传入函数（用户无需手动传递）；
* 3. 不可显式声明：this指针是编译器隐含的，不能在函数参数列表中写出来，但可在函数体内使用。
*******************************************************************************************************************/

// 1. 场景1：this指针的隐含使用（非静态成员函数的隐含参数）
class Pointer {
private:
    int row, col; // 成员变量：行、列
public:
    // 成员函数：设置行（隐含this指针，等价于void SetRow(Pointer* const this, int r)）
    void SetRow(int r) {
        this->row = r; // this->row 明确访问当前对象的成员变量（可省略this->，编译器自动识别）
    }

    // 成员函数：设置列（隐含this指针，等价于void SetCol(Pointer* const this, int c)）
    void SetCol(int c) {
        col = c; // 省略this->，编译器自动通过this指针访问成员变量
    }

    // 类外实现的成员函数（仍隐含this指针）
    int GetRow() const; // const修饰成员函数：this指针变为const Pointer* const（不能修改对象成员）
    int GetCol() const;
};

// 类外实现成员函数
int Pointer::GetRow() const {
    return this->row; // const成员函数中，this指针指向的对象不可修改
}
int Pointer::GetCol() const {
    return col;
}
void this_pointer_demo() {
    cout << "----- this指针的隐含使用 -----" << endl;

    // 创建多个对象，验证this指针指向不同对象
    Pointer pa, pb, pc, pd;
    pa.SetRow(10); // 编译器自动传递&pa作为this指针：SetRow(&pa, 10)
    pb.SetRow(20); // 传递&pb作为this指针：SetRow(&pb, 20)
    pc.SetCol(30); // 传递&pc作为this指针：SetCol(&pc, 30)
    pd.SetCol(40); // 传递&pd作为this指针：SetCol(&pd, 40)

    cout << "pa的行：" << pa.GetRow() << endl; // 10（this指向pa）
    cout << "pb的行：" << pb.GetRow() << endl; // 20（this指向pb）
    cout << "pc的列：" << pc.GetCol() << endl; // 30（this指向pc）
    cout << "pd的列：" << pd.GetCol() << endl; // 40（this指向pd）
    cout << "\n" << endl;
}

// 2. 场景2：构造函数与析构函数（处理动态内存，避免内存泄漏）
void constructor_destructor_demo() {
    cout << "----- 构造函数与析构函数（动态内存管理） -----" << endl;

    class Array {
    private:
        int* data; // 动态数组指针（需手动分配/释放内存）
        int size;  // 数组大小
    public:
        // 带参构造函数：创建对象时分配动态内存
        Array(int s) {
            if (s <= 0) {
                size = 0;
                data = nullptr;
                cout << "构造函数：数组大小非法，未分配内存" << endl;
                return;
            }
            size = s;
            data = new int[size]; // 分配size个int的动态内存（new[]对应delete[]）
            cout << "构造函数：分配了" << size << "个int的动态内存（地址：" << (void*)data << "）" << endl;
        }

        // 析构函数：对象销毁时释放动态内存
        ~Array() {
            if (data != nullptr) {
                delete[] data; // 释放new[]分配的内存（必须匹配，否则内存泄漏）
                data = nullptr; // 避免野指针
                cout << "析构函数：释放了" << size << "个int的动态内存" << endl;
            }
            else {
                cout << "析构函数：无内存可释放" << endl;
            }
        }

        // 成员函数：给数组元素赋值
        void set(int index, int value) {
            if (index >= 0 && index < size && data != nullptr) {
                data[index] = value;
                cout << "数组索引" << index << "赋值为" << value << endl;
            }
        }
    };

    // 测试：对象在局部作用域中的生命周期
    {
        cout << "\n创建Array对象arr（大小5）：" << endl;
        Array arr(5); // 创建对象，调用构造函数（分配内存）
        arr.set(0, 100); // 给数组第0个元素赋值
        arr.set(2, 200); // 给数组第2个元素赋值
    } // 离开局部作用域，对象arr销毁，自动调用析构函数（释放内存）

    // 测试：非法大小的数组
    cout << "\n创建Array对象err_arr（大小-3）：" << endl;
    Array err_arr(-3); // 构造函数判断大小非法，不分配内存
    cout << "\n" << endl;
}

// 3. 场景3：构造函数重载（无参构造 vs 带参构造）
void constructor_overload_demo() {
    cout << "----- 构造函数重载（无参 vs 带参） -----" << endl;

    class Person {
    private:
        string name; // 姓名
        int age;     // 年龄
    public:
        // 无参构造函数（手动定义，满足无参创建对象的需求）
        Person() {
            name = "Unknown"; // 初始化默认姓名
            age = 0;         // 初始化默认年龄
            cout << "无参构造函数被调用" << endl;
        }

        // 带参构造函数（重载，创建对象时直接初始化）
        Person(const string& n, int a) {
            name = n; // 用参数初始化姓名
            age = a;  // 用参数初始化年龄
            cout << "带参构造函数被调用" << endl;
        }

        // 成员函数：展示对象信息
        void show() const {
            cout << "Name: " << name << ", Age: " << age << endl;
        }
    };

    // 调用无参构造函数创建对象
    cout << "\n创建无参对象p1：" << endl;
    Person p1;
    p1.show(); // 输出默认初始化的值

    // 调用带参构造函数创建对象
    cout << "\n创建带参对象p2（Tom, 18）：" << endl;
    Person p2("Tom", 18);
    p2.show(); // 输出参数初始化的值
}

#if 0
int main() {
    this_pointer_demo();                // this指针的隐含使用演示
    constructor_destructor_demo();      // 构造与析构（动态内存管理）演示
    constructor_overload_demo();        // 构造函数重载（无参vs带参）演示
    return 0;
}
#endif