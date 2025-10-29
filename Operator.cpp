#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

/*******************************************************************************************************************
* C++ 运算符重载（Operator Overloading）
* 定义：运算符重载是C++允许用户为自定义类型（如类、结构体）重新定义运算符的行为，使运算符能像操作基本类型（int、double）一样
*      操作自定义对象，提高代码的可读性和直观性。本质是“通过特殊函数（operator运算符）实现的函数重载”。
*
* 核心规则：
* 1. 不能创造新运算符：只能重载C++已有的运算符（如+、-、==、++、<<等），不能自定义新运算符（如#、@）。
* 2. 运算符优先级和结合性不变：重载后运算符的优先级、结合性、操作数个数与原运算符一致（如+仍为左结合，优先级不变）。
* 3. 必须操作自定义类型：至少有一个操作数是自定义类型（不能重载基本类型的运算符，如int+int的行为不能修改）。
* 4. 两种实现方式：
*    - 类成员函数：运算符函数作为类的成员，左操作数为当前对象（this指针指向），右操作数为函数参数；
*    - 全局函数（常为友元）：运算符函数为全局函数，左右操作数均为函数参数，若需访问类私有成员，需声明为友元。
*
* 常见重载场景：
* 1. 算术运算符（+、-、*、/）：实现自定义对象的加减乘除（如复数相加、矩阵运算）；
* 2. 关系运算符（==、!=、>、<）：实现自定义对象的比较（如判断两个对象的属性是否相等）；
* 3. 自增/自减运算符（++、--）：区分前缀（++a）和后缀（a++），后缀需加一个无用的int参数作为标记；
* 4. 流运算符（<<、>>）：实现自定义对象的输入输出（如用cout输出复数、cin输入对象属性），需重载为全局友元函数。
*******************************************************************************************************************/

// 1. 场景1：算术运算符重载（成员函数实现，以复数相加为例）
void arithmetic_operator_overload() {
    std::cout << "----- 算术运算符重载（成员函数：复数相加） -----" << std::endl;

    class Complex {
    public:
        double real; // 实部
        double imag; // 虚部

        // 构造函数：初始化复数
        Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

        // 重载+运算符（成员函数）：左操作数为this指针指向的对象，右操作数为other
        Complex operator+(const Complex& other) const {
            // 复数相加规则：(a+bi) + (c+di) = (a+c) + (b+d)i
            return Complex(real + other.real, imag + other.imag);
        }
    };

    // 使用重载的+运算符
    Complex c1(1.0, 2.0);  // 1 + 2i
    Complex c2(3.0, 4.0);  // 3 + 4i
    Complex c3 = c1 + c2;  // 等价于 c1.operator+(c2)

    std::cout << "c1 = " << c1.real << " + " << c1.imag << "i" << std::endl;
    std::cout << "c2 = " << c2.real << " + " << c2.imag << "i" << std::endl;
    std::cout << "c3 = c1 + c2 = " << c3.real << " + " << c3.imag << "i" << "\n" << std::endl;
}

// 2. 场景2：关系运算符重载（全局函数实现，以对象属性比较为例）
class Person {
public:
    int age; // 年龄属性

    // 构造函数：初始化年龄
    Person(int a = 0) : age(a) {}

    // 声明==运算符为全局友元（需访问私有成员时使用，本例age为public可省略友元声明）
    friend bool operator==(const Person& p1, const Person& p2);
};

// 重载==运算符（全局函数）：左右操作数均为函数参数
bool operator==(const Person& p1, const Person& p2) {
    // 比较两个Person对象的age属性
    return p1.age == p2.age;
}
void relation_operator_overload() {
    std::cout << "----- 关系运算符重载（全局函数：对象年龄比较） -----" << std::endl;

    // 使用重载的==运算符
    Person p1(18), p2(18), p3(20);
    std::cout << "p1.age = " << p1.age << ", p2.age = " << p2.age << std::endl;
    if (p1 == p2) { // 等价于 operator==(p1, p2)
        std::cout << "p1 和 p2 年龄相等" << std::endl;
    }
    else {
        std::cout << "p1 和 p2 年龄不等" << std::endl;
    }

    std::cout << "p1.age = " << p1.age << ", p3.age = " << p3.age << std::endl;
    if (p1 == p3) {
        std::cout << "p1 和 p3 年龄相等" << std::endl;
    }
    else {
        std::cout << "p1 和 p3 年龄不等" << std::endl;
    }
    std::cout << "\n" << std::endl;
}

// 3. 场景3：自增运算符重载（成员函数实现，区分前缀和后缀）
void increment_operator_overload() {
    std::cout << "----- 自增运算符重载（成员函数：区分前缀++和后缀++） -----" << std::endl;

    class UPInt {
    private:
        int value; // 存储自增的值
    public:
        // 构造函数：初始化值
        UPInt(int v = 0) : value(v) {}

        // 1. 前缀自增运算符（++a）：无参数，返回对象引用（支持链式操作）
        UPInt& operator++() {
            ++value; // 先自增
            return *this; // 返回自增后的当前对象
        }

        // 2. 后缀自增运算符（a++）：需加int参数（无实际意义，仅作为标记），返回const临时对象
        const UPInt operator++(int) {
            UPInt temp = *this; // 保存自增前的对象
            ++value; // 后自增
            return temp; // 返回自增前的临时对象
        }

        // 辅助函数：输出值（便于验证）
        void show() const {
            std::cout << "value = " << value << std::endl;
        }
    };

    // 使用重载的自增运算符
    UPInt a(10);
    std::cout << "初始a：";
    a.show();

    ++a; // 调用前缀自增：operator++()
    std::cout << "++a（前缀）后：";
    a.show();

    a++; // 调用后缀自增：operator++(0)（编译器自动传入int参数）
    std::cout << "a++（后缀）后：";
    a.show();

    // 验证前缀和后缀的返回值差异
    UPInt b(5), c(5);
    UPInt d = ++b; // 前缀返回自增后对象，d = 6
    UPInt e = c++; // 后缀返回自增前对象，e = 5
    std::cout << "++b后，b："; b.show(); std::cout << "d = ++b："; d.show();
    std::cout << "c++后，c："; c.show(); std::cout << "e = c++："; e.show();
    std::cout << "\n" << std::endl;
}

// 4. 场景4：流运算符重载（全局友元函数，实现对象的cout输出）
class Complex {
public:
    double real; // 实部
    double imag; // 虚部

    // 构造函数：初始化复数
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 声明<<运算符为友元（需访问私有成员real和imag）
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};
// 重载<<运算符（全局友元函数）：左操作数为ostream对象（如cout），右操作数为Complex对象
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    // 自定义输出格式：(实部, 虚部)
    os << "(" << c.real << ", " << c.imag << ")";
    return os; // 返回ostream对象，支持链式输出（如cout << c1 << c2）
}

void stream_operator_overload() {
    std::cout << "----- 流运算符重载（全局友元：cout输出复数） -----" << std::endl;

    // 使用重载的<<运算符
    Complex c1(1.0, 2.0), c2(3.0, 4.0);
    std::cout << "c1 = " << c1 << std::endl; // 等价于 operator<<(cout, c1)
    std::cout << "c2 = " << c2 << std::endl; // 等价于 operator<<(cout, c2)
    std::cout << "c1 和 c2 分别为：" << c1 << " 和 " << c2 << std::endl; // 链式输出
}

#if 0
int main() {
    arithmetic_operator_overload();   // 算术运算符重载（复数相加）
    relation_operator_overload();     // 关系运算符重载（对象比较）
    increment_operator_overload();    // 自增运算符重载（前缀vs后缀）
    stream_operator_overload();       // 流运算符重载（cout输出对象）
    return 0;
}
#endif