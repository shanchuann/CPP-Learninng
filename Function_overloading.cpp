#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
/*******************************************************************************************************************
* �������أ�Function Overloading��
* ���壺��ͬһ�������ڣ�������ڶ��ͬ��������ֻҪ���ǵ�**�����б�ͬ**�������������������͡�����˳��ͬ����
*      ע�⣺����ֵ���Ͳ�ͬ������Ϊ�������ص����ݣ�������ֵ��ͬ��ͬ�������ᱻ��Ϊ�ظ����壩��
*
* ���Ĺ���
* 1. ���ص��ж������������б�����ǩ�������뷵��ֵ���͡��������޹ء�
* 2. �����б�ͬ��������ʽ��
*    - ����������ͬ��
*    - �������Ͳ�ͬ��
*    - ����˳��ͬ�������ڲ�ͬ���͵Ĳ�������
* 3. const/volatile���η������ص�Ӱ�죺
*    - ������Ϊ**���û�ָ��**ʱ��const/volatile���λᵼ�º���ǩ����ͬ���ɹ������أ�
*    - ������Ϊ**��ֵ����**ʱ��const/volatile���β���ı亯��ǩ�������β���ʵ�εĸ����������ܹ������ء�
*******************************************************************************************************************/

// 2.1 �������Ͳ�ͬ������
void Function_Overloadingfunc(int a) {
    std::cout << "func with int: " << a << std::endl;
}
void Function_Overloadingfunc(double a) {
    std::cout << "func with double: " << a << std::endl;
}

// 2.2 ����������ͬ������
void Function_Overloadingfunc(int a, int b) {
    std::cout << "func with two int: " << a << ", " << b << std::endl;
}

// 2.3 ����˳��ͬ�����أ������ڲ�ͬ���Ͳ�����
void Function_Overloadingprint(int a, double b) {
    std::cout << "print(int, double): " << a << ", " << b << std::endl;
}
void Function_Overloadingprint(double a, int b) {
    std::cout << "print(double, int): " << a << ", " << b << std::endl;
}

// 3.1 const�������ò������ɹ������أ����õ�const���Բ�ͬ��
void Function_Overloadingprint(int& a) {
    std::cout << "print(int&): " << a << std::endl;
}
void Function_Overloadingprint(const int& a) {
    std::cout << "print(const int&): " << a << std::endl;
}

// 3.2 const���ΰ�ֵ���ݲ��������ܹ������أ�����ǩ����ͬ�����뱨��
// ����ʾ�����������������ᱻ��Ϊ�ظ�����
// void show(int a) { cout << "show(int): " << a << endl; }
// void show(const int a) { cout << "show(const int): " << a << endl; }

#if 0
int main() {
    using namespace std;
    cout << "----- �������Ͳ�ͬ������ -----" << endl;
    Function_Overloadingfunc(10);       // ƥ�� func(int)
    Function_Overloadingfunc(10.5);     // ƥ�� func(double)
    cout << endl;

    cout << "----- ����������ͬ������ -----" << endl;
    Function_Overloadingfunc(10, 20);   // ƥ�� func(int, int)
    cout << endl;

    cout << "----- ����˳��ͬ������ -----" << endl;
    Function_Overloadingprint(10, 20.5);  // ƥ�� print(int, double)
    Function_Overloadingprint(20.5, 10);  // ƥ�� print(double, int)
    cout << endl;

    cout << "----- const�������ò��������� -----" << endl;
    int x = 10;
    const int y = 20;
    Function_Overloadingprint(x);       // ƥ�� print(int&)��x�ǿ��޸ĵ���ֵ��
    Function_Overloadingprint(y);       // ƥ�� print(const int&)��y��const��ֵ��
    Function_Overloadingprint(12);      // ƥ�� print(const int&)��12����ֵ��ֻ�ܰ�const���ã�
    cout << endl;

    return 0;
}
#endif