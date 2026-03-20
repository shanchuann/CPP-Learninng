#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdint>
#include<cassert>
#include<typeinfo>
#include<string>

/*******************************************************************************************************************
* C++ 四种类型转换核心知识点总结
* C++提供了四种语义明确的类型转换运算符，替代C语言模糊的强制转换（(Type)expr），从**安全到不安全**、**编译期到运行期**依次为：static_cast、const_cast、dynamic_cast、reinterpret_cast。
* 核心内容包括：每种转换的定义、语法、特点、适用场景、代码示例覆盖的具体用法。
*
* 一、static_cast（编译期、最常用、相对安全）
* 1. 核心定义：编译期进行的“语义合理”的类型转换，不进行运行时类型检查；
* 2. 语法：`static_cast<目标类型>(表达式)`；
* 3. 特点：
*    - 仅支持“语义上有逻辑关联”的类型转换；
*    - 编译期完成，无运行时开销；
*    - 不修改const/volatile属性；
* 4. 适用场景：
*    - 数值类型转换（如double→int、int→char、int→枚举）；
*    - void*与其他指针类型的双向转换；
*    - 继承体系中的**上行转换**（派生类指针/引用→基类指针/引用，安全）；
*    - 左值转右值引用（触发移动语义）。
*
* 二、const_cast（编译期、仅修改const/volatile属性）
* 1. 核心定义：编译期进行的“仅修改类型的const/volatile属性”的转换，不改变类型本身；
* 2. 语法：`const_cast<目标类型>(表达式)`；
* 3. 特点：
*    - 仅能修改const/volatile属性，不能进行其他类型转换；
*    - 编译期完成，无运行时开销；
* 4. 适用场景：
*    - 调用第三方库函数时，函数参数为非const引用/指针，但实际不会修改值；
*    - **安全修改前提**：原对象本身不是const，仅被const引用/指针指向；
* 5. 禁止场景：原对象本身是const，去掉const后修改会触发**未定义行为**。
*
* 三、dynamic_cast（运行期、仅用于多态类、安全向下转型）
* 1. 核心定义：运行期进行的“多态类继承体系中的安全向下转型”，会检查类型是否匹配；
* 2. 语法：
*    - 指针转换：`dynamic_cast<目标指针类型>(表达式)`，失败返回`nullptr`；
*    - 引用转换：`dynamic_cast<目标引用类型>(表达式)`，失败抛出`std::bad_cast`异常；
* 3. 特点：
*    - 仅适用于**包含至少一个虚函数的多态类**；
*    - 运行期完成，有少量类型检查开销；
*    - 支持继承体系中的**双向转换**（上行转换自动安全，下行转换需检查）；
* 4. 适用场景：
*    - 多态类继承体系中的**安全向下转型**（基类指针/引用→派生类指针/引用）；
*    - 替代C语言模糊的强制转换，避免未定义行为。
*
* 四、reinterpret_cast（编译期、最不安全、底层内存重解释）
* 1. 核心定义：编译期进行的“底层内存位模式的直接重解释”，不进行任何语义检查；
* 2. 语法：`reinterpret_cast<目标类型>(表达式)`；
* 3. 特点：
*    - 支持“任意指针/整数/引用类型”的转换，无任何语义限制；
*    - 编译期完成，无运行时开销；
*    - 极其危险，转换结果依赖编译器实现，极易触发未定义行为；
* 4. 适用场景：
*    - 底层硬件编程（如直接访问内存地址）；
*    - 指针与整数的双向转换（需用`uintptr_t`/`intptr_t`存储地址）；
*    - 极少数特殊场景（如对象起始地址与第一个成员地址一致时的直接访问）；
* 5. 原则：非必要不使用，使用时必须明确知道底层内存布局。
*******************************************************************************************************************/

// 全局定义
// 1. static_cast用的枚举
enum WeekType { Sun = 0, Mon = 1, Tues = 2, Wed = 3, Thurs = 4, Fir = 5, Sat = 6 };

// 2. reinterpret_cast用的Object类
class Object {
    int value; // 对象的第一个成员，地址与对象起始地址一致
public:
    Object(int x = 0) : value(x) {}
    void setValue(int x) { value = x; }
    int getValue() const { return value; }
    void printValue() const { std::cout << "value: " << value << std::endl; }
};

// 3. static_cast/dynamic_cast用的Base/Derived类
class Base {
public:
    virtual void show() const { std::cout << "Base::show" << std::endl; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void show() const override { std::cout << "Derived::show" << std::endl; }
};

// 4. dynamic_cast用的Person/Student/Employee类
class Person {
    std::string p_name;
public:
    Person(const std::string& name) : p_name(name) {}
    virtual ~Person() = default;
    virtual void drop() const { std::cout << "Person: " << p_name << std::endl; }
};

class Student : public Person {
    std::string s_id;
public:
    Student(const std::string& name, const std::string& id)
        : Person(name), s_id(id) {
    }
    void drop() const override { std::cout << "Student s_id: " << s_id << std::endl; }
};

class Employee : public Person {
    std::string e_id;
public:
    Employee(const std::string& name, const std::string& id)
        : Person(name), e_id(id) {
    }
    void drop() const override { std::cout << "Employee e_id: " << e_id << std::endl; }
};

// 5. const_cast用的第三方库模拟函数
void func(int& num) {
    std::cout << "func内的数值：" << num << std::endl;
}

// 6. dynamic_cast对比用的不安全C风格转换函数
void unsafeFunc(Person* p) {
    assert(p != nullptr);
    p->drop();
    Student* sp = nullptr;
    Employee* ep = nullptr;
    // C语言强制转换：无论p实际指向什么类型，都能转换成功
    sp = (Student*)p;
    ep = (Employee*)p;
    sp->drop(); // 类型不匹配时，触发未定义行为
    ep->drop();
}

// 7. dynamic_cast对比用的安全转换函数
void safeFunc(Person* p) {
    assert(p != nullptr);
    p->drop();
    Student* sp = nullptr;
    Employee* ep = nullptr;
    // dynamic_cast：运行时检查类型，匹配才转换成功，不匹配返回nullptr
    sp = dynamic_cast<Student*>(p);
    ep = dynamic_cast<Employee*>(p);
    // 判空后再使用，完全避免未定义行为
    if (sp != nullptr) {
        std::cout << "转换为Student成功：";
        sp->drop();
    }
    else {
        std::cout << "转换为Student失败，类型不匹配" << std::endl;
    }
    if (ep != nullptr) {
        std::cout << "转换为Employee成功：";
        ep->drop();
    }
    else {
        std::cout << "转换为Employee失败，类型不匹配" << std::endl;
    }
}

void testStaticCast() {
    std::cout << "\n测试 static_cast" << std::endl;

    // 1. int转枚举类型
    int num = 2;
    WeekType week = static_cast<WeekType>(num);
    std::cout << "转换后的枚举值：" << week << std::endl;

    // 2. 数值类型转换：double转int
    double pi = 3.14159;
    int intPi = static_cast<int>(pi);
    std::cout << "double转int结果：" << intPi << std::endl;

    // 3. int转char
    int ascii = 97;
    char ch = static_cast<char>(ascii);
    std::cout << "int转char结果：" << ch << std::endl;

    // 4. void*与其他指针类型的双向转换
    int a = 10;
    void* voidPtr = static_cast<void*>(&a);
    int* intPtr = static_cast<int*>(voidPtr);
    std::cout << "解引用结果：" << *intPtr << std::endl;

    // 5. 继承体系中的上行转换
    Derived d;
    Base* basePtr = static_cast<Base*>(&d);
    basePtr->show();

    // 6. 左值转右值引用（触发移动语义）
    std::string str = "hello world";
    std::string movedStr = static_cast<std::string&&>(str);
    std::cout << "移动后的字符串：" << movedStr << std::endl;
}

void testReinterpretCast() {
    std::cout << "\n测试 reinterpret_cast" << std::endl;

    // 1. 对象起始地址与第一个成员地址一致时的直接访问
    Object obj(10);
    obj.printValue();
    int* ip = reinterpret_cast<int*>(&obj);
    *ip = 100;
    obj.printValue();

    // 2. 指针与整数的双向转换
    int a = 10;
    int* ptr = &a;
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    std::cout << "指针的地址值：" << std::hex << addr << std::endl;
    int* newPtr = reinterpret_cast<int*>(addr);
    std::cout << "解引用结果：" << std::dec << *newPtr << std::endl;
}

void testConstCast() {
    std::cout << "\n测试 const_cast" << std::endl;

    // 1. 安全场景：原对象不是const，仅被const引用指向
    int a = 10;
    const int& constRef = a;
    func(const_cast<int&>(constRef));
    int* ptr = const_cast<int*>(&constRef);
    *ptr = 20;
    std::cout << "修改后的a：" << a << std::endl;

    // 2. 禁止场景：原对象本身是const（注释掉避免未定义行为）
    // const int constA = 10;
    // int* badPtr = const_cast<int*>(&constA);
    // *badPtr = 20;
    // std::cout << "constA的值：" << constA << std::endl;
    // std::cout << "badPtr解引用的值：" << *badPtr << std::endl;
}

void testDynamicCast() {
    std::cout << "\n测试 dynamic_cast" << std::endl;

    // 1. 测试Student对象
    std::cout << "测试Student对象" << std::endl;
    Student stud("yhping", "24001");
    safeFunc(&stud);

    // 2. 测试Employee对象
    std::cout << "\n测试Employee对象" << std::endl;
    Employee emp("zhangsan", "E1001");
    safeFunc(&emp);

    // 3. 测试引用类型转换（失败抛异常）
    std::cout << "\n测试引用类型转换" << std::endl;
    Person& personRef = stud;
    try {
        Student& stuRef = dynamic_cast<Student&>(personRef);
        std::cout << "引用转换为Student成功" << std::endl;
        Employee& empRef = dynamic_cast<Employee&>(personRef); // 类型不匹配，抛异常
    }
    catch (const std::bad_cast& e) {
        std::cout << "引用转换失败，捕获异常：" << e.what() << std::endl;
    }

    // 4. 对比不安全的C风格转换（注释掉避免未定义行为）
    // std::cout << "\n对比不安全的C风格转换" << std::endl;
    // unsafeFunc(&emp);
}
#if 0
int main() {
    testStaticCast();
    testReinterpretCast();
    testConstCast();
    testDynamicCast();

    return 0;
}
#endif