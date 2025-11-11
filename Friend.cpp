#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>

/*******************************************************************************************************************
* C++ 友元机制（Friend Mechanism）
* 友元是C++中打破类封装性的特殊机制，允许指定的外部函数、成员函数或类访问当前类的私有（private）和保护（protected）成员。
* 核心作用：在保证封装性的基础上，为特定场景（如运算符重载、跨类协作）提供灵活的访问权限，避免为了访问私有成员而暴露过多public接口。
*
* 友元的三种形式及特点：
* 1. 友元函数（Friend Function）
*    - 定义：全局函数被类声明为友元，可直接访问类的私有成员。
*    - 语法：在类内部用`friend 返回值类型 函数名(参数列表);`声明，函数定义在类外部（无需加类名::）。
*    - 适用场景：需要访问多个类私有成员的函数（如比较两个不同类的对象）、重载流运算符（<<、>>）等。
*
* 2. 友元类（Friend Class）
*    - 定义：类A被类B声明为友元，则类A的所有成员函数都可访问类B的私有成员。
*    - 语法：在类B内部用`friend class 类A;`声明。
*    - 特点：友元关系单向（A是B的友元≠B是A的友元）、不可传递（A是B的友元，B是C的友元≠A是C的友元）。
*    - 适用场景：两个类关系紧密（如容器类与迭代器类），需要相互访问内部实现。
*
* 3. 友元成员函数（Friend Member Function）
*    - 定义：类A的某个特定成员函数被类B声明为友元，只有该成员函数可访问类B的私有成员。
*    - 语法：需提前声明类A，在类B中用`friend 返回值类型 类A::函数名(参数列表);`声明，函数定义在类A中。
*    - 特点：比友元类更精确（仅开放特定函数的访问权限），减少封装性破坏。
*    - 适用场景：仅需类A的某个函数访问类B的私有成员，其他函数无需访问的场景。
*
* 友元的优缺点：
* - 优点：简化跨类访问逻辑，避免冗余的public接口，提高代码效率。
* - 缺点：破坏类的封装性（私有成员被外部访问），增加类间耦合度，降低代码可维护性。
* - 原则：最小化友元范围（能用友元成员函数就不用友元类，能用友元函数就不用成员函数），仅在必要时使用。
*******************************************************************************************************************/

// 全局类声明与定义（避免局部类导致的编译错误）

// 示例1：友元函数相关类
class Point {
private:
    int x;
    int y;
public:
    Point(int x_, int y_) : x(x_), y(y_) {}
    // 声明友元函数（全局函数）
    friend void printPoint(const Point& p);
};
// 全局友元函数定义（在类外部）
void printPoint(const Point& p);

// 示例2：友元类相关类
class Student2 {
private:
    std::string name;
    int score;
    friend class Teacher2; // 声明友元类
public:
    Student2(std::string n, int s) : name(n), score(s) {}
};
class Teacher2 {
public:
    void adjustScore(Student2& s, int newScore);
    void printStudentScore(const Student2& s);
};

// 示例3：友元成员函数相关类
class Student3; // 提前声明
class Teacher3 {
public:
    void adjustScore(Student3& s, int newScore);
    void printStudentScore(const Student3& s);
};
class Student3 {
private:
    std::string name;
    int score;
    friend void Teacher3::adjustScore(Student3& s, int newScore); // 友元成员函数
public:
    Student3(std::string n, int s) : name(n), score(s) {}
    std::string getName() const { return name; }
    int getScore() const { return score; }
};

// 示例4：友元运算符重载相关类
class Student4 {
private:
    std::string name;
    int age;
public:
    Student4(std::string n, int a) : name(n), age(a) {}
    friend std::ostream& operator<<(std::ostream& os, const Student4& s); // 友元运算符
};
// 提前声明友元运算符
std::ostream& operator<<(std::ostream& os, const Student4& s);

// 示例1：友元函数
void friend_function_demo() {
    std::cout << "----- 示例1：友元函数 -----" << std::endl;

    Point pt(10, 20);
    printPoint(pt); // 调用全局友元函数
    std::cout << std::endl;
}
// 实现全局友元函数
void printPoint(const Point& p) {
    std::cout << "Point(" << p.x << ", " << p.y << ")" << std::endl;
}

// 示例2：友元类
void friend_class_demo() {
    std::cout << "----- 示例2：友元类 -----" << std::endl;

    Student2 stu("John", 85);
    Teacher2 t;
    t.printStudentScore(stu);
    t.adjustScore(stu, 95);
    t.printStudentScore(stu);
    std::cout << std::endl;
}
// 实现Teacher2的成员函数
void Teacher2::adjustScore(Student2& s, int newScore) {
    s.score = newScore;
}
void Teacher2::printStudentScore(const Student2& s) {
    std::cout << s.name << "'s score: " << s.score << std::endl;
}

// 示例3：友元成员函数
void friend_member_function_demo() {
    std::cout << "----- 示例3：友元成员函数 -----" << std::endl;

    Student3 stu("Jane", 75);
    Teacher3 t;
    t.printStudentScore(stu);
    t.adjustScore(stu, 88);
    t.printStudentScore(stu);
    std::cout << std::endl;
}
// 实现Teacher3的成员函数
void Teacher3::adjustScore(Student3& s, int newScore) {
    s.score = newScore; // 友元成员函数访问私有成员
}
void Teacher3::printStudentScore(const Student3& s) {
    std::cout << s.getName() << "'s score: " << s.getScore() << std::endl;
}

// 示例4：友元运算符重载
void friend_operator_overload_demo() {
    std::cout << "----- 示例4：友元运算符重载（<<） -----" << std::endl;

    Student4 s("Alice", 20);
    std::cout << s << std::endl;
    Student4 s2("Bob", 22);
    std::cout << s << " 和 " << s2 << std::endl;
}
// 实现友元运算符
std::ostream& operator<<(std::ostream& os, const Student4& s) {
    os << "Name: " << s.name << ", Age: " << s.age;
    return os;
}
#if 0
int main() {
    friend_function_demo();
    friend_class_demo();
    friend_member_function_demo();
    friend_operator_overload_demo();
    return 0;
}
#endif