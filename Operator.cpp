#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

/*******************************************************************************************************************
* C++ ��������أ�Operator Overloading��
* ���壺�����������C++�����û�Ϊ�Զ������ͣ����ࡢ�ṹ�壩���¶������������Ϊ��ʹ�������������������ͣ�int��double��һ��
*      �����Զ��������ߴ���Ŀɶ��Ժ�ֱ���ԡ������ǡ�ͨ�����⺯����operator�������ʵ�ֵĺ������ء���
*
* ���Ĺ���
* 1. ���ܴ������������ֻ������C++���е����������+��-��==��++��<<�ȣ��������Զ��������������#��@����
* 2. ��������ȼ��ͽ���Բ��䣺���غ�����������ȼ�������ԡ�������������ԭ�����һ�£���+��Ϊ���ϣ����ȼ����䣩��
* 3. ��������Զ������ͣ�������һ�����������Զ������ͣ��������ػ������͵����������int+int����Ϊ�����޸ģ���
* 4. ����ʵ�ַ�ʽ��
*    - ���Ա�����������������Ϊ��ĳ�Ա���������Ϊ��ǰ����thisָ��ָ�򣩣��Ҳ�����Ϊ����������
*    - ȫ�ֺ�������Ϊ��Ԫ�������������Ϊȫ�ֺ��������Ҳ�������Ϊ�������������������˽�г�Ա��������Ϊ��Ԫ��
*
* �������س�����
* 1. �����������+��-��*��/����ʵ���Զ������ļӼ��˳����縴����ӡ��������㣩��
* 2. ��ϵ�������==��!=��>��<����ʵ���Զ������ıȽϣ����ж���������������Ƿ���ȣ���
* 3. ����/�Լ��������++��--��������ǰ׺��++a���ͺ�׺��a++������׺���һ�����õ�int������Ϊ��ǣ�
* 4. ���������<<��>>����ʵ���Զ��������������������cout���������cin����������ԣ���������Ϊȫ����Ԫ������
*******************************************************************************************************************/

// 1. ����1��������������أ���Ա����ʵ�֣��Ը������Ϊ����
void arithmetic_operator_overload() {
    std::cout << "----- ������������أ���Ա������������ӣ� -----" << std::endl;

    class Complex {
    public:
        double real; // ʵ��
        double imag; // �鲿

        // ���캯������ʼ������
        Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

        // ����+���������Ա���������������Ϊthisָ��ָ��Ķ����Ҳ�����Ϊother
        Complex operator+(const Complex& other) const {
            // ������ӹ���(a+bi) + (c+di) = (a+c) + (b+d)i
            return Complex(real + other.real, imag + other.imag);
        }
    };

    // ʹ�����ص�+�����
    Complex c1(1.0, 2.0);  // 1 + 2i
    Complex c2(3.0, 4.0);  // 3 + 4i
    Complex c3 = c1 + c2;  // �ȼ��� c1.operator+(c2)

    std::cout << "c1 = " << c1.real << " + " << c1.imag << "i" << std::endl;
    std::cout << "c2 = " << c2.real << " + " << c2.imag << "i" << std::endl;
    std::cout << "c3 = c1 + c2 = " << c3.real << " + " << c3.imag << "i" << "\n" << std::endl;
}

// 2. ����2����ϵ��������أ�ȫ�ֺ���ʵ�֣��Զ������ԱȽ�Ϊ����
class Person {
public:
    int age; // ��������

    // ���캯������ʼ������
    Person(int a = 0) : age(a) {}

    // ����==�����Ϊȫ����Ԫ�������˽�г�Աʱʹ�ã�����ageΪpublic��ʡ����Ԫ������
    friend bool operator==(const Person& p1, const Person& p2);
};

// ����==�������ȫ�ֺ����������Ҳ�������Ϊ��������
bool operator==(const Person& p1, const Person& p2) {
    // �Ƚ�����Person�����age����
    return p1.age == p2.age;
}
void relation_operator_overload() {
    std::cout << "----- ��ϵ��������أ�ȫ�ֺ�������������Ƚϣ� -----" << std::endl;

    // ʹ�����ص�==�����
    Person p1(18), p2(18), p3(20);
    std::cout << "p1.age = " << p1.age << ", p2.age = " << p2.age << std::endl;
    if (p1 == p2) { // �ȼ��� operator==(p1, p2)
        std::cout << "p1 �� p2 �������" << std::endl;
    }
    else {
        std::cout << "p1 �� p2 ���䲻��" << std::endl;
    }

    std::cout << "p1.age = " << p1.age << ", p3.age = " << p3.age << std::endl;
    if (p1 == p3) {
        std::cout << "p1 �� p3 �������" << std::endl;
    }
    else {
        std::cout << "p1 �� p3 ���䲻��" << std::endl;
    }
    std::cout << "\n" << std::endl;
}

// 3. ����3��������������أ���Ա����ʵ�֣�����ǰ׺�ͺ�׺��
void increment_operator_overload() {
    std::cout << "----- ������������أ���Ա����������ǰ׺++�ͺ�׺++�� -----" << std::endl;

    class UPInt {
    private:
        int value; // �洢������ֵ
    public:
        // ���캯������ʼ��ֵ
        UPInt(int v = 0) : value(v) {}

        // 1. ǰ׺�����������++a�����޲��������ض������ã�֧����ʽ������
        UPInt& operator++() {
            ++value; // ������
            return *this; // ����������ĵ�ǰ����
        }

        // 2. ��׺�����������a++�������int��������ʵ�����壬����Ϊ��ǣ�������const��ʱ����
        const UPInt operator++(int) {
            UPInt temp = *this; // ��������ǰ�Ķ���
            ++value; // ������
            return temp; // ��������ǰ����ʱ����
        }

        // �������������ֵ��������֤��
        void show() const {
            std::cout << "value = " << value << std::endl;
        }
    };

    // ʹ�����ص����������
    UPInt a(10);
    std::cout << "��ʼa��";
    a.show();

    ++a; // ����ǰ׺������operator++()
    std::cout << "++a��ǰ׺����";
    a.show();

    a++; // ���ú�׺������operator++(0)���������Զ�����int������
    std::cout << "a++����׺����";
    a.show();

    // ��֤ǰ׺�ͺ�׺�ķ���ֵ����
    UPInt b(5), c(5);
    UPInt d = ++b; // ǰ׺�������������d = 6
    UPInt e = c++; // ��׺��������ǰ����e = 5
    std::cout << "++b��b��"; b.show(); std::cout << "d = ++b��"; d.show();
    std::cout << "c++��c��"; c.show(); std::cout << "e = c++��"; e.show();
    std::cout << "\n" << std::endl;
}

// 4. ����4������������أ�ȫ����Ԫ������ʵ�ֶ����cout�����
class Complex {
public:
    double real; // ʵ��
    double imag; // �鲿

    // ���캯������ʼ������
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // ����<<�����Ϊ��Ԫ�������˽�г�Աreal��imag��
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};
// ����<<�������ȫ����Ԫ���������������Ϊostream������cout�����Ҳ�����ΪComplex����
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    // �Զ��������ʽ��(ʵ��, �鲿)
    os << "(" << c.real << ", " << c.imag << ")";
    return os; // ����ostream����֧����ʽ�������cout << c1 << c2��
}

void stream_operator_overload() {
    std::cout << "----- ����������أ�ȫ����Ԫ��cout��������� -----" << std::endl;

    // ʹ�����ص�<<�����
    Complex c1(1.0, 2.0), c2(3.0, 4.0);
    std::cout << "c1 = " << c1 << std::endl; // �ȼ��� operator<<(cout, c1)
    std::cout << "c2 = " << c2 << std::endl; // �ȼ��� operator<<(cout, c2)
    std::cout << "c1 �� c2 �ֱ�Ϊ��" << c1 << " �� " << c2 << std::endl; // ��ʽ���
}

#if 0
int main() {
    arithmetic_operator_overload();   // ������������أ�������ӣ�
    relation_operator_overload();     // ��ϵ��������أ�����Ƚϣ�
    increment_operator_overload();    // ������������أ�ǰ׺vs��׺��
    stream_operator_overload();       // ����������أ�cout�������
    return 0;
}
#endif