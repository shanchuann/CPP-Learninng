#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

/*******************************************************************************************************************
* C++ 纯虚函数与抽象类核心知识点总结
* 纯虚函数与抽象类是C++实现动态多态的核心机制，用于定义统一的接口规范，强制派生类实现特定功能，是面向对象“接口与实现分离”设计思想的体现。
* 核心内容包括：纯虚函数定义、抽象类定义、构造/析构函数设计、派生类实现要求、典型应用场景。
*
* 一、核心概念
* 1. 纯虚函数：基类中声明为“无默认实现”的虚函数，语法为 `virtual 返回值类型 函数名(参数列表) = 0;`；
* 2. 抽象类：包含至少一个纯虚函数的类，**无法直接实例化对象**；
* 3. 作用：定义统一的接口规范，强制派生类实现纯虚函数，确保多态的一致性。
*
* 二、纯虚函数的定义与特点
* 1. 语法：在虚函数声明后加 `= 0`，表示该函数在基类中无默认实现；
* 2. 特点：
*    - 纯虚函数可以在类外手动实现（语法允许，但不常见）；
*    - 派生类必须重写所有纯虚函数，否则派生类也会成为抽象类；
*    - 纯虚函数的存在使类成为抽象类。
*
* 三、抽象类的定义与特点
* 1. 定义：包含至少一个纯虚函数的类；
* 2. 特点：
*    - 无法直接实例化对象（如 `Shape s;` 会编译报错）；
*    - 可以定义指针和引用（如 `Shape* s;` 合法），用于指向派生类对象实现多态；
*    - 可以包含普通成员函数和成员变量，供派生类复用。
*
* 四、抽象类的构造函数与析构函数
* 1. 构造函数：
*    - 通常声明为 `protected`，防止直接实例化抽象类，但允许派生类调用；
*    - 用于初始化抽象类的成员变量，供派生类使用。
* 2. 析构函数：
*    - 必须声明为 `virtual`，确保通过基类指针删除派生类对象时，先调用派生类析构，再调用基类析构，防止内存泄漏；
*    - 可以使用 `= default` 让编译器生成默认虚析构函数。
*
* 五、派生类的实现要求
* 1. 必须重写基类的**所有纯虚函数**，否则派生类也会成为抽象类；
* 2. 重写时推荐使用 `override` 关键字（C++11），显式标记重写，避免拼写错误或参数不匹配；
* 3. 通过基类指针/引用调用重写的纯虚函数，实现动态多态。
*
* 六、典型应用场景
* 1. 定义统一的接口规范：如图形类的draw()、getArea()，强制所有图形派生类实现绘制和计算面积的功能；
* 2. 实现“接口与实现分离”：基类只定义接口，派生类负责具体实现，提高代码的可扩展性和可维护性；
* 3. 多态编程：通过基类指针/引用操作不同派生类对象，无需关心对象的具体类型。
*******************************************************************************************************************/

// 抽象类
class Shape {
protected:
    // 抽象类的构造函数声明为protected，防止直接实例化，但允许派生类调用
    Shape() = default;
public:
    // 虚析构函数，确保通过基类指针删除派生类对象时，调用正确的析构顺序
    virtual ~Shape() = default;
    // 纯虚函数：绘制图形（强制派生类实现）
    virtual void draw() const = 0;
    // 纯虚函数：计算面积（强制派生类实现）
    virtual double getArea() const = 0;
};

// 派生类：圆形
class Circle : public Shape {
private:
    double radius; // 圆形半径
public:
    Circle(double r) : radius(r) {}
    // 重写draw函数（override显式标记）
    void draw() const override {
        std::cout << "绘制圆形，半径为 " << radius << std::endl;
    }
    // 重写getArea函数
    double getArea() const override {
        return 3.14159 * radius * radius;
    }
};

// 派生类：矩形
class Rectangle : public Shape {
private:
    double width;  // 矩形宽度
    double height; // 矩形高度
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    // 重写draw函数
    void draw() const override {
        std::cout << "绘制矩形，宽为 " << width << "，高为 " << height << std::endl;
    }
    // 重写getArea函数
    double getArea() const override {
        return width * height;
    }
};

#if 0
int main() {
    // 创建派生类对象，用基类指针指向（实现多态）
    Shape* circle = new Circle(5.0);
    Shape* rectangle = new Rectangle(4.0, 6.0);

    // 通过基类指针调用派生类的方法，实现动态多态
    circle->draw();
    std::cout << "圆形面积：" << circle->getArea() << std::endl;

    rectangle->draw();
    std::cout << "矩形面积：" << rectangle->getArea() << std::endl;

    // 释放内存，虚析构函数确保调用正确的析构顺序
    delete circle;
    delete rectangle;

    return 0;
}
#endif