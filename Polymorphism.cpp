#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <typeinfo>

/*******************************************************************************************************************
* C++ 多态（Polymorphism）核心知识点总结
* 多态是面向对象编程（OOP）三大特性之一，核心是“同一接口，不同实现”，根据绑定时机分为静态多态（编译期）和动态多态（运行期）。
* 核心内容包括：多态分类与绑定机制、静态多态（函数重载、运算符重载、模板）、动态多态（虚函数、抽象类、协变）、RTTI、虚函数表底层原理。
*
* 一、多态的核心分类与绑定机制
* 1. 静态多态（编译期绑定）：
*    - 定义：在**编译阶段**根据参数类型/数量/模板参数确定调用的具体实现；
*    - 特点：效率高、无运行时开销，但灵活性较低；
*    - 典型实现：函数重载、运算符重载、模板。
* 2. 动态多态（运行期绑定）：
*    - 定义：在**程序运行阶段**根据对象的**实际类型**确定调用的具体实现；
*    - 特点：灵活性高、可扩展性强，但有少量运行时开销；
*    - 核心条件：虚函数、基类指针/引用、派生类重写虚函数。
*
* 二、静态多态（编译期绑定）
* 1. 函数重载：
*    - 定义：同一作用域内，函数名相同但**参数列表（类型/数量/顺序）不同**的函数；
*    - 特点：编译期根据参数匹配规则选择最优实现；
*    - 注意：返回值类型不能作为重载依据。
* 2. 运算符重载：
*    - 定义：为自定义类型重新定义C++已有运算符的行为；
*    - 特点：保留运算符优先级、结合性、操作数个数；
*    - 实现方式：类成员函数、全局友元函数。
* 3. 模板：
*    - 定义：使用类型/非类型参数的通用代码框架，编译期根据实参生成具体类型/函数；
*    - 分类：函数模板、类模板；
*    - 特点：代码复用性强、类型安全、编译期生成无运行时开销。
*
* 三、动态多态（运行期绑定）
* 1. 核心实现条件：
*    - 基类声明**虚函数**（virtual 关键字）；
*    - 派生类**重写**（override 关键字，C++11推荐）基类虚函数；
*    - 通过**基类指针/引用**调用虚函数。
* 2. 虚析构函数：
*    - 定义：基类析构函数声明为 virtual；
*    - 作用：通过基类指针删除派生类对象时，**先调用派生类析构，再调用基类析构**，防止内存泄漏；
*    - 规则：只要类可能被继承，基类析构函数必须声明为 virtual。
* 3. 纯虚函数与抽象类：
*    - 纯虚函数：基类中声明为 `virtual 返回值类型 函数名() = 0;` 的虚函数，无默认实现（类外可手动实现但不常见）；
*    - 抽象类：包含至少一个纯虚函数的类，**无法实例化对象**；
*    - 作用：强制派生类重写纯虚函数，定义统一的接口规范。
* 4. 协变返回类型：
*    - 定义：派生类重写虚函数时，返回值类型可以是基类虚函数返回值类型的**公有派生类指针/引用**；
*    - 作用：避免向下类型转换，提高代码可读性和安全性；
*    - 规则：仅适用于指针/引用返回值，必须是公有继承的派生类。
*
* 四、RTTI（运行时类型识别，Run-Time Type Identification）
* 1. dynamic_cast：
*    - 定义：安全的**向下类型转换**（基类指针/引用 → 派生类指针/引用）；
*    - 特点：仅适用于包含虚函数的类，转换失败返回 nullptr（指针）或抛出 bad_cast 异常（引用）；
*    - 作用：在运行期判断基类指针/引用指向的实际类型，安全调用派生类特有功能。
* 2. typeid：
*    - 定义：获取对象/类型的运行时类型信息，返回 type_info 类的引用；
*    - 特点：type_info 类支持 ==、!= 比较，name() 函数返回类型名称（不同编译器格式不同）；
*    - 作用：在运行期判断两个对象的类型是否相同。
*
* 五、虚函数表与虚函数指针（底层原理）
* 1. 虚函数表（vtable）：
*    - 定义：每个包含虚函数的类都有一张虚函数表，存储该类所有虚函数的地址；
*    - 特点：派生类重写基类虚函数时，会覆盖虚函数表中对应位置的地址；
*    - 内容：基类未被重写的虚函数地址、派生类重写的虚函数地址、派生类新增的虚函数地址。
* 2. 虚函数指针（vptr）：
*    - 定义：每个包含虚函数的类的对象都有一个隐藏的虚函数指针，指向该类的虚函数表；
*    - 特点：虚函数指针存储在对象的**前几个字节**（64位系统8字节，32位系统4字节）；
*    - 作用：运行期通过虚函数指针找到虚函数表，再根据虚函数在表中的索引调用对应实现。
*******************************************************************************************************************/

// ===================== 1. 静态多态 - 函数重载 =====================
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
    double add(double a, double b) { return a + b; }
};

void testFunctionOverload() {
    std::cout << "\n=== 静态多态 - 函数重载 ===" << std::endl;
    Calculator calc;
    std::cout << "两个整数相加：" << calc.add(10, 20) << std::endl;
    std::cout << "三个整数相加：" << calc.add(10, 20, 30) << std::endl;
    std::cout << "两个浮点数相加：" << calc.add(1.5, 2.5) << std::endl;
}

// ===================== 2. 静态多态 - 运算符重载（见Operator.cpp） =====================
//class Complex {
//private:
//    double real;
//    double imag;
//public:
//    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
//    Complex operator+(const Complex& other) const {
//        return Complex(real + other.real, imag + other.imag);
//    }
//    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
//    void show() const {
//        std::cout << real << " + " << imag << "i" << std::endl;
//    }
//};
//
//std::ostream& operator<<(std::ostream& os, const Complex& c) {
//    os << c.real << " + " << c.imag << "i";
//    return os;
//}
//
//void testOperatorOverload() {
//    std::cout << "\n=== 静态多态 - 运算符重载 ===" << std::endl;
//    Complex c1(1.0, 2.0);
//    Complex c2(3.0, 4.0);
//    Complex c3 = c1 + c2;
//    std::cout << "复数相加结果：" << c3 << std::endl;
//}

// ===================== 3. 静态多态 - 模板 =====================
template <typename T>
T add(T a, T b) {
    return a + b;
}

template <typename T, int size>
class Array {
private:
    T arr[size];
public:
    void set(int index, T value) {
        if (index >= 0 && index < size) arr[index] = value;
    }
    T get(int index) const { return arr[index]; }
};

void testTemplate() {
    std::cout << "\n=== 静态多态 - 模板 ===" << std::endl;
    std::cout << "整数相加：" << add<int>(10, 20) << std::endl;
    std::cout << "浮点数相加：" << add<double>(1.5, 2.5) << std::endl;
    std::cout << "自动推导类型相加：" << add(30, 40) << std::endl;

    Array<int, 5> intArr;
    intArr.set(0, 100);
    std::cout << "整数数组第一个元素：" << intArr.get(0) << std::endl;
    Array<double, 3> doubleArr;
    doubleArr.set(1, 3.14);
    std::cout << "浮点数数组第二个元素：" << doubleArr.get(1) << std::endl;
}

// ===================== 4. 动态多态 - 基础虚函数 =====================
class Shape {
public:
    virtual void draw() const {
        std::cout << "绘制一个通用图形" << std::endl;
    }
    virtual ~Shape() {}
};

class Circle : public Shape {
public:
    void draw() const override {
        std::cout << "绘制一个圆形" << std::endl;
    }
};

class Rectangle : public Shape {
public:
    void draw() const override {
        std::cout << "绘制一个矩形" << std::endl;
    }
};

void drawShape(const Shape* shape) {
    shape->draw();
}

void testBasicDynamicPolymorphism() {
    std::cout << "\n=== 动态多态 - 基础虚函数 ===" << std::endl;
    Shape* s1 = new Circle();
    Shape* s2 = new Rectangle();
    drawShape(s1);
    drawShape(s2);
    delete s1;
    delete s2;
}

// ===================== 5. 动态多态 - 纯虚函数与抽象类 =====================
class Animal {
public:
    virtual void makeSound() const = 0;
    void eat() const { std::cout << "动物在吃东西" << std::endl; }
    virtual ~Animal() {}
};

void Animal::makeSound() const {
    std::cout << "动物发出通用的声音" << std::endl;
}

class Dog : public Animal {
public:
    void makeSound() const override { std::cout << "汪汪汪" << std::endl; }
    void makeBaseSound() const { Animal::makeSound(); }
};

class Cat : public Animal {
public:
    void makeSound() const override { std::cout << "喵喵喵" << std::endl; }
};

void testAbstractClass() {
    std::cout << "\n=== 动态多态 - 纯虚函数与抽象类 ===" << std::endl;
    Animal* d = new Dog();
    Animal* c = new Cat();
    d->makeSound();
    c->makeSound();
    d->eat();
    static_cast<Dog*>(d)->makeBaseSound();
    delete d;
    delete c;
}

// ===================== 6. 动态多态 - 协变返回类型 =====================
class Product {
public:
    virtual void show() const { std::cout << "这是一个通用产品" << std::endl; }
    virtual ~Product() {}
};

class ProductA : public Product {
public:
    void show() const override { std::cout << "这是产品A" << std::endl; }
    void specificFuncA() const { std::cout << "产品A的特有功能" << std::endl; }
};

class Factory {
public:
    virtual Product* createProduct() const { return new Product(); }
    virtual ~Factory() {}
};

class FactoryA : public Factory {
public:
    ProductA* createProduct() const override { return new ProductA(); }
};

void testCovariantReturnType() {
    std::cout << "\n=== 动态多态 - 协变返回类型 ===" << std::endl;
    Factory* f = new FactoryA();
    Product* p = f->createProduct();
    p->show();
    ProductA* pa = static_cast<ProductA*>(p);
    pa->specificFuncA();
    delete p;
    delete f;
}

// ===================== 7. RTTI - dynamic_cast 与 typeid =====================
class Base_RTTI {
public:
    virtual void func() const { std::cout << "Base_RTTI::func" << std::endl; }
    virtual ~Base_RTTI() {}
};

class Derived_RTTI : public Base_RTTI {
public:
    void func() const override { std::cout << "Derived_RTTI::func" << std::endl; }
    void derivedFunc() const { std::cout << "Derived_RTTI::derivedFunc（特有功能）" << std::endl; }
};

void testRTTI() {
    std::cout << "\n=== RTTI - dynamic_cast 与 typeid ===" << std::endl;
    Base_RTTI* b = new Derived_RTTI();

    Derived_RTTI* d = dynamic_cast<Derived_RTTI*>(b);
    if (d != nullptr) {
        std::cout << "dynamic_cast转换成功" << std::endl;
        d->derivedFunc();
    }
    else {
        std::cout << "dynamic_cast转换失败" << std::endl;
    }

    std::cout << "b的类型：" << typeid(*b).name() << std::endl;
    std::cout << "Derived_RTTI的类型：" << typeid(Derived_RTTI).name() << std::endl;
    if (typeid(*b) == typeid(Derived_RTTI)) {
        std::cout << "b指向的是Derived_RTTI对象" << std::endl;
    }

    delete b;
}

// ===================== 8. 虚函数表与虚函数指针（底层原理演示） =====================
class Base_VTable {
public:
    virtual void func1() { std::cout << "Base_VTable::func1" << std::endl; }
    virtual void func2() { std::cout << "Base_VTable::func2" << std::endl; }
    void nonVirtualFunc() { std::cout << "Base_VTable::nonVirtualFunc" << std::endl; }
};

class Derived_VTable : public Base_VTable {
public:
    void func1() override { std::cout << "Derived_VTable::func1" << std::endl; }
    void func3() { std::cout << "Derived_VTable::func3" << std::endl; }
};

void testVTable() {
    std::cout << "\n=== 虚函数表与虚函数指针（底层原理演示） ===" << std::endl;
    Base_VTable b;
    Derived_VTable d;
    std::cout << "Base_VTable 对象大小：" << sizeof(b) << std::endl;
    std::cout << "Derived_VTable 对象大小：" << sizeof(d) << std::endl;

    using FuncPtr = void (*)();
    Base_VTable* ptr = &d;
    long* vptr = (long*)ptr;
    long* vtable = (long*)*vptr;

    FuncPtr func = (FuncPtr)vtable[0];
    func();
    func = (FuncPtr)vtable[1];
    func();
}
#if 0
int main() {
    testFunctionOverload();
    //testOperatorOverload();
    testTemplate();
    testBasicDynamicPolymorphism();
    testAbstractClass();
    testCovariantReturnType();
    testRTTI();
    testVTable();
    return 0;
}
#endif