#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<typeinfo> // ����typeid()��ȡ������Ϣ
using namespace std;
/*******************************************************************************************************************
* C++�����Ƶ���Type Deduction��
* �����Ƶ���C++11��������׼����ĺ������ԣ�ͨ��auto��decltype���ֹؼ���ʵ�֣�������ʽָ�����ͼ����Ƶ�����/���ʽ�����ͣ�
* �򻯴����д���������ԣ�������ģ��͸������ͳ�������
*
* һ��auto�����Ƶ�
* ���壺auto���ݱ�����**��ʼ��ֵ**�Զ��Ƶ����ͣ������ǡ�����ʼ��ֵȷ�����͡������auto���������ʼ����
*
* ���Ĺ���
* 1. �����ʼ����auto�����޳�ʼ��ֵʱ�޷��Ƶ����ͣ����뱨����
* 2. ��һ�����Ƶ���ͬһ����auto�����������ʱ�����б����������Ƶ�Ϊͬһ���ͣ�������뱨����
* 3. const���Դ���
*    - ���ԡ�����const�������������const����const int ca=10; auto a=ca; �� a�Ƶ�Ϊint��
*    - �������ײ�const����ָ��/����ָ������const����const int ca=10; auto b=&ca; �� b�Ƶ�Ϊconst int*��
* 4. ����/ָ���Ƶ���
*    - auto���&���Ƶ�Ϊ�������ͣ�auto& c=ca; �� cΪconst int&����
*    - auto���*���Ƶ�Ϊָ�����ͣ�auto* p=&b; �� pΪint*��ǰ���ǳ�ʼ��ֵΪָ�룩��
* 5. ��չ������
*    - ����������C++14��֧�֣���auto��Ϊ��������ʱ��ʵ����ģ�������������Ϊ���ͺ�������
*    - new���ʽ��auto* p=new int(20); �� auto s=new auto(30); �� ���Ƶ�Ϊint*��
*
* ����decltype�����Ƶ�
* ���壺decltype����**��������ʽ**�������Ƶ��������ǡ�ֱ�ӻ�ȡ���ʽ�����͡��������ʼ���������ʽ��Ϸ�����
*
* ���Ĺ���
* 1. �Ƶ��������ͣ�decltype(������) �� ��ȫ�������������ͣ�����const�����ã���
*    - int x=10; decltype(x) a; �� aΪint��int& rx=x; decltype(rx) b=x; �� bΪint&��
* 2. �Ƶ����ʽ���ͣ�
*    - �����ʽ�ǡ������������š�����decltype((x))�����Ƶ�Ϊ�ñ������������ͣ�int&����
*    - �����ʽ����ͨ���㣨��x+y�����Ƶ�Ϊ�����������ͣ�int����
* 3. �����������ԣ�decltype�ᱣ�����ʽ��const��volatile���������ԣ������κκ��ԡ�
*    - const int& rc=x; decltype(rc) rb=x; �� rbΪconst int&��
* 4. �Ƶ���������ֵ��decltype(������(��������)) �� �Ƶ������ķ���ֵ���ͣ�������ú�������
*    - int add(int a,int b); decltype(add(0,0)) z=0; �� zΪint��
*
* ����auto��decltype�ĺ�������
* | ����                | auto                          | decltype                      |
* |---------------------|-------------------------------|-------------------------------|
* | �Ƶ�����            | �����ĳ�ʼ��ֵ                | ��������ʽ����              |
* | �Ƿ���Ҫ��ʼ��      | �����ʼ���������޷��Ƶ���    | �����ʼ�������ʽ�Ϸ����ɣ�  |
* | const���Դ���       | ���Զ���const�������ײ�const  | ��ȫ��������const����         |
* | ���������Ƶ�        | ����ʽ��&���Ƶ�Ϊ����         | �Զ����ݱ��ʽ�����Ƶ�����     |
* | ���ʽ����Ӱ��      | ��Ӱ�죨������ʼ��ֵ���ͣ�    | (������)�ᵼ���Ƶ�Ϊ����       |
*******************************************************************************************************************/

// 1. auto�����Ƶ�ʾ��
// 1.1 auto�����Ƶ�����ͨ���͡�const��ָ�롢���ã�
void auto_basic() {
    cout << "----- auto�����Ƶ� -----" << endl;
    const int ca = 10;    // ����const����
    int x = 20;
    int& rx = x;          // ���ñ���

    // ��ͨ�Ƶ������Զ���const
    auto a = ca;          // a�Ƶ�Ϊint������ca�Ķ���const��
    cout << "auto a = ca �� type: " << typeid(a).name() << " (ֵ: " << a << ")" << endl;

    // ָ���Ƶ��������ײ�const
    auto b = &ca;         // b�Ƶ�Ϊconst int*��&ca��const int*���ײ�const������
    cout << "auto b = &ca �� type: " << typeid(b).name() << " (ָ��ֵ: " << *b << ")" << endl;

    // �����Ƶ�����ʽ��&������const
    auto& c = ca;         // c�Ƶ�Ϊconst int&����ca��
    cout << "auto& c = ca �� type: " << typeid(c).name() << " (ֵ: " << c << ")" << endl;

    // ָ�����*���Ƶ�
    auto* p = &x;         // p�Ƶ�Ϊint*��&x��int*����auto*ƥ�䣩
    auto q = p;           // q�Ƶ�Ϊint*��auto�Զ�ʶ��ָ�����ͣ�
    cout << "auto* p = &x �� type: " << typeid(p).name() << " (ָ��ֵ: " << *p << ")" << endl;
    cout << "auto q = p �� type: " << typeid(q).name() << " (ָ��ֵ: " << *q << ")" << endl;

    // ����ʾ����auto�����Ƶ��������ͣ�ע�͵�������뱨��
    // auto err = 5, err2 = 10.5; // ����autoֻ���Ƶ�һ�����ͣ�int��double��ͻ��
    cout << endl;
}

// 1.2 auto��new���ʽ�Ͷ���������е�ʹ��
void auto_new_multi() {
    cout << "----- auto��new�Ͷ���������� -----" << endl;
    // new���ʽ�е�auto
    auto* p1 = new int(30);    // p1�Ƶ�Ϊint*
    auto p2 = new auto(40);    // p2�Ƶ�Ϊint*��new auto���ݳ�ʼ��ֵ�Ƶ���
    cout << "auto* p1 = new int(30) �� ָ��ֵ: " << *p1 << endl;
    cout << "auto p2 = new auto(40) �� ָ��ֵ: " << *p2 << endl;

    // ����������������Ƶ�Ϊͬһ����
    auto* p3 = &p1, a = p1;     // p3�Ƶ�Ϊint**��&p1��int**����a�Ƶ�Ϊint��auto*��Լ��p3��a������
    cout << "auto* p3 = &p1, a = 5 �� p3����: " << typeid(p3).name() << ", a����: " << typeid(a).name() << endl;

    // �ͷ��ڴ�
    delete p1;
    delete p2;
    p1 = nullptr;
    p2 = nullptr;
    cout << endl;
}

// 1.3 auto��Ϊ����������C++14��֧�֣������Ƿ��ͺ�����
void auto_func_param(auto x) {
    cout << "auto������������ֵ: " << x << " (����: " << typeid(x).name() << ")" << endl;
}

void auto_function() {
    cout << "----- auto��Ϊ�������� -----" << endl;
    auto_func_param(10);       // x�Ƶ�Ϊint
    auto_func_param(10.5);     // x�Ƶ�Ϊdouble
    auto_func_param("hello");  // x�Ƶ�Ϊconst char*
    cout << endl;
}

// 2. decltype�����Ƶ�ʾ��
// 2.1 decltype�Ƶ��������ͣ�����const�����ã�
void decltype_var() {
    cout << "----- decltype�Ƶ��������� -----" << endl;
    int x = 10;
    const int& rx = x;    // const����
    int& rrx = x;         // ��ͨ����

    // �Ƶ���ͨ����������ԭʼ����
    decltype(x) a;        // a�Ƶ�Ϊint��x��int��
    a = 20;
    cout << "decltype(x) a �� type: " << typeid(a).name() << " (ֵ: " << a << ")" << endl;

    // �Ƶ����ñ�����������������
    decltype(rx) b = x;   // b�Ƶ�Ϊconst int&��rx��const int&��
    cout << "decltype(rx) b �� type: " << typeid(b).name() << " (ֵ: " << b << ")" << endl;

    // �Ƶ����õ����ã��۵�Ϊ��ͨ���ã�C++�����۵�����
    decltype(rrx) c = x;  // c�Ƶ�Ϊint&��rrx��int&��
    cout << "decltype(rrx) c �� type: " << typeid(c).name() << " (ֵ: " << c << ")" << endl;
    cout << endl;
}

// 2.2 decltype�Ƶ����ʽ���ͣ�����Ӱ�졢��������
void decltype_expr() {
    cout << "----- decltype�Ƶ����ʽ���� -----" << endl;
    int x = 10, y = 20;

    // �Ƶ���ͨ������ʽ�����Ϊint
    decltype(x + y) z;    // x+y��int��z�Ƶ�Ϊint
    z = 30;
    cout << "decltype(x + y) z �� type: " << typeid(z).name() << " (ֵ: " << z << ")" << endl;

    // �Ƶ������������š������Ϊ���ã�(x)����ֵ���ʽ��
    decltype((x)) rx = x; // (x)����ֵ��rx�Ƶ�Ϊint&
    rx = 100;             // �޸�rx���޸�x
    cout << "decltype((x)) rx �� type: " << typeid(rx).name() << " (xֵ��Ϊ: " << x << ")" << endl;

    // �Ƶ�ָ����ʽ�����Ϊָ������
    int* p = &x;
    decltype(&x) q = p;   // &x��int*��q�Ƶ�Ϊint*
    cout << "decltype(&x) q �� type: " << typeid(q).name() << " (ָ��ֵ: " << *q << ")" << endl;
    cout << endl;
}

// 2.3 decltype�Ƶ���������ֵ����
int type_add(int a, int b) { return a + b; } // ��������int

void decltype_func_ret() {
    cout << "----- decltype�Ƶ���������ֵ -----" << endl;
    // �Ƶ���������ֵ���ͣ�������ú���������������ͺϷ���
    decltype(type_add(0, 0)) ret;  // type_add(0,0)����int��ret�Ƶ�Ϊint
    ret = type_add(5, 3);
    cout << "decltype(type_add(0,0)) ret �� type: " << typeid(ret).name() << " (type_add(5,3)= " << ret << ")" << endl;
    cout << endl;
}

// 3. auto��decltype������Ա�ʾ��
void auto_vs_decltype() {
    cout << "----- auto��decltype����Ա� -----" << endl;
    const int ca = 10;

    // 1. const���Դ������
    auto a = ca;          // auto���Զ���const �� int
    decltype(ca) b = 20;  // decltype����const �� const int
    cout << "auto a = ca �� type: " << typeid(a).name() << " (��const)" << endl;
    cout << "decltype(ca) b �� type: " << typeid(b).name() << " (����const)" << endl;

    // 2. �����Ƶ�����
    auto& c = ca;         // auto����ʽ��& �� const int&
    decltype((ca)) d = ca;// decltype�Զ��Ƶ����� �� const int&��(ca)����ֵ��
    cout << "auto& c = ca �� type: " << typeid(c).name() << " (��ʽ��&)" << endl;
    cout << "decltype((ca)) d �� type: " << typeid(d).name() << " (�Զ��Ƶ�����)" << endl;
}

#if 0
int main() {
    // ����auto���ʾ��
    auto_basic();         // auto�����Ƶ�
    auto_new_multi();     // auto��new�Ͷ������
    auto_function();      // auto��Ϊ��������

    // ����decltype���ʾ��
    decltype_var();       // decltype�Ƶ�����
    decltype_expr();      // decltype�Ƶ����ʽ
    decltype_func_ret();  // decltype�Ƶ���������ֵ

    // �Ա�auto��decltype
    auto_vs_decltype();

    return 0;
}
#endif