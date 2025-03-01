#include<iostream>
#include<vector>
/********************************************************************************************************************
*C++��ʼ��
* 
*1.��ʼ����ָΪ���������ʼֵ����C++�У���ʼ���ж��ַ�ʽ���������Ƴ�ʼ����ֱ�ӳ�ʼ����
*	1.1. ���Ƴ�ʼ����copy initialization����ʹ�õȺ�=���г�ʼ��
*	1.2. ֱ�ӳ�ʼ����direct initialization����ʹ��Բ����()������{}���г�ʼ��
*
*2.��ʼ���͸�ֵ:��ʼ���ĵȺź͸�ֵ�ĵȺź��岻ͬ
*	2.1.��ʼ����Ϊ��������洢�ռ䣬�����µı���������������ͣ���������Ĺ��캯��
*	2.2.��ֵ����һ�����б�����ֵ����һ��ֵ������������µı���������������ͣ���������ĸ�ֵ����� operator=()
*	e.g.int a = 1; // ��ʼ�� 
*		a = 2;     // ��ֵ
*		ClassType obj1;			 // ��ʼ��,���ù��캯��
* 		ClassType obj2("Hello"); // ��ʼ��,���ù��캯��
* 		obj1 = obj2;			 // ��ֵ
* 
*3.�б��ʼ����list initialization����ʹ�û�����{}���г�ʼ��
*	C++ ��ʼ����ʽ�е� (2)(4) �������б��ʼ������ C++11 �еõ�ȫ��Ӧ��
*	�������ص�
* 	3.1.ͳһ�﷨��ʹ�û�����{}���г�ʼ���������ڼ����������͵ĳ�ʼ����
*	3.2.�Զ�����խ��ת�������б��ʼ���У������ʼ�����̻ᵼ�����ݶ�ʧ���磬�� double �� int ��ת�������������ᱨ��
*		long double num = 3.1415;
*		int a{num};    // �޷����룬ת��������Ϣ��ʧ�ķ���
*		int b = {num}; // Բ�����򲻻ᱨ������Ϣ��ʧ����������
*	3.3.�������������ͣ����� POD��Plain Old Data�����Ա�ֱ��ʹ�� memcpy ���и��ƵĶ������͡��ۺ����͡������͵ȡ�
********************************************************************************************************************/
#if 0
int main()
{
	int a = 1;		//��1�� ���Ƴ�ʼ����copy initialization��
	int b = { 2 };  //��2�� ���Ƴ�ʼ����copy initialization��
	int c(3);		//��3�� ֱ�ӳ�ʼ����direct initialization��
	int d{ 4 };		//��4�� ֱ�ӳ�ʼ����direct initialization��

	int arr_copy[5] = { 1,2,3,4,5 };	//���Ƴ�ʼ����copy initialization��
	int arr_direct1[5]{ 1,2,3 }; 		//ֱ�ӳ�ʼ����direct initialization��
	int arr_direct2[5]{ 1,2,3,4,5 };	//ֱ�ӳ�ʼ����direct initialization��

	long double num = 3.1415;
	//int a1{ num };     ����	C2397	�ӡ�long double��ת������int����Ҫ����ת��	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	41
	//int a2 = { num };  ����	C2397	�ӡ�long double��ת������int����Ҫ����ת��	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	42
	int b1 = num;	  // ����	C4244	����ʼ���� : �ӡ�long double��ת������int�������ܶ�ʧ����	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	43
	int b2(num);	  // ����	C4244	����ʼ���� : �ӡ�long double��ת������int�������ܶ�ʧ����	C++_Learninng	D : \cpp language\C++_Learninng\Initialization.cpp	44

	//�ۺ����ͳ�ʼ�����ۺ�������ָû���û��Զ���Ĺ��캯���������������������캯���͸�ֵ��������࣬�������顢�ṹ��ȡ�
	struct A
	{
		int a;
		int b;
	};
	A a1 = { 1,2 };	//���Ƴ�ʼ��, C++11������б��ʼ��
	A a2{ 3,4 };	//ֱ�ӳ�ʼ��,,����һ�г�ʼ���ķ�ʽ����ͨ��

	//�����ͳ�ʼ��
	class MyClass {
        public:
            MyClass(int x, double y) : a(x), b(y) {}
            int a;
            double b;
    };

	MyClass obj1{ 5, 3.14 };				 // �б��ʼ��
	std::string str{ "Hello, C plusplus!" }; // �б��ʼ��

	//�����������ʼ��
	std::vector<int> vec{ 1,2,3,4,5 }; // �б��ʼ��
	int arr[]{ 1,2,3,4,5 };			   // �����б��ʼ��

	//������
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

	std::cout << "����խ��ת��" << std::endl;
	//std::cout << "a1: " << a1 << " ";
	//std::cout << "a2: " << a2 << std::endl;
	std::cout << "a1, a2 �޷�����" << std::endl;
	std::cout << "b1: " << b1 << " ";
	std::cout << "b2: " << b2 << std::endl;

	std::cout << "�ۺ����ͳ�ʼ��" << std::endl;
	std::cout << "a1.a: " << a1.a << " ";
	std::cout << "a1.b: " << a1.b << std::endl;
	std::cout << "a2.a: " << a2.a << " ";
	std::cout << "a2.b: " << a2.b << std::endl;

	std::cout << "�����ͳ�ʼ��" << std::endl;
	std::cout << "obj1: " << obj1.a << " " << obj1.b << std::endl;
	std::cout << "str: " << str << std::endl;

	std::cout << "�����������ʼ��" << std::endl;
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