#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring> // ����C����ַ�������������֤c_str()�����
using namespace std;
/*******************************************************************************************************************
* C++ std::string ���Ĳ���������ת��
* std::string��C++��׼���ṩ���ַ����࣬��װ���ַ����Ĵ洢���޸ġ����ҵȹ��ܣ������C����ַ�����char*����������������������⡣
* �������ݰ�����C����ַ�����std::string��˫��ת����std::string���ó�Ա�������ַ�����������ء�
*
* һ��C����ַ�����std::string��˫��ת��
* 1. C����ַ��� �� std::string����ʽת������ֱ�ӣ�
*    - �﷨��std::string ������ = C����ַ�����const char* ���ͣ�;
*    - ԭ��std::string�Ĺ��캯��֧�ֽ���const char*�������Զ������ַ���string�ڲ��������������ֶ������ڴ档
*
* 2. std::string �� C����ַ�������ʽ���ó�Ա����c_str()��
*    - �﷨��const char* ָ���� = string����.c_str();
*    - �ؼ�ע�����
*      - c_str()���ص���ָ��string�ڲ���������const char*ָ�룬**�����޸�**���޸Ļᵼ��δ������Ϊ����
*      - ָ�����Ч��������ԭstring���󣺵�string�����޸ģ���ƴ�ӡ���գ�������ʱ��ָ������ʧЧ��
*      - �����޸�ת�����C����ַ��������ֶ�����c_str()������µ�char���飨����strcpy()����
*
* ����std::string���ó�Ա����
* 1. �������滻
*    - find(const string& sub)�������Ӵ�sub�ڵ�ǰstring��**�״γ��ֵ���ʼλ��**������size_t���ͣ�δ�ҵ�����string::npos����
*    - replace(size_t pos, size_t len, const string& rep)����posλ�ÿ�ʼ���滻len���ַ�Ϊ�ַ���rep��
*
* 2. ƴ������չ
*    - append(const string& str)���ڵ�ǰstringĩβ׷���ַ���str��
*    - ��д����operator+=��ֱ����+=׷���ַ������﷨����࣬������append()��Ч����
*
* 3. ������п�
*    - clear()�����string�е������ַ���ʹ���Ϊ���ַ���������Ϊ0����
*    - empty()���ж�string�Ƿ�Ϊ�գ�����bool������true���ǿ���false��������clear()����ʹ�á�
*
* ����std::string��������أ��򻯲�����
* 1. operator+��ƴ������string�����string��C����ַ����������µ�string����ԭ�����޸ģ���
* 2. operator+=���ڵ�ǰstringĩβ׷���ַ������޸�ԭ����Ч�ʱ�operator+�ߣ����贴����ʱ���󣩣�
* 3. ���������������==���ж��ַ�����ȣ���!=���жϲ��ȣ���<���ֵ���Ƚϣ��ȣ�֧��ֱ�۵��ַ����Ƚϡ�
*******************************************************************************************************************/

// 1. ����1��C����ַ�����std::string��˫��ת��
void str_conversion() {
    cout << "----- C����ַ�����std::string˫��ת�� -----" << endl;

    // 1.1 C����ַ��� �� std::string����ʽת����
    const char* c_str = "Hello C"; // C����ַ�����const char*��
    string s = c_str;              // ֱ�Ӹ�ֵ���Զ�ת��Ϊstd::string
    cout << "C����ַ���תstd::string��" << s << endl;

    // 1.2 std::string �� C����ַ���������c_str()��
    const char* new_c_str = s.c_str(); // ��ȡstring�ڲ���C����ַ���ָ��
    cout << "std::stringתC����ַ�����" << new_c_str << endl;

    // ע�⣺c_str()���ص�ָ�벻���޸ģ�const���Σ���������ԭstring����
    // char* modify_ptr = s.c_str(); // ���󣺲��ܽ�const char*��ֵ��char*
    // �����޸ģ����ֶ�������char����
    char modifiable_arr[20];
    strcpy(modifiable_arr, s.c_str()); // ����c_str()��������޸�����
    modifiable_arr[5] = '!';          // �޸����飨�Ϸ���
    cout << "�ֶ���������޸ĵ�C����ַ�����" << modifiable_arr << "\n" << endl;
}

// 2. ����2��std::string�Ĳ������滻
void str_find_replace() {
    cout << "----- std::string�������滻 -----" << endl;

    string s = "I love C++, C++ is easy!";
    cout << "ԭʼ�ַ�����" << s << endl;

    // 2.1 �����Ӵ�"C++"���״�λ�ã�find()����size_t���ͣ�
    size_t pos = s.find("C++"); // ���ҳɹ�������ʼ������ʧ�ܷ���string::npos
    if (pos != string::npos) {  // �ж��Ƿ��ҵ��Ӵ�
        cout << "�Ӵ�\"C++\"�״γ���λ�ã�" << pos << endl;

        // 2.2 �滻�Ӵ�����posλ�ÿ�ʼ���滻3���ַ���"C++"����Ϊ3��Ϊ"C plus plus"
        s.replace(pos, 3, "C plus plus");
        cout << "�滻���ַ�����" << s << "\n" << endl;
    }
    else {
        cout << "δ�ҵ��Ӵ�\"C++\"" << "\n" << endl;
    }
}

// 3. ����3��std::string��ƴ�ӡ�������п�
void str_append_clear() {
    cout << "----- std::stringƴ�ӡ�������п� -----" << endl;

    // 3.1 ƴ���ַ�����append()��operator+=
    string s = "Hello";
    s.append(" World"); // ��append()׷���ַ���
    cout << "append(\" World\")��" << s << endl;

    s += "!"; // ��operator+=׷�ӣ���д������append()��Ч��
    cout << "operator+=��\"!\"����" << s << endl;

    // 3.2 ������п�
    s.clear(); // ����ַ�������Ϊ�մ�
    if (s.empty()) { // �ж��Ƿ�Ϊ��
        cout << "clear()���ַ�����\"" << s << "\"�����ַ�����" << "\n" << endl;
    }
}

// 4. ����4��std::string��operator+ƴ�ӣ������¶���
void str_operator_plus() {
    cout << "----- std::string operator+ƴ�� -----" << endl;

    string s1 = "Hello";
    string s2 = "World";

    // ��operator+ƴ�ӣ�s1 + " " + s2�������µ�string����s3
    string s3 = s1 + " " + s2;
    cout << "s1��" << s1 << "�� + \" \" + s2��" << s2 << "�� = " << s3 << endl;

    // ע�⣺operator+���޸�ԭ����ԭs1��s2���ֲ���
    cout << "ƴ�Ӻ�ԭs1��" << s1 << "��ԭs2��" << s2 << endl;
}

#if 0
int main() {
    str_conversion();         // C����ַ�����std::stringת��
    str_find_replace();       // �������滻
    str_append_clear();       // ƴ�ӡ�������п�
    str_operator_plus();      // operator+ƴ��
    return 0;
}
#endif