#include<iostream>
#include <vector>
using namespace std;
//范围for循环

int main() {
    vector<int> nums = { 1, 2, 3, 4 };

    // 用引用&接收元素，修改会影响原vector
    for (auto& num : nums) {
        num *= 2;  // 每个元素乘以2
    }

    // 再次遍历查看结果
    for (auto num : nums) {
        cout << num << " ";  // 输出：2 4 6 8
    }
    return 0;
}


#if 0


int main() {
    vector<string> fruits = { "apple", "banana", "cherry" };

    // 遍历vector中的字符串
    for (const auto& fruit : fruits) {  // 用const&避免拷贝，提高效率
        cout << fruit << " ";  // 输出：apple banana cherry
    }
    return 0;
}

int main() {
    int arr[] = { 10, 20, 30, 40, 50 };

    // 遍历数组中的每个元素（用auto自动推导类型为int）
    for (auto num : arr) {
        cout << num << " ";  // 输出：10 20 30 40 50
    }
    return 0;
}
#endif
