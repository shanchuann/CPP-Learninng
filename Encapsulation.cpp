#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
/*******************************************************************************************************************
* C++ 面向对象核心特性（封装、构造函数委托、友元函数）
* 面向对象编程（OOP）的核心是将数据（成员变量）和操作数据的行为（成员函数）封装为一个整体（类），同时通过访问控制、
* 构造函数委托、友元等机制，实现代码的安全性、可维护性和灵活性。
*
* 一、封装（Encapsulation）
* 定义：封装是OOP的核心特性之一，通过将类的成员变量设为private（私有），仅允许通过public（公有）接口（getter/setter）
*      访问或修改，隐藏内部实现细节，控制数据的访问权限，避免外部直接操作导致数据异常。
*
* 核心规则与场景：
* 1. 访问权限控制：
*    - private：仅类内部可访问，外部（包括子类）不可直接访问；
*    - public：类内外均可访问，用于提供对外的操作接口（如setName、getName）。
* 2. 常见接口设计：
*    - 读写接口：同时提供setXxx（修改）和getXxx（读取），如name的setName/getName；
*    - 只读接口：仅提供getXxx，不提供setXxx，如age的getAge（避免外部随意修改）；
*    - 只写接口：仅提供setXxx，不提供getXxx，如password的setPassword（保护敏感信息）；
*    - 带验证的接口：在setXxx中添加逻辑验证，确保数据合法性，如age的setAge（仅允许正数）。
* 3. 优势：
*    - 隐藏实现细节：外部无需关心类的内部数据结构，只需调用接口；
*    - 保证数据安全：通过接口验证避免非法数据（如年龄为负）；
*    - 便于维护：后续修改内部实现时，只要接口不变，外部代码无需改动。
*
* 二、构造函数委托（Constructor Delegation，C++11特性）
* 定义：构造函数委托允许一个构造函数调用同一类中的另一个构造函数，避免多个构造函数间的代码重复（代码复用）。
*
* 核心语法：
* class 类名 {
* public:
*     // 目标构造函数（被委托的构造函数）
*     类名(参数1, 参数2, ...) : 成员变量1(参数1), 成员变量2(参数2), ... { ... }
*     // 委托构造函数（调用目标构造函数）
*     类名(参数A) : 类名(参数A, 默认值B, ...) { ... } // 用":"后接类名+参数列表委托
* };
*
* 关键注意事项：
* - 委托构造函数不能同时初始化成员变量（成员初始化列表只能属于目标构造函数）；
* - 避免循环委托（A委托B，B委托A），会导致编译错误；
* - 适用于多个构造函数仅有参数数量/默认值差异的场景，减少重复代码。
*
* 三、友元函数（Friend Function）
* 定义：友元函数是被类声明为“朋友”的外部函数，它可以突破类的访问权限限制，直接访问类的private成员（变量/函数）。
*
* 核心语法与规则：
* 1. 声明方式：在类内部（通常是private区域）用friend关键字声明：
*    friend 返回值类型 函数名(参数列表);
* 2. 实现方式：友元函数定义在类外部，无需加"类名::"（不属于类的成员函数）；
* 3. 关键特性：
*    - 友元关系是单向的：A声明B为友元，不代表B声明A为友元；
*    - 友元关系不可传递：A是B的友元，B是C的友元，不代表A是C的友元；
*    - 友元会破坏封装：仅在“外部函数必须访问类私有成员”的场景使用（如计算类的面积、重载运算符），避免滥用。
*******************************************************************************************************************/

// 1. 场景1：封装（Encapsulation）—— 数据与行为封装+访问权限控制
void encapsulation_demo() {
    cout << "----- 面向对象核心：封装（Encapsulation） -----" << endl;

    // 1.1 基础封装：Circle类（数据：半径；行为：设置半径、计算面积）
    class Circle {
    private:
        double radius; // 私有数据成员：半径（外部不可直接访问）
    public:
        // 公有行为成员：设置半径（接口）
        void setRadius(double r) {
            if (r > 0) radius = r; // 数据验证：仅允许正数半径
            else cout << "错误：半径必须大于0！" << endl;
        }
        // 公有行为成员：计算面积（接口）
        double getArea() const {
            return 3.14 * radius * radius; // 内部使用私有成员
        }
    };

    Circle c1;
    c1.setRadius(5.0); // 通过接口设置半径（合法）
    cout << "圆形半径=5.0，面积=" << c1.getArea() << endl;
    c1.setRadius(-2.0); // 非法值，触发验证提示
    cout << endl;

    // 1.2 精细化权限控制：Person类（读写、只读、只写、带验证）
    class Person {
    private:
        string name;    // 私有数据：姓名
        int age = 0;    // 私有数据：年龄（默认初始化0）
        string password;// 私有数据：密码（敏感信息）
    public:
        // 1. 读写接口：姓名（可修改可读取）
        void setName(const string& n) { name = n; }
        string getName() const { return name; }

        // 2. 只读+带验证接口：年龄（仅允许通过set设置合法值，外部只能读）
        void setAge(int a) {
            if (a > 0 && a <= 150) age = a; // 验证：年龄在1-150之间
            else cout << "错误：年龄必须在1-150之间！" << endl;
        }
        int getAge() const { return age; }

        // 3. 只写接口：密码（仅允许设置，不允许读取，保护敏感信息）
        void setPassword(const string& p) { password = p; }
    };

    Person p;
    // 操作姓名（读写）
    p.setName("Alice");
    cout << "Person姓名：" << p.getName() << endl;

    // 操作年龄（带验证的只读）
    p.setAge(25); // 合法年龄
    cout << "Person年龄：" << p.getAge() << endl;
    p.setAge(200); // 非法年龄，触发提示
    cout << "非法年龄后，Person年龄：" << p.getAge() << endl;

    // 操作密码（只写）
    p.setPassword("my_secret_123");
    // cout << p.password; // 错误：private成员不可直接访问
    // 无getPassword，外部无法读取密码
    cout << "\n" << endl;
}

// 2. 场景2：构造函数委托（C++11特性）—— 减少构造函数代码重复
void constructor_delegation_demo() {
    cout << "----- 构造函数委托（Constructor Delegation） -----" << endl;

    // Employee类：两个构造函数，第二个委托第一个避免重复
    class Employee {
    private:
        string name; // 姓名
        int age;     // 年龄
    public:
        // 1. 目标构造函数（被委托的构造函数）：初始化所有成员
        Employee(const string& n, int a) : name(n), age(a) {
            cout << "目标构造函数调用：姓名=" << name << "，年龄=" << age << endl;
        }

        // 2. 委托构造函数：仅传姓名，年龄默认0，委托给目标构造函数
        Employee(const string& n) : Employee(n, 0) { // 核心："类名(参数)"委托
            cout << "委托构造函数调用：年龄默认0" << endl;
        }
    };

    // 使用构造函数
    cout << "创建emp1（传姓名+年龄）：" << endl;
    Employee emp1("John", 30); // 调用目标构造函数

    cout << "\n创建emp2（仅传姓名）：" << endl;
    Employee emp2("Jane");     // 调用委托构造函数，再触发目标构造函数
    cout << "\n" << endl;
}

// 3. 场景3：友元函数（Friend Function）—— 突破封装访问私有成员
// Rectangle类：私有成员width/height，友元函数getArea访问它们
class Rectangle {
private:
    int width;  // 宽（私有）
    int height; // 高（私有）
public:
    // 构造函数：初始化宽高（避免随机值）
    Rectangle(int w = 3, int h = 4) : width(w), height(h) {}

    // 声明友元函数：getArea可访问本类私有成员
    friend int getArea(const Rectangle& r);
};

// 友元函数定义（类外部，无需加"Rectangle::"）
int getArea(const Rectangle& r) {
    return r.width * r.height;
}
void friend_function_demo() {
    cout << "----- 友元函数（Friend Function） -----" << endl;
    // 使用友元函数
    Rectangle rect1;          // 默认宽3、高4
    Rectangle rect2(5, 6);    // 自定义宽5、高6

    cout << "rect1（宽3，高4）面积：" << getArea(rect1) << endl;
    cout << "rect2（宽5，高6）面积：" << getArea(rect2) << endl;

    // 验证：外部无法直接访问私有成员
    // cout << rect1.width; // 错误：private成员不可直接访问
}
int main() {
    encapsulation_demo();              // 封装特性演示（权限控制、接口设计）
    constructor_delegation_demo();     // 构造函数委托演示（代码复用）
    friend_function_demo();            // 友元函数演示（突破封装）
    return 0;
}
#if 0

#endif