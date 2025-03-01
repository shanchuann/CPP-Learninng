#include<iostream>
#include<vector>
/********************************************************************************************************************
*C++初始化
* 
*1.初始化是指为变量赋予初始值。在C++中，初始化有多种方式，包括复制初始化和直接初始化。
*	1.1. 复制初始化（copy initialization）：使用等号=进行初始化
*	1.2. 直接初始化（direct initialization）：使用圆括号()或花括号{}进行初始化
*
*2.初始化和赋值:初始化的等号和赋值的等号含义不同
*	2.1.初始化：为变量申请存储空间，创建新的变量。如果是类类型，将调用类的构造函数
*	2.2.赋值：把一个现有变量的值用另一个值替代，不创建新的变量。如果是类类型，将调用类的赋值运算符 operator=()
*	e.g.int a = 1; // 初始化 
*		a = 2;     // 赋值
*		ClassType obj1;			 // 初始化,调用构造函数
* 		ClassType obj2("Hello"); // 初始化,调用构造函数
* 		obj1 = obj2;			 // 赋值
* 
*3.列表初始化（list initialization）：使用花括号{}进行初始化
*	C++ 初始化形式中的 (2)(4) 都属于列表初始化，在 C++11 中得到全面应用
*	其最大的特点
* 	3.1.统一语法：使用花括号{}进行初始化，适用于几乎所有类型的初始化。
*	3.2.自动避免窄化转换：在列表初始化中，如果初始化过程会导致数据丢失（如，从 double 到 int 的转换），编译器会报错。
*		long double num = 3.1415;
*		int a{num};    // 无法编译，转换存在信息丢失的风险
*		int b = {num}; // 圆括号则不会报错，但信息丢失，发出警告
*	3.3.适用于所有类型：包括 POD（Plain Old Data，可以被直接使用 memcpy 进行复制的对象）类型、聚合类型、类类型等。
********************************************************************************************************************/
#if 0
int main()
{
	int a = 1;		//（1） 复制初始化（copy initialization）
	int b = { 2 };  //（2） 复制初始化（copy initialization）
	int c(3);		//（3） 直接初始化（direct initialization）
	int d{ 4 };		//（4） 直接初始化（direct initialization）

	int arr_copy[5] = { 1,2,3,4,5 };	//复制初始化（copy initialization）
	int arr_direct1[5]{ 1,2,3 }; 		//直接初始化（direct initialization）
	int arr_direct2[5]{ 1,2,3,4,5 };	//直接初始化（direct initialization）

	long double num = 3.1415;
	//int a1{ num };     错误	C2397	从“long double”转换到“int”需要收缩转换	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	41
	//int a2 = { num };  错误	C2397	从“long double”转换到“int”需要收缩转换	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	42
	int b1 = num;	  // 警告	C4244	“初始化” : 从“long double”转换到“int”，可能丢失数据	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	43
	int b2(num);	  // 警告	C4244	“初始化” : 从“long double”转换到“int”，可能丢失数据	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	44

	//聚合类型初始化：聚合类型是指没有用户自定义的构造函数、析构函数、拷贝构造函数和赋值运算符的类，包括数组、结构体等。
	struct A
	{
		int a;
		int b;
	};
	A a1 = { 1,2 };	//复制初始化, C++11引入的列表初始化
	A a2{ 3,4 };	//直接初始化,,比上一行初始化的方式更加通用

	//类类型初始化
	class MyClass {
        public:
            MyClass(int x, double y) : a(x), b(y) {}
            int a;
            double b;
    };

	MyClass obj1{ 5, 3.14 };				 // 列表初始化
	std::string str{ "Hello, C plusplus!" }; // 列表初始化

	//容器和数组初始化
	std::vector<int> vec{ 1,2,3,4,5 }; // 列表初始化
	int arr[]{ 1,2,3,4,5 };			   // 数组列表初始化

	//输出结果
	std::cout << "Result print" << std::endl;
	std::cout << std::endl;
	std::cout << "a: " << a << " ";
	std::cout << "b: " << b << " ";
	std::cout << "c: " << c << " ";
	std::cout << "d: " << d << std::endl;

	std::cout << "Array print arr" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << arr_copy[i] <<" ";
	}
	std::cout << arr_copy[4] << std::endl;
	std::cout << "Array print arr2" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << arr_direct1[i] <<" ";
	}
	std::cout << arr_direct1[4] << std::endl;
	std::cout << "Array print arr3" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << arr_direct2[i] <<" ";
	}
	std::cout << arr_direct2[4] << std::endl;

	std::cout << "避免窄化转换" << std::endl;
	//std::cout << "a1: " << a1 << " ";
	//std::cout << "a2: " << a2 << std::endl;
	std::cout << "a1, a2 无法编译" << std::endl;
	std::cout << "b1: " << b1 << " ";
	std::cout << "b2: " << b2 << std::endl;

	std::cout << "聚合类型初始化" << std::endl;
	std::cout << "a1.a: " << a1.a << " ";
	std::cout << "a1.b: " << a1.b << std::endl;
	std::cout << "a2.a: " << a2.a << " ";
	std::cout << "a2.b: " << a2.b << std::endl;

	std::cout << "类类型初始化" << std::endl;
	std::cout << "obj1: " << obj1.a << " " << obj1.b << std::endl;
	std::cout << "str: " << str << std::endl;

	std::cout << "容器和数组初始化" << std::endl;
	std::cout << "vec: ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << vec[i] << " ";
	}
	std::cout << vec[4] << std::endl;
	std::cout << "arr: ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << arr[4] << std::endl;

	return 0;
}
#endif