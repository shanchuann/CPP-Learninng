#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>

/*******************************************************************************************************************
* C++ 类继承核心知识点总结
* 继承是C++面向对象的核心特性，允许派生类复用基类的成员变量和成员函数，实现代码复用与层次化设计。
* 核心内容包括：继承基本语法、构造析构顺序、访问限定符、赋值兼容规则、同名隐藏及解除方式、派生类初始化。
*
* 一、继承的基本概念与语法
* 1. 定义：派生类通过 `: public 基类名` 继承基类，自动拥有基类的非私有成员；
* 2. 语法：class 派生类 : 继承方式 基类 { 派生类新增成员 };
* 3. 核心作用：复用基类代码，扩展新功能，是多态的基础。
*
* 二、构造函数与析构函数的调用顺序
* 1. 构造顺序：**先调用基类构造函数 → 再调用派生类构造函数**；
* 2. 析构顺序：**先调用派生类析构函数 → 再调用基类析构函数**（与构造顺序相反）；
* 3. 规则：基类构造函数自动调用，无需手动声明。
*
* 三、继承的访问限定符（三种继承方式）
* 1. 公有继承(public)：基类public/protected权限不变，private不可访问；
* 2. 保护继承(protected)：基类public变为protected，protected不变，private不可访问；
* 3. 私有继承(private)：基类public/protected均变为private，private不可访问；
* 4. 通用规则：基类**私有成员**无论何种继承方式，派生类都无法直接访问。
*
* 四、赋值兼容规则（公有继承专属）
* 派生类对象可安全转换为基类类型（向上转型），包含三种场景：
* 1. 派生类对象 → 赋值给基类对象；
* 2. 派生类对象地址 → 赋值给基类指针；
* 3. 派生类对象 → 初始化基类引用；
* 4. 限制：基类指针/引用只能访问基类成员，无法访问派生类新增成员。
*
* 五、同名隐藏机制
* 1. 定义：派生类定义与基类**同名的成员变量/函数**时，基类同名成员会被自动隐藏；
* 2. 默认行为：直接调用时优先使用派生类成员，基类成员无法直接访问；
* 3. 强制访问：通过 `基类名::成员名` 显式调用基类同名成员。
*
* 六、解除同名隐藏的两种方式
* 1. using声明：在派生类中写 `using 基类名::成员名`，导入基类所有同名重载函数；
* 2. 虚函数：基类用 `virtual` 声明函数，派生类重写(override)，实现多态，基类指针可调用派生类重写函数。
*
* 七、派生类构造函数初始化规则
* 1. 必须通过初始化列表 `: 基类名(参数)` 调用基类构造函数；
* 2. 先初始化基类成员，再初始化派生类新增成员。
*******************************************************************************************************************/

// ===================== 1. 构造函数和析构函数的调用顺序 =====================
class Base1 {
public:
	Base1() { std::cout << "Base 构造函数" << std::endl; }
	~Base1() { std::cout << "Base 析构函数" << std::endl; }
};

class Derived1 : public Base1 {
public:
	Derived1() { std::cout << "Derived 构造函数" << std::endl; }
	~Derived1() { std::cout << "Derived 析构函数" << std::endl; }
};

void testConstructorDestructorOrder() {
	std::cout << "=== 构造/析构调用顺序 ===" << std::endl;
	Derived1 d;
	std::cout << std::endl;
}

// ===================== 2. 赋值兼容规则 =====================
class Base2 {
protected:
	int base_val;
public:
	Base2(int val) : base_val(val) {}

	void showBase() {
		std::cout << "基类成员变量值：" << base_val << std::endl;
	}
};

class Derived2 : public Base2 {
private:
	int derived_val;
public:
	Derived2(int b_val, int d_val) : Base2(b_val), derived_val(d_val) {}

	void showDerived() {
		std::cout << "派生类新增成员变量值：" << derived_val << std::endl;
	}
};

void testAssignmentCompatibility() {
	std::cout << "=== 赋值兼容规则 ===" << std::endl;
	Derived2 d(10, 20);
	std::cout << "原始派生类对象" << std::endl;
	d.showBase();
	d.showDerived();
	std::cout << std::endl;

	// 规则1：派生类对象赋值给基类对象
	Base2 b = d;
	std::cout << "规则1：派生类对象赋值给基类对象" << std::endl;
	b.showBase();
	std::cout << std::endl;

	// 规则2：派生类对象地址赋给基类指针
	Base2* pBase = &d;
	std::cout << "规则2：基类指针指向派生类对象地址" << std::endl;
	pBase->showBase();
	std::cout << std::endl;

	// 规则3：派生类对象初始化基类引用
	Base2& refBase = d;
	std::cout << "规则3：基类引用绑定派生类对象" << std::endl;
	refBase.showBase();
	std::cout << std::endl;
}

// ===================== 3. 同名隐藏 =====================
class Base3 {
public:
	int var = 10;
	void show() { std::cout << "Base show,var = " << var << std::endl; }
};

class Derived3 : public Base3 {
public:
	int var = 20;
	void show() { std::cout << "Derived show,var = " << var << std::endl; }
};

void testNameHiding() {
	std::cout << "=== 同名隐藏 ===" << std::endl;
	Derived3 d;
	d.show();
	std::cout << "Derived var = " << d.var << std::endl;

	// 访问基类的成员
	d.Base3::show();
	std::cout << "Base var = " << d.Base3::var << std::endl;
	std::cout << std::endl;
}

// ===================== 4. using 声明解除同名隐藏 =====================
class Base4 {
public:
	void show() {
		std::cout << "基类：无参 show()" << std::endl;
	}
	void show(int a) {
		std::cout << "基类：带参 show(" << a << ")" << std::endl;
	}
};

class Derived4 : public Base4 {
public:
	// 导入基类所有show函数，解除隐藏
	using Base4::show;

	void show(std::string s) {
		std::cout << "派生类：带参 show(\"" << s << "\")" << std::endl;
	}
};

void testUsingReleaseHiding() {
	std::cout << "=== using 解除同名隐藏 ===" << std::endl;
	Derived4 d;
	d.show();
	d.show(10);
	d.show("测试");
	std::cout << std::endl;
}

// ===================== 5. 虚函数解除同名隐藏(多态) =====================
class Base5 {
public:
	virtual void show() {
		std::cout << "基类虚函数：show()" << std::endl;
	}
};

class Derived5 : public Base5 {
public:
	void show() override {
		std::cout << "派生类：show()" << std::endl;
	}
};

void testVirtualFunction() {
	std::cout << "=== 虚函数解除同名隐藏(多态) ===" << std::endl;
	Derived5 d;
	d.show();
	d.Base5::show();

	Base5* ptr = new Derived5();
	ptr->show();
	delete ptr;
	std::cout << std::endl;
}

// ===================== 6. 继承访问限定符 =====================
class Base6 {
public:
	int public_var;
protected:
	int protected_var;
private:
	int private_var;
};

// 公有继承
class Derived_Public : public Base6 {};
// 保护继承
class Derived_Protected : protected Base6 {};
// 私有继承
class Derived_Private : private Base6 {};

void testAccessModifier() {
	std::cout << "=== 继承访问限定符 ===" << std::endl;
	std::cout << "1. 公有继承(public): 基类public/protected权限不变" << std::endl;
	std::cout << "2. 保护继承(protected): 基类public变为protected" << std::endl;
	std::cout << "3. 私有继承(private): 基类public/protected变为private" << std::endl;
	std::cout << "基类private成员，任何继承方式都无法直接访问" << std::endl;
	std::cout << std::endl;
}

// ===================== 7. 基本继承示例 =====================
class Person {
public:
	std::string name;
	int age;
	Person(std::string name, int age) : name(name), age(age) {}
	void display() {
		std::cout << "Name: " << name << ", Age: " << age << std::endl;
	}
};

class Student : public Person {
public:
	std::string school;
	Student(std::string name, int age, std::string school)
		: Person(name, age), school(school) {
	}
	void display() {
		Person::display();
		std::cout << "School: " << school << std::endl;
	}
};

void testBasicInheritance() {
	std::cout << "=== 基本继承示例 ===" << std::endl;
	Student student("Shanchuan", 20, "XAUFE");
	student.display();
	std::cout << std::endl;
}

#if 0
int main() {
	testConstructorDestructorOrder();   // 构造析构顺序
	testAssignmentCompatibility();      // 赋值兼容规则
	testNameHiding();                   // 同名隐藏
	testUsingReleaseHiding();           // using解除隐藏
	testVirtualFunction();              // 虚函数多态
	testAccessModifier();               // 访问限定符
	testBasicInheritance();             // 基本继承

	return 0;
}
#endif