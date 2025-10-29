#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
/*******************************************************************************************************************
* C++ 拷贝构造函数、赋值运算符重载与const成员函数
* 本次内容聚焦类的三大核心机制：const成员函数（控制对象访问权限）、拷贝构造函数（对象拷贝初始化）、赋值运算符重载（对象赋值），
* 同时结合对象生命周期（创建、拷贝、析构），解析函数参数传递、返回值处理中的对象行为，避免拷贝冗余或资源错误。
*
* 一、const成员函数与const对象
* 1. 核心定义：
*    - const成员函数：在成员函数参数列表后加const（如void Print() const），其隐含的this指针类型为「const 类名* const」，
*      表示函数不能修改对象的成员变量（保证对象只读）。
*    - const对象：用const修饰的对象（如const Int b{30}），其this指针为「const 类名* const」，只能调用const成员函数，
*      禁止调用非const成员函数（避免对象被意外修改）。
* 2. 关键规则：
*    - 非const对象可调用const和非const成员函数；
*    - const对象只能调用const成员函数；
*    - const成员函数内不能修改成员变量，也不能调用非const成员函数。
*
* 二、拷贝构造函数（Copy Constructor）
* 1. 核心定义：
*    拷贝构造函数是特殊的构造函数，用于「用已存在的对象初始化新对象」，语法为：
*    类名(const 类名& 源对象) : 成员变量(源对象.成员变量) { ... }
*    （参数必须为「const 类名&」，避免值传递引发无限递归拷贝，同时支持接收const对象）。
* 2. 调用场景：
*    - 用已有对象初始化新对象（如Int c(b)、Int c = b）；
*    - 对象作为函数参数「值传递」时（如void func(Int it)，调用func(a)会拷贝a创建形参it）；
*    - 函数返回值为对象时（非NRVO优化下，会拷贝局部对象创建临时返回值）。
* 3. 编译器默认行为：
*    若未手动定义拷贝构造函数，编译器会自动生成「默认拷贝构造函数」，实现「浅拷贝」（逐成员赋值，适合无动态资源的类）；
*    若类包含动态资源（如new分配的内存），需手动定义拷贝构造函数实现「深拷贝」，避免资源重复释放。
*
* 三、赋值运算符重载（Copy Assignment Operator）
* 1. 核心定义：
*    赋值运算符重载用于「已存在的对象之间赋值」，语法为：
*    类名& operator=(const 类名& 源对象) {
*        if (this != &源对象) { // 关键：判断自赋值，避免重复操作或资源错误
*            成员变量 = 源对象.成员变量; // 浅拷贝/深拷贝逻辑
*        }
*        return *this; // 返回自身引用，支持链式赋值（如a = b = c）
*    }
* 2. 与拷贝构造的区别：
*    - 拷贝构造：用于「初始化新对象」（对象不存在时），调用时机是对象创建时；
*    - 赋值重载：用于「已有对象赋值」（对象已存在时），调用时机是赋值语句（=）时。
* 3. 编译器默认行为：
*    若未手动定义，编译器生成默认赋值运算符，实现浅拷贝；含动态资源时需手动实现深拷贝，避免内存泄漏。
*
* 四、对象生命周期与拷贝/析构流程
* 当对象涉及函数参数传递、返回值时，会触发拷贝构造与析构的连锁反应，典型流程：
* 1. 函数参数值传递：调用拷贝构造创建形参 → 函数执行 → 函数结束形参析构；
* 2. 函数返回对象：创建局部对象 → 调用拷贝构造创建临时返回值 → 局部对象析构 → 临时值赋值给目标对象（若有）→ 临时值析构。
* （注：现代编译器会通过NRVO（命名返回值优化）省略部分拷贝，减少冗余，实际输出可能与理论流程略有差异，但逻辑一致）
*******************************************************************************************************************/

// 1. 场景1：const对象与const成员函数（控制对象访问权限）
class Int {
private:
    int value;
public:
    // 构造函数
    Int() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // const成员函数：不能修改成员变量，可被const对象调用
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
    int GetValue() const {
        return value;
    }

    // 非const成员函数：可修改成员变量，不能被const对象调用
    void SetValue(int v) {
        value = v;
    }
};

void const_object_member_demo() {
    std::cout << "----- const对象与const成员函数 -----" << std::endl;
    // 非const对象：可调用const和非const成员函数
    Int a{ 10 };
    a.SetValue(20); // 调用非const成员函数（合法）
    std::cout << "a.GetValue() = " << a.GetValue() << std::endl; // 调用const成员函数（合法）
    a.Print(); // 调用const成员函数（合法）

    // const对象：只能调用const成员函数
    const Int b{ 30 };
    std::cout << "b.GetValue() = " << b.GetValue() << std::endl; // 调用const成员函数（合法）
    b.Print(); // 调用const成员函数（合法）
    // b.SetValue(100); // 错误：const对象不能调用非const成员函数

    std::cout << "\n" << std::endl;
}

// 2. 场景2：拷贝构造函数（对象拷贝初始化与函数参数值传递）
class Int2 {
private:
    int value;
public:
    // 构造函数
    Int2() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int2(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // 手动定义拷贝构造函数（参数为const Int&，避免递归拷贝）
    Int2(const Int2& it) : value(it.value) {
        std::cout << "Copy create Int (value=" << value << ") at " << this
            << " (from " << &it << ")" << std::endl;
    }

    // 析构函数：观察对象销毁时机
    ~Int2() {
        std::cout << "Destroy Int (value=" << value << ") at " << this << std::endl;
    }

    // const成员函数：打印值
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
};
void copy_constructor_demo1() {
    std::cout << "----- 拷贝构造函数（对象初始化与函数参数） -----" << std::endl;
    // 场景1：用已有对象初始化新对象（调用拷贝构造）
    Int2 a{ 10 };
    Int2 b(a); // 拷贝a初始化b → 调用拷贝构造
    Int2 c = a; // 等价于Int c(a) → 调用拷贝构造（初始化，非赋值）
    std::cout << "a.Print(): "; a.Print();
    std::cout << "b.Print(): "; b.Print();
    std::cout << "c.Print(): "; c.Print();

    // 场景2：对象作为函数参数值传递（调用拷贝构造创建形参）
    auto func = [](Int2 it) { // 形参it是值传递，需拷贝实参创建
        std::cout << "In func: ";
        it.Print();
        }; // 函数结束后，形参it析构

    std::cout << "\nCall func(a):" << std::endl;
    func(a); // 拷贝a创建形参it → 调用拷贝构造；函数结束it析构

    std::cout << "\nMain end (a, b, c will destroy):" << std::endl;
    // main函数结束，a、b、c依次析构
    std::cout << "\n" << std::endl;
}

// 3. 场景3：拷贝构造与赋值运算符的区别（初始化vs赋值）
class Int3 {
private:
    int value;
public:
    // 构造函数
    Int3() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int3(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // 拷贝构造函数（初始化新对象）
    Int3(const Int3& it) : value(it.value) {
        std::cout << "Copy create Int (value=" << value << ") at " << this
            << " (from " << &it << ")" << std::endl;
    }

    // 赋值运算符重载（已有对象赋值）
    Int3& operator=(const Int3& it) {
        if (this != &it) { // 关键：判断自赋值（避免a = a的错误）
            value = it.value;
            std::cout << "Assign: " << this << " = " << &it
                << " (new value=" << value << ")" << std::endl;
        }
        else {
            std::cout << "Self-assignment skipped (this == &it)" << std::endl;
        }
        return *this; // 返回自身引用，支持链式赋值
    }

    // 析构函数
    ~Int3() {
        std::cout << "Destroy Int (value=" << value << ") at " << this << std::endl;
    }

    // 打印值
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
};
void copy_vs_assignment_demo() {
    std::cout << "----- 拷贝构造 vs 赋值运算符重载 -----" << std::endl;

    

    // 1. 拷贝构造：初始化新对象（对象不存在）
    Int3 a{ 10 };
    Int3 b = a; // 初始化b → 调用拷贝构造（非赋值）
    std::cout << "After Int b = a (copy constructor):" << std::endl;
    std::cout << "a.Print(): "; a.Print();
    std::cout << "b.Print(): "; b.Print();

    // 2. 赋值重载：已有对象赋值（对象已存在）
    Int3 c{ 20 };
    c = a; // c已存在，赋值 → 调用赋值运算符
    std::cout << "\nAfter c = a (assignment operator):" << std::endl;
    std::cout << "c.Print(): "; c.Print();

    // 3. 链式赋值（依赖赋值重载返回自身引用）
    Int3 d{ 30 };
    d = c = a; // 等价于d.operator=(c.operator=(a)) → 链式赋值
    std::cout << "\nAfter d = c = a (chained assignment):" << std::endl;
    std::cout << "d.Print(): "; d.Print();

    // 4. 自赋值（触发自赋值判断，跳过操作）
    std::cout << "\nCall a = a (self-assignment):" << std::endl;
    a = a;

    std::cout << "\nMain end (a, b, c, d will destroy):" << std::endl;
    std::cout << "\n" << std::endl;
}

// 4. 场景4：函数返回对象的拷贝与析构（含临时对象）
class Int4 {
private:
    int value;
public:
    // 构造函数
    Int4() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int4(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // 拷贝构造函数
    Int4(const Int4& it) : value(it.value) {
        std::cout << "Copy create Int (value=" << value << ") at " << this
            << " (from " << &it << ")" << std::endl;
    }

    // 赋值运算符重载
    Int4& operator=(const Int4& it) {
        if (this != &it) {
            value = it.value;
            std::cout << "Assign: " << this << " = " << &it
                << " (new value=" << value << ")" << std::endl;
        }
        return *this;
    }

    // 析构函数
    ~Int4() {
        std::cout << "Destroy Int (value=" << value << ") at " << this << std::endl;
    }

    // 打印值
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
};
void return_object_copy_demo() {
    std::cout << "----- 函数返回对象的拷贝与析构 -----" << std::endl;
    // 函数返回值为对象（非NRVO优化下的流程）
    //Int4 func(Int4 it) {
    //    std::cout << "In func: modify it's value" << std::endl;
    //    // 注意：此处若要修改it，需去掉形参的const（本例it为非const，可修改）
    //    // （实际开发中，函数参数若无需修改，建议加const：const Int& it）
    //    // 为演示拷贝，此处不修改，直接返回
    //    std::cout << "In func: return it (create temp return value)" << std::endl;
    //    return it; // 拷贝it创建临时返回值 → 调用拷贝构造；函数结束it析构
    //}

    // 测试1：函数返回值直接初始化新对象（可能触发NRVO优化，省略临时值拷贝）
    std::cout << "\nTest 1: Int e = func(a):" << std::endl;
    Int4 a{ 10 };
    //Int4 e = func(a); // 用返回值初始化e → 可能省略临时值，直接拷贝到e
    //std::cout << "e.Print(): "; e.Print();

    // 测试2：函数返回值赋值给已有对象（临时值先创建，再赋值，最后临时值析构）
    std::cout << "\nTest 2: Int f{20}; f = func(a):" << std::endl;
    Int4 f{ 20 };
    //f = func(a); // 1. 拷贝a创建形参it；2. 拷贝it创建临时返回值；3. 临时值赋值给f；4. it和临时值析构
    std::cout << "f.Print(): "; f.Print();

    std::cout << "\nMain end (a, e, f will destroy):" << std::endl;
}

#if 0
int main() {
    const_object_member_demo();    // const对象与const成员函数
    copy_constructor_demo1();      // 拷贝构造（初始化与函数参数）
    copy_vs_assignment_demo();     // 拷贝构造 vs 赋值重载
    return_object_copy_demo();     // 函数返回对象的拷贝与析构
    return 0;
}
#endif