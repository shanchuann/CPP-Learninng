#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
/*******************************************************************************************************************
* C++ �������캯������ֵ�����������const��Ա����
* �������ݾ۽����������Ļ��ƣ�const��Ա���������ƶ������Ȩ�ޣ����������캯�������󿽱���ʼ��������ֵ��������أ�����ֵ����
* ͬʱ��϶����������ڣ������������������������������������ݡ�����ֵ�����еĶ�����Ϊ�����⿽���������Դ����
*
* һ��const��Ա������const����
* 1. ���Ķ��壺
*    - const��Ա�������ڳ�Ա���������б���const����void Print() const������������thisָ������Ϊ��const ����* const����
*      ��ʾ���������޸Ķ���ĳ�Ա��������֤����ֻ������
*    - const������const���εĶ�����const Int b{30}������thisָ��Ϊ��const ����* const����ֻ�ܵ���const��Ա������
*      ��ֹ���÷�const��Ա������������������޸ģ���
* 2. �ؼ�����
*    - ��const����ɵ���const�ͷ�const��Ա������
*    - const����ֻ�ܵ���const��Ա������
*    - const��Ա�����ڲ����޸ĳ�Ա������Ҳ���ܵ��÷�const��Ա������
*
* �����������캯����Copy Constructor��
* 1. ���Ķ��壺
*    �������캯��������Ĺ��캯�������ڡ����Ѵ��ڵĶ����ʼ���¶��󡹣��﷨Ϊ��
*    ����(const ����& Դ����) : ��Ա����(Դ����.��Ա����) { ... }
*    ����������Ϊ��const ����&��������ֵ�����������޵ݹ鿽����ͬʱ֧�ֽ���const���󣩡�
* 2. ���ó�����
*    - �����ж����ʼ���¶�����Int c(b)��Int c = b����
*    - ������Ϊ����������ֵ���ݡ�ʱ����void func(Int it)������func(a)�´��a�����β�it����
*    - ��������ֵΪ����ʱ����NRVO�Ż��£��´���ֲ����󴴽���ʱ����ֵ����
* 3. ������Ĭ����Ϊ��
*    ��δ�ֶ����忽�����캯�������������Զ����ɡ�Ĭ�Ͽ������캯������ʵ�֡�ǳ�����������Ա��ֵ���ʺ��޶�̬��Դ���ࣩ��
*    ���������̬��Դ����new������ڴ棩�����ֶ����忽�����캯��ʵ�֡��������������Դ�ظ��ͷš�
*
* ������ֵ��������أ�Copy Assignment Operator��
* 1. ���Ķ��壺
*    ��ֵ������������ڡ��Ѵ��ڵĶ���֮�丳ֵ�����﷨Ϊ��
*    ����& operator=(const ����& Դ����) {
*        if (this != &Դ����) { // �ؼ����ж��Ը�ֵ�������ظ���������Դ����
*            ��Ա���� = Դ����.��Ա����; // ǳ����/����߼�
*        }
*        return *this; // �����������ã�֧����ʽ��ֵ����a = b = c��
*    }
* 2. �뿽�����������
*    - �������죺���ڡ���ʼ���¶��󡹣����󲻴���ʱ��������ʱ���Ƕ��󴴽�ʱ��
*    - ��ֵ���أ����ڡ����ж���ֵ���������Ѵ���ʱ��������ʱ���Ǹ�ֵ��䣨=��ʱ��
* 3. ������Ĭ����Ϊ��
*    ��δ�ֶ����壬����������Ĭ�ϸ�ֵ�������ʵ��ǳ����������̬��Դʱ���ֶ�ʵ������������ڴ�й©��
*
* �ġ��������������뿽��/��������
* �������漰�����������ݡ�����ֵʱ���ᴥ������������������������Ӧ���������̣�
* 1. ��������ֵ���ݣ����ÿ������촴���β� �� ����ִ�� �� ���������β�������
* 2. �������ض��󣺴����ֲ����� �� ���ÿ������촴����ʱ����ֵ �� �ֲ��������� �� ��ʱֵ��ֵ��Ŀ��������У��� ��ʱֵ������
* ��ע���ִ���������ͨ��NRVO����������ֵ�Ż���ʡ�Բ��ֿ������������࣬ʵ����������������������в��죬���߼�һ�£�
*******************************************************************************************************************/

// 1. ����1��const������const��Ա���������ƶ������Ȩ�ޣ�
class Int {
private:
    int value;
public:
    // ���캯��
    Int() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // const��Ա�����������޸ĳ�Ա�������ɱ�const�������
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
    int GetValue() const {
        return value;
    }

    // ��const��Ա���������޸ĳ�Ա���������ܱ�const�������
    void SetValue(int v) {
        value = v;
    }
};

void const_object_member_demo() {
    std::cout << "----- const������const��Ա���� -----" << std::endl;
    // ��const���󣺿ɵ���const�ͷ�const��Ա����
    Int a{ 10 };
    a.SetValue(20); // ���÷�const��Ա�������Ϸ���
    std::cout << "a.GetValue() = " << a.GetValue() << std::endl; // ����const��Ա�������Ϸ���
    a.Print(); // ����const��Ա�������Ϸ���

    // const����ֻ�ܵ���const��Ա����
    const Int b{ 30 };
    std::cout << "b.GetValue() = " << b.GetValue() << std::endl; // ����const��Ա�������Ϸ���
    b.Print(); // ����const��Ա�������Ϸ���
    // b.SetValue(100); // ����const�����ܵ��÷�const��Ա����

    std::cout << "\n" << std::endl;
}

// 2. ����2���������캯�������󿽱���ʼ���뺯������ֵ���ݣ�
class Int2 {
private:
    int value;
public:
    // ���캯��
    Int2() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int2(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // �ֶ����忽�����캯��������Ϊconst Int&������ݹ鿽����
    Int2(const Int2& it) : value(it.value) {
        std::cout << "Copy create Int (value=" << value << ") at " << this
            << " (from " << &it << ")" << std::endl;
    }

    // �����������۲��������ʱ��
    ~Int2() {
        std::cout << "Destroy Int (value=" << value << ") at " << this << std::endl;
    }

    // const��Ա��������ӡֵ
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
};
void copy_constructor_demo1() {
    std::cout << "----- �������캯���������ʼ���뺯�������� -----" << std::endl;
    // ����1�������ж����ʼ���¶��󣨵��ÿ������죩
    Int2 a{ 10 };
    Int2 b(a); // ����a��ʼ��b �� ���ÿ�������
    Int2 c = a; // �ȼ���Int c(a) �� ���ÿ������죨��ʼ�����Ǹ�ֵ��
    std::cout << "a.Print(): "; a.Print();
    std::cout << "b.Print(): "; b.Print();
    std::cout << "c.Print(): "; c.Print();

    // ����2��������Ϊ��������ֵ���ݣ����ÿ������촴���βΣ�
    auto func = [](Int2 it) { // �β�it��ֵ���ݣ��追��ʵ�δ���
        std::cout << "In func: ";
        it.Print();
        }; // �����������β�it����

    std::cout << "\nCall func(a):" << std::endl;
    func(a); // ����a�����β�it �� ���ÿ������죻��������it����

    std::cout << "\nMain end (a, b, c will destroy):" << std::endl;
    // main����������a��b��c��������
    std::cout << "\n" << std::endl;
}

// 3. ����3�����������븳ֵ����������𣨳�ʼ��vs��ֵ��
class Int3 {
private:
    int value;
public:
    // ���캯��
    Int3() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int3(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // �������캯������ʼ���¶���
    Int3(const Int3& it) : value(it.value) {
        std::cout << "Copy create Int (value=" << value << ") at " << this
            << " (from " << &it << ")" << std::endl;
    }

    // ��ֵ��������أ����ж���ֵ��
    Int3& operator=(const Int3& it) {
        if (this != &it) { // �ؼ����ж��Ը�ֵ������a = a�Ĵ���
            value = it.value;
            std::cout << "Assign: " << this << " = " << &it
                << " (new value=" << value << ")" << std::endl;
        }
        else {
            std::cout << "Self-assignment skipped (this == &it)" << std::endl;
        }
        return *this; // �����������ã�֧����ʽ��ֵ
    }

    // ��������
    ~Int3() {
        std::cout << "Destroy Int (value=" << value << ") at " << this << std::endl;
    }

    // ��ӡֵ
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
};
void copy_vs_assignment_demo() {
    std::cout << "----- �������� vs ��ֵ��������� -----" << std::endl;

    

    // 1. �������죺��ʼ���¶��󣨶��󲻴��ڣ�
    Int3 a{ 10 };
    Int3 b = a; // ��ʼ��b �� ���ÿ������죨�Ǹ�ֵ��
    std::cout << "After Int b = a (copy constructor):" << std::endl;
    std::cout << "a.Print(): "; a.Print();
    std::cout << "b.Print(): "; b.Print();

    // 2. ��ֵ���أ����ж���ֵ�������Ѵ��ڣ�
    Int3 c{ 20 };
    c = a; // c�Ѵ��ڣ���ֵ �� ���ø�ֵ�����
    std::cout << "\nAfter c = a (assignment operator):" << std::endl;
    std::cout << "c.Print(): "; c.Print();

    // 3. ��ʽ��ֵ��������ֵ���ط����������ã�
    Int3 d{ 30 };
    d = c = a; // �ȼ���d.operator=(c.operator=(a)) �� ��ʽ��ֵ
    std::cout << "\nAfter d = c = a (chained assignment):" << std::endl;
    std::cout << "d.Print(): "; d.Print();

    // 4. �Ը�ֵ�������Ը�ֵ�жϣ�����������
    std::cout << "\nCall a = a (self-assignment):" << std::endl;
    a = a;

    std::cout << "\nMain end (a, b, c, d will destroy):" << std::endl;
    std::cout << "\n" << std::endl;
}

// 4. ����4���������ض���Ŀ���������������ʱ����
class Int4 {
private:
    int value;
public:
    // ���캯��
    Int4() : value(0) {
        std::cout << "Default create Int (value=0) at " << this << std::endl;
    }
    Int4(int v) : value(v) {
        std::cout << "Create Int (value=" << value << ") at " << this << std::endl;
    }

    // �������캯��
    Int4(const Int4& it) : value(it.value) {
        std::cout << "Copy create Int (value=" << value << ") at " << this
            << " (from " << &it << ")" << std::endl;
    }

    // ��ֵ���������
    Int4& operator=(const Int4& it) {
        if (this != &it) {
            value = it.value;
            std::cout << "Assign: " << this << " = " << &it
                << " (new value=" << value << ")" << std::endl;
        }
        return *this;
    }

    // ��������
    ~Int4() {
        std::cout << "Destroy Int (value=" << value << ") at " << this << std::endl;
    }

    // ��ӡֵ
    void Print() const {
        std::cout << "Print value: " << value << std::endl;
    }
};
void return_object_copy_demo() {
    std::cout << "----- �������ض���Ŀ��������� -----" << std::endl;
    // ��������ֵΪ���󣨷�NRVO�Ż��µ����̣�
    //Int4 func(Int4 it) {
    //    std::cout << "In func: modify it's value" << std::endl;
    //    // ע�⣺�˴���Ҫ�޸�it����ȥ���βε�const������itΪ��const�����޸ģ�
    //    // ��ʵ�ʿ����У����������������޸ģ������const��const Int& it��
    //    // Ϊ��ʾ�������˴����޸ģ�ֱ�ӷ���
    //    std::cout << "In func: return it (create temp return value)" << std::endl;
    //    return it; // ����it������ʱ����ֵ �� ���ÿ������죻��������it����
    //}

    // ����1����������ֱֵ�ӳ�ʼ���¶��󣨿��ܴ���NRVO�Ż���ʡ����ʱֵ������
    std::cout << "\nTest 1: Int e = func(a):" << std::endl;
    Int4 a{ 10 };
    //Int4 e = func(a); // �÷���ֵ��ʼ��e �� ����ʡ����ʱֵ��ֱ�ӿ�����e
    //std::cout << "e.Print(): "; e.Print();

    // ����2����������ֵ��ֵ�����ж�����ʱֵ�ȴ������ٸ�ֵ�������ʱֵ������
    std::cout << "\nTest 2: Int f{20}; f = func(a):" << std::endl;
    Int4 f{ 20 };
    //f = func(a); // 1. ����a�����β�it��2. ����it������ʱ����ֵ��3. ��ʱֵ��ֵ��f��4. it����ʱֵ����
    std::cout << "f.Print(): "; f.Print();

    std::cout << "\nMain end (a, e, f will destroy):" << std::endl;
}

#if 0
int main() {
    const_object_member_demo();    // const������const��Ա����
    copy_constructor_demo1();      // �������죨��ʼ���뺯��������
    copy_vs_assignment_demo();     // �������� vs ��ֵ����
    return_object_copy_demo();     // �������ض���Ŀ���������
    return 0;
}
#endif