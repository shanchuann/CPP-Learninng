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
void func(int a) {
    cout << "func with int: " << a << endl;
}
void func(double a) {
    cout << "func with double: " << a << endl;
}

// 2.2 ����������ͬ������
void func(int a, int b) {
    cout << "func with two int: " << a << ", " << b << endl;
}

// 2.3 ����˳��ͬ�����أ������ڲ�ͬ���Ͳ�����
void print(int a, double b) {
    cout << "print(int, double): " << a << ", " << b << endl;
}
void print(double a, int b) {
    cout << "print(double, int): " << a << ", " << b << endl;
}

// 3.1 const�������ò������ɹ������أ����õ�const���Բ�ͬ��
void print(int& a) {
    cout << "print(int&): " << a << endl;
}
void print(const int& a) {
    cout << "print(const int&): " << a << endl;
}

// 3.2 const���ΰ�ֵ���ݲ��������ܹ������أ�����ǩ����ͬ�����뱨��
// ����ʾ�����������������ᱻ��Ϊ�ظ�����
// void show(int a) { cout << "show(int): " << a << endl; }
// void show(const int a) { cout << "show(const int): " << a << endl; }

#if 0
int main() {
    cout << "----- �������Ͳ�ͬ������ -----" << endl;
    func(10);       // ƥ�� func(int)
    func(10.5);     // ƥ�� func(double)
    cout << endl;

    cout << "----- ����������ͬ������ -----" << endl;
    func(10, 20);   // ƥ�� func(int, int)
    cout << endl;

    cout << "----- ����˳��ͬ������ -----" << endl;
    print(10, 20.5);  // ƥ�� print(int, double)
    print(20.5, 10);  // ƥ�� print(double, int)
    cout << endl;

    cout << "----- const�������ò��������� -----" << endl;
    int x = 10;
    const int y = 20;
    print(x);       // ƥ�� print(int&)��x�ǿ��޸ĵ���ֵ��
    print(y);       // ƥ�� print(const int&)��y��const��ֵ��
    print(12);      // ƥ�� print(const int&)��12����ֵ��ֻ�ܰ�const���ã�
    cout << endl;

    return 0;
}
#endif