#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
/*******************************************************************************************************************
* C++��̬�ڴ���䣨Dynamic Memory Allocation��
* ��̬�ڴ������ָ�ڳ�������ʱ�������������ͷ��ڴ�Ļ��ƣ���Ҫͨ��C++��new/delete�������C���Ե�malloc/free����
* �Լ������placement new����λnew��ʵ�֡�
*
* һ��new��delete�������C++�Ƽ���
* 1. ��������ķ������ͷ�
*    - ���䣺new ����(��ʼ��ֵ) �� �Զ������ڴ��С�������ڴ沢��ʼ������������ֱ�Ӹ�ֵ�������͵��ù��캯����
*    - �ͷţ�delete ָ�� �� �ͷ��ڴ棨�������ȵ�������������
*
* 2. ����ķ������ͷ�
*    - ���䣺new ����[�����С]{��ʼ���б�} �� ���������ڴ棬֧�ֳ�ʼ����C++11�����ϣ�
*    - �ͷţ�delete[] ָ�� �� ������new[]��ԣ�ȷ��������ÿ��Ԫ�أ������ͣ�����������������
*
* ����placement new����λnew��
* ���壺���ѷ�����ڴ�ռ��Ϲ�����󣬲��������ڴ棬�����ö���Ĺ��캯����
* �﷨��new(�ڴ��ַ) ����(��ʼ��ֵ)
* �ص㣺
*    - �ڴ�����ǰ���䣨����ջ���ѻ�ȫ���ڴ棩��
*    - ������delete�ͷ��ڴ棨���ڴ治��new����ģ�������������ʽ��������������
*    - �������ڴ�ء��̶��������ȳ���������ڴ渴��Ч�ʡ�
*
* ����malloc��free��C���Լ��ݣ�
* 1. ���䣺void* malloc(�ֽ���) �� ������ָ����С��ԭʼ�ڴ棬����ʼ��������void*����ǿת��
* 2. �ͷţ�free(ָ��) �� ���ͷ��ڴ棬�����������������������з��գ�
* 3. ��new�ĺ�������malloc�Ǻ����������ڴ���䣻new�������������ڴ����+��ʼ�������캯������
*
* �ؼ�ԭ��
* - ���ʹ�ã�new �� delete��new[] �� delete[]��malloc �� free�������ã������ڴ�й©�������
* - ����Ұָ�룺�ͷ��ڴ��ָ����Ϊnullptr����ֹ�����ͷ�
*******************************************************************************************************************/

// 1. ���������new/deleteʹ��
void singleObjectDemo() {
    cout << "----- ���������new/delete -----" << endl;
    // new�Ĺ��̣�1.����int��С 2.�����ڴ� 3.��ʼ��ֵ10 4.����int*
    int* pInt = new int(10);  // ���䲢��ʼ������int
    cout << "new int(10) = " << *pInt << endl;

    // ֱ�ӵ���operator new���ײ��ڴ���亯��������malloc��
    int* pOpNew = (int*)::operator new(sizeof(int));  // �������ڴ棬����ʼ��
    *pOpNew = 20;  // �ֶ���ʼ��
    cout << "::operator new�����int = " << *pOpNew << endl;

    // �ͷ��ڴ�
    delete pInt;       // �ͷ�new������ڴ�
    ::operator delete(pOpNew);  // �ͷ�operator new������ڴ棨��Ӧ������
    pInt = nullptr;    // ����Ұָ��
    pOpNew = nullptr;
    cout << endl;
}

// 2. �����new[]/delete[]ʹ��
void arrayDemo() {
    cout << "----- �����new[]/delete[] -----" << endl;
    size_t size = 10;

    // ����δ��ʼ����int����
    int* arr = new int[size];  // new[]��������
    for (size_t i = 0; i < size; i++) {
        arr[i] = i + 1;  // �ֶ���ʼ��
    }
    cout << "δ��ʼ�����飨�ֶ���ֵ��: ";
    for (size_t i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // ���䲢��ʼ����int���飨C++11�б��ʼ����
    int* arrInit = new int[size] {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    cout << "��ʼ�����飨�б�ֵ��: ";
    for (size_t i = 0; i < size; i++) {
        cout << arrInit[i] << " ";
    }
    cout << endl;

    // C���Ե�malloc/free�������飨����ʹ�ã�
    int* arrMalloc = (int*)malloc(size * sizeof(int));  // ����ʽ�����ֽ���������void*
    if (arrMalloc != nullptr) {  // mallocʧ�ܷ���NULL������
        free(arrMalloc);  // �ͷ�malloc������ڴ�
    }

    // �ͷ������ڴ棨������delete[]����new[]��ԣ�
    delete[] arr;
    delete[] arrInit;
    arr = nullptr;
    arrInit = nullptr;
    cout << endl;
}

// 3. placement new����λnew��ʹ��
void placementNewDemo() {
    cout << "----- placement new����λnew�� -----" << endl;
    // ��ջ�Ϸ���һ�黺������Ҳ�����Ƕѻ�ȫ���ڴ棩
    char buffer[128];  // ջ�ϵ��ڴ�飬��С128�ֽ�
    cout << "��������ʼ��ַ: " << (void*)buffer << endl;

    // �ڻ�����ָ��λ�ù���int�����������ڴ棬����ʼ����
    int* p1 = new(buffer) int(10);  // ��buffer��ʼλ�ù���int
    // ƫ��sizeof(int)λ�ã�����ڶ���int
    int* p2 = new(buffer + sizeof(int)) int(20);
    // ƫ��sizeof(double)λ�ã�doubleͨ��8�ֽڣ�������double
    double* p3 = new(buffer + sizeof(double)) double(3.14);

    // ��֤����
    cout << "buffer��ʼλ�õ�int: " << *p1 << "����ַ: " << (void*)p1 << "��" << endl;
    cout << "ƫ��int��С��int: " << *p2 << "����ַ: " << (void*)p2 << "��" << endl;
    cout << "ƫ��double��С��double: " << *p3 << "����ַ: " << (void*)p3 << "��" << endl;

    // ע�⣺placement new�������ڴ棬������delete�ͷ�buffer
    // ��Ϊ�����ͣ�����ʽ���������������������Ϳɺ��ԣ�
    // p1->~int();  // �����������������壬��ʡ��
    cout << endl;
}

// 4. new��malloc�ĺ�������Ա�
void newVsMalloc() {
    cout << "----- new��malloc�ĺ������� -----" << endl;
    // new�����ԣ��Զ������С����������ָ�롢��ʼ�������캯����
    int* pNew = new int(12);
    // malloc�����ԣ�����ʽָ����С������void*����ǿת��������ʼ��
    int* pMalloc = (int*)malloc(sizeof(int));
    *pMalloc = 12;  // �ֶ���ʼ��

    cout << "new�����int: " << *pNew << endl;
    cout << "malloc�����int: " << *pMalloc << endl;

    // �ͷţ�new��Ӧdelete��malloc��Ӧfree
    delete pNew;    // �ͷ�+�������ͣ�����
    free(pMalloc);  // ���ͷ�
    pNew = nullptr;
    pMalloc = nullptr;
}

#if 0
int main() {
    singleObjectDemo();    // ���������new/delete
    arrayDemo();           // �����new[]/delete[]
    placementNewDemo();    // placement new����λnew��
    newVsMalloc();         // new��malloc������Ա�
    return 0;
}
#endif