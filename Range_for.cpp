#include<iostream>
#include <vector>
using namespace std;
//��Χforѭ��

int main() {
    vector<int> nums = { 1, 2, 3, 4 };

    // ������&����Ԫ�أ��޸Ļ�Ӱ��ԭvector
    for (auto& num : nums) {
        num *= 2;  // ÿ��Ԫ�س���2
    }

    // �ٴα����鿴���
    for (auto num : nums) {
        cout << num << " ";  // �����2 4 6 8
    }
    return 0;
}


#if 0


int main() {
    vector<string> fruits = { "apple", "banana", "cherry" };

    // ����vector�е��ַ���
    for (const auto& fruit : fruits) {  // ��const&���⿽�������Ч��
        cout << fruit << " ";  // �����apple banana cherry
    }
    return 0;
}

int main() {
    int arr[] = { 10, 20, 30, 40, 50 };

    // ���������е�ÿ��Ԫ�أ���auto�Զ��Ƶ�����Ϊint��
    for (auto num : arr) {
        cout << num << " ";  // �����10 20 30 40 50
    }
    return 0;
}
#endif
