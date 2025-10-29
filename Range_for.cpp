#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>

/*******************************************************************************************************************
* 范围for循环（Range-based for Loop）
* 定义：范围for循环是C++11引入的语法糖，用于**遍历可迭代对象的所有元素**（无需手动控制索引或迭代器），
*      自动从对象的起始位置遍历到结束位置，简化传统for循环的代码，提高可读性。
*
* 核心语法：
* for (循环变量 : 可迭代对象) {
*     // 循环体（循环变量代表当前遍历的元素）
* }
*
* 一、核心规则
* 1. 可迭代对象要求：
*    - 必须是“可遍历的容器或序列”，包括：
*      - 原生数组（如int arr[5]）；
*      - STL容器（如vector、string、list等，需支持begin()和end()成员函数）；
*      - 自定义容器（需实现begin()和end()，返回合法迭代器）。
* 2. 循环变量的类型选择（关键！影响功能和效率）：
*    - （值传递）auto 变量名：遍历元素的副本，修改变量不影响原对象，适合简单类型（int、char）；
*    - （引用传递）auto& 变量名：直接引用原元素，修改变量会同步修改原对象，适合需修改元素的场景；
*    - （const引用）const auto& 变量名：引用原元素但不可修改，避免拷贝（尤其适合大对象如string、自定义类），提高效率。
* 3. 遍历本质：
*    范围for循环底层会调用可迭代对象的 begin() 和 end() 函数获取迭代器，每次循环通过迭代器取元素，
*    等价于“for (auto it = obj.begin(); it != obj.end(); ++it) { 循环变量 = *it; ... }”。
* 4. 注意事项：
*    - 遍历期间**禁止修改可迭代对象的结构**（如vector的push_back/pop_back、数组的大小变更），否则迭代器失效，导致未定义行为；
*    - 不可遍历空对象（如空vector），但空对象不会报错，循环体不会执行；
*    - 原生数组需明确大小（不能用数组名退化的指针，如int* arr无法遍历）。
*
* 二、常见使用场景
* 1. 遍历原生数组：无需手动计算数组长度，自动遍历所有元素；
* 2. 遍历STL容器：简化vector、string、map等容器的遍历代码；
* 3. 读取元素：用const auto&避免拷贝，适合大对象；
* 4. 修改元素：用auto&直接操作原元素，无需通过索引或迭代器间接访问。
*******************************************************************************************************************/

// 1. 场景1：范围for循环遍历原生数组（值传递）
void range_for_array() {
    std::cout << "----- 范围for循环遍历原生数组 -----" << std::endl;
    int arr[] = { 10, 20, 30, 40, 50 }; // 原生数组（明确大小）

    // 用auto值传递：遍历数组元素的副本，适合简单int类型
    std::cout << "数组元素: ";
    for (auto num : arr) {
        std::cout << num << " "; // 自动遍历所有元素，无需索引
    }
    std::cout << "\n" << std::endl;
}

// 2. 场景2：范围for循环遍历vector并修改元素（引用传递）
void range_for_vector_modify() {
    using namespace std;
    cout << "----- 范围for循环遍历vector并修改元素 -----" << endl;
    vector<int> nums = { 1, 2, 3, 4 }; // STL容器vector

    // 用auto&引用传递：直接操作原元素，修改会同步到vector
    cout << "修改前vector: ";
    for (auto num : nums) {
        cout << num << " ";
    }
    cout << endl;

    // 修改元素（每个元素乘以2）
    for (auto& num : nums) {
        num *= 2; // 引用传递，修改num即修改vector中的原元素
    }

    // 遍历修改后的结果
    cout << "修改后vector: ";
    for (auto num : nums) {
        cout << num << " ";
    }
    cout << "\n" << endl;
}

// 3. 场景3：范围for循环遍历vector<string>（const引用，避免拷贝）
void range_for_vector_string() {
    using namespace std;
    cout << "----- 范围for循环遍历vector<string>（const引用） -----" << endl;
    vector<string> fruits = { "apple", "banana", "cherry" }; // 元素为string（大对象）

    // 用const auto&：引用原元素但不可修改，避免string的拷贝开销（提高效率）
    cout << "vector中的字符串: ";
    for (const auto& fruit : fruits) {
        cout << fruit << " "; // 直接访问字符串，无需迭代器或索引
    }
    cout << "\n" << endl;
}

// 4. 场景4：范围for循环的注意事项（禁止修改容器结构）
void range_for_warning() {
    using namespace std;
    cout << "----- 范围for循环注意事项（禁止修改容器结构） -----" << endl;
    vector<int> nums = { 1, 2, 3 };

    // 错误示例（注释掉避免编译报错）：遍历期间修改容器大小（push_back），导致迭代器失效
    // for (auto num : nums) {
    //     nums.push_back(4); // 未定义行为：容器大小变更，迭代器end()失效
    // }

    // 正确做法：先遍历，后修改；或用传统for循环控制迭代器
    cout << "遍历后修改容器（正确操作）: " << endl;
    for (auto num : nums) {
        cout << num << " ";
    }
    nums.push_back(4); // 遍历结束后再修改容器
    cout << "\n添加元素4后，vector大小: " << nums.size() << endl;
}

#if 0
int main() {
    range_for_array();               // 遍历原生数组
    range_for_vector_modify();       // 遍历vector并修改元素
    range_for_vector_string();       // 遍历vector<string>（const引用）
    range_for_warning();             // 范围for循环注意事项演示
    return 0;
}
#endif