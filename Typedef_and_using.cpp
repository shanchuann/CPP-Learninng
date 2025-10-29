#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>

/*******************************************************************************************************************
* C++11 using������Using Alias��
* ���壺using������C++11���������/ģ��������ƣ�ͨ��`using ���� = Ŀ������/ģ��`�﷨��Ϊ�������ͻ�ģ�嶨���������
*      �����ϼ��ݲ���չ�˴�ͳ��typedef��������ģ����������н����typedef�ľ����ԡ�
*
* ���ķ��������
* һ����ͨ���ͱ��������typedef��
* 1. �﷨��using ���� = ��������;
*    - ��typedef������ȫ��Ч�����﷨��ֱ�ۣ�����ֵʽ��д�������ϱ�������ϰ�ߣ���
* 2. ֧�ֵ�Ŀ�����ͣ�
*    - �������ͣ���unsigned int��char*����
*    - �������ͣ���int[10]��char[20]����
*    - ָ��/�������ͣ���char*��const int&����
*    - �Զ������ͣ����ࡢ�ṹ�壩��
* 3. ��typedef�ĶԱȣ�
*    - typedef��typedef �������� ����; ������׺ʽ��д�������������׶��Բ��
*    - using��using ���� = ��������; ����ǰ׺ʽ��д�����߼�������������Ƕ�����ͣ���

* ����ģ�������C++11������typedef�޷�ʵ�֣�
* 1. �﷨��template<ģ������б�> using ���� = Ŀ��ģ������;
*    - �������ã�Ϊģ�嶨�塰�������ı�������ʵ��ģ�����͵ľֲ��̻���򻯣����typedef�޷�ģ�廯�����⡣
* 2. ֧�ֵ�Ŀ��ģ�����ͣ�
*    - ģ�廯�������ͣ���T[N]��TΪ���Ͳ�����NΪ�����Ͳ�������
*    - STL����ģ�壨��vector<T>��map<K,V>����
*    - �Զ���ģ�����͡�
* 3. �ؼ����ԣ�
*    - ģ������б������Ŀ��ģ������Ĳ��������Ͳ�����class/typename�������Ͳ����þ���������int����
*    - ʹ�ñ���ʱ����ʽ����ģ����������ɾ������ͣ���Array<int,10> �ȼ��� int[10]����

* ����using�����ĺ�������
* 1. �﷨ֱ�ۣ�������=Ŀ�����͡���д�������ճ���֪����typedef�ġ�Ŀ������ ���������׶������临�����ͣ��纯��ָ�롢��ά���飩��
* 2. ֧��ģ�����������typedef�ĺ��Ķ̰塪��typedef�޷�Ϊģ�嶨���������using������ʵ��ģ�廯�����ͱ�����
* 3. ������ǿ����ȫ����typedef�����г��������޷��滻typedef�����ڴ����ع�ʱ����ά����
* 4. �ɶ��Ըߣ�Ƕ�����ͻ�ģ�����͵ı������������������`using StrVec = vector<string>`��`typedef vector<string> StrVec`��ֱ�ۡ�
*******************************************************************************************************************/

// 1. ����1����ͨ���ͱ��������typedef���������͡�ָ�롢���飩
void using_basic_type_alias() {
    using namespace std;
    cout << "----- ��ͨ���ͱ��������typedef�� -----" << endl;

    // 1.1 �������ͱ�����unsigned int �� UINT
    using UINT = unsigned int;
    // typedef unsigned int UINT; // ������using�����ȫ��Ч
    UINT num = 100;
    cout << "UINT��unsigned int�����ͱ�����" << num << endl;

    // 1.2 ָ�����ͱ�����char* �� PCHAR
    using PCHAR = char*;
    // typedef char* PCHAR; // ������using�����ȫ��Ч
    PCHAR str_ptr = new char[20];
    strcpy(str_ptr, "Hello Using Alias");
    cout << "PCHAR��char*������ָ�룺" << str_ptr << endl;

    // 1.3 �������ͱ�����int[10] �� IntArray10
    using IntArray10 = int[10];
    // typedef int IntArray10[10]; // ������using�����ȫ��Ч
    IntArray10 arr = { 1,2,3,4,5,6,7,8,9,10 };
    cout << "IntArray10��int[10]���������飺";
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << "\n" << endl;

    // �ͷ�ָ���ڴ�
    delete[] str_ptr;
    str_ptr = nullptr;
}

// 2. ����2��ģ�������C++11������typedef�޷�ʵ�֣�
// 2.1 ����ģ�������Ϊ��T���͡�N��С�����顱����ģ�����Array
template<class T, int N> // T������ģ�������N��������ģ���������Ϊ�������ʽ��
using Array = T[N];       // ģ�������Array<T,N> �ȼ��� T[N]

// 2.2 ʹ��ģ��������ɾ�������
void using_template_alias() {
    using namespace std;
    cout << "----- ģ�������C++11������ -----" << endl;

    // 2.2.1 ���ɡ�int���͡�10��С�����������ͣ�Array<int,10> �� int[10]
    Array<int, 10> int_arr = { 0,1,2,3,4,5,6,7,8,9 };
    cout << "Array<int,10>��int[10]�����飺";
    for (int i = 0; i < 10; i++) {
        cout << int_arr[i] << " ";
    }
    cout << endl;

    // 2.2.2 ���ɡ�char���͡�20��С�����������ͣ�Array<char,20> �� char[20]
    Array<char, 20> char_arr;
    strcpy(char_arr, "Hello, World!"); // �ַ�������13��С��20����ȫ
    cout << "Array<char,20>��char[20]���ַ�����" << char_arr << endl;

    // 2.2.3 ģ�����������ԣ������л�����/��С
    Array<double, 5> double_arr = { 1.1, 2.2, 3.3, 4.4, 5.5 };
    cout << "Array<double,5>��double[5]�����飺";
    for (int i = 0; i < 5; i++) {
        cout << double_arr[i] << " ";
    }
    cout << "\n" << endl;
}

// 3. ����3��using��typedef�Ŀɶ��ԶԱȣ���������ʾ����
void using_vs_typedef_readability() {
    using namespace std;
    cout << "----- using��typedef�ɶ��ԶԱȣ��������ͣ� -----" << endl;

    // ʾ����Ϊ������ֵint������Ϊ(int, char*)�ĺ���ָ�롱�������
    // typedefд������׺ʽ�����������������׻�����
    typedef int (*FuncPtr_Typedef)(int, char*);
    // usingд������ֵʽ��ֱ�Ӷ�Ӧ������=Ŀ�����͡�����ֱ�ۣ�
    using FuncPtr_Using = int (*)(int, char*);

    // ��֤�����ֱ�����ȫ��Ч���ɻ���ʹ��
    auto test_func = [](int a, char* b) -> int {
        return a + strlen(b);
        };
    FuncPtr_Typedef fp1 = test_func;
    FuncPtr_Using fp2 = test_func;

    char test_str[] = "test";
    cout << "FuncPtr_Typedef���ý����" << fp1(5, test_str) << endl; // 5+4=9
    cout << "FuncPtr_Using���ý����" << fp2(5, test_str) << endl;   // ���һ��
}

#if 0
int main() {
    using_basic_type_alias();         // ��ͨ���ͱ�����ʾ�����typedef��
    using_template_alias();           // ģ�������ʾ��C++11������
    using_vs_typedef_readability();   // using��typedef�ɶ��ԶԱ�
    return 0;
}
#endif