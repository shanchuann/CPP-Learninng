#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>

/*******************************************************************************************************************
* C++ std::vector 核心知识点总结
* std::vector是C++标准库中最常用的动态数组容器，封装了连续内存的自动管理，支持动态扩容、随机访问、高效的尾部插入/删除，是替代C风格数组的首选。
* 核心内容包括：vector基本特性、构造函数、元素访问、容量操作、修改操作、迭代器与失效规则。
*
* 一、vector基本特性
* 1. 核心定义：动态数组容器，元素在内存中连续存储；
* 2. 特点：
*    - 支持随机访问（O(1)时间复杂度）；
*    - 尾部插入/删除高效（平均O(1)，扩容时O(n)）；
*    - 中间插入/删除低效（O(n)，需移动元素）；
*    - 自动管理内存，无需手动分配/释放；
* 3. 头文件：#include <vector>。
*
* 二、构造函数
* 1. 默认构造：创建空vector；
*    语法：std::vector<T> vec;
* 2. 填充构造：创建包含n个值为val的元素；
*    语法：std::vector<T> vec(n, val); 或 std::vector<T> vec(n);（val为默认值）
* 3. 范围构造：从迭代器区间复制元素；
*    语法：std::vector<T> vec(begin, end);
* 4. 初始化列表构造（C++11）：用初始化列表创建；
*    语法：std::vector<T> vec = {v1, v2, v3, ...};
* 5. 拷贝构造：复制另一个vector；
*    语法：std::vector<T> vec(other_vec);
* 6. 移动构造（C++11）：转移另一个vector的所有权，不拷贝；
*    语法：std::vector<T> vec(std::move(other_vec));（other_vec变为空）。
*
* 三、元素访问
* 1. operator[]：下标访问，不检查越界；
*    语法：vec[index]; 特点：O(1)，高效但不安全；
* 2. at()：带边界检查的访问；
*    语法：vec.at(index); 特点：O(1)，越界抛出std::out_of_range异常；
* 3. front()/back()：访问首尾元素；
*    语法：vec.front(); vec.back(); 特点：O(1)，空vector调用未定义行为；
* 4. data()：获取底层数组的指针（C++11）；
*    语法：vec.data(); 特点：可用于与C接口交互。
*
* 四、容量操作
* 1. size()：返回当前元素个数；
*    语法：vec.size();
* 2. capacity()：返回当前容量（不扩容最多能存的元素数）；
*    语法：vec.capacity();
* 3. empty()：判断是否为空；
*    语法：vec.empty();
* 4. reserve(n)：预分配至少n个元素的容量，只改capacity，不改size；
*    语法：vec.reserve(n); 特点：避免频繁扩容，提高性能；
* 5. resize(n)：调整size为n，会创建/销毁对象；
*    语法：vec.resize(n); 或 vec.resize(n, val);（新增元素用val初始化）
* 6. shrink_to_fit()：请求将capacity收缩到与size相等（C++11，非强制）；
*    语法：vec.shrink_to_fit();
* 7. 强制缩容技巧：利用临时对象交换内存；
*    语法：std::vector<T>(vec).swap(vec); 原理：创建容量=size的临时拷贝，交换指针后临时对象销毁释放旧内存。
*
* 五、修改操作
* 1. push_back：在尾部添加元素；
*    语法：vec.push_back(val);
* 2. emplace_back：在尾部直接构造元素（C++11）；
*    语法：vec.emplace_back(args...); 特点：对于复杂对象，比push_back更高效，避免临时对象的拷贝；
* 3. pop_back：删除尾部元素；
*    语法：vec.pop_back();
* 4. insert：在指定位置插入元素；
*    语法：vec.insert(pos, val); 特点：返回新元素的迭代器，O(n)复杂度；
* 5. erase：删除指定位置或区间的元素；
*    语法：vec.erase(pos); 或 vec.erase(begin, end); 特点：返回下一个元素的迭代器，O(n)复杂度；
* 6. assign：赋值新内容，会清空原有内容；
*    语法：vec.assign(n, val); 或 vec.assign(init_list); 或 vec.assign(begin, end);
* 7. swap：交换两个vector的内容；
*    语法：vec.swap(other_vec); 特点：O(1)，只交换指针，不拷贝元素。
*
* 六、迭代器与失效规则
* 1. 迭代器类型：
*    - begin()/end()：正向迭代器；
*    - rbegin()/rend()：反向迭代器；
* 2. 遍历方式：
*    - 迭代器遍历：for (auto it = vec.begin(); it != vec.end(); ++it);
*    - 范围for循环（C++11）：for (const auto& val : vec);（本质是迭代器的语法糖）
* 3. 迭代器失效规则：
*    - 扩容操作（如push_back触发扩容）：所有迭代器、引用、指针失效；
*    - 插入操作：插入点及之后的迭代器、引用、指针失效；
*    - 删除操作：删除点及之后的迭代器、引用、指针失效；
*    - 注意：失效的迭代器不能再使用，否则触发未定义行为。
*******************************************************************************************************************/

// 全局定义（用于emplace_back示例的Person类）
class Person {
public:
    Person(std::string name, int age) : name_(name), age_(age) {}
private:
    std::string name_;
    int age_;
};

void testVectorConstructors() {
    std::cout << "\n=== 测试 vector 构造函数 ===" << std::endl;

    // 1. 默认构造：创建空的vector
    std::vector<int> vec1;
    std::cout << "vec1 size: " << vec1.size() << ", capacity: " << vec1.capacity() << std::endl;

    // 2. 填充构造：创建包含n个值为val的元素
    std::vector<int> vec2(10, 23); // 10个int，每个值都是23
    std::vector<int> vec3(10);      // 10个int，每个值都是0（默认构造）
    std::cout << "vec2[0]: " << vec2[0] << ", vec3[0]: " << vec3[0] << std::endl;

    // 3. 范围构造：从迭代器区间复制元素
    int arr[] = { 1, 2, 3, 4, 5 };
    std::vector<int> vec4(arr, arr + 5); // 从C数组复制
    std::cout << "vec4[2]: " << vec4[2] << std::endl;

    // 4. 初始化列表构造（C++11）
    std::vector<int> vec5 = { 1, 2, 3, 4, 5, 6, 7 };
    std::cout << "vec5[3]: " << vec5[3] << std::endl;

    // 5. 拷贝构造：复制另一个vector
    std::vector<int> vec6(vec5);
    std::cout << "vec6[4]: " << vec6[4] << std::endl;

    // 6. 移动构造（C++11）：转移另一个vector的所有权，不拷贝
    std::vector<int> vec7(std::move(vec6)); // vec6现在变为空
    std::cout << "vec7[5]: " << vec7[5] << ", vec6 size: " << vec6.size() << std::endl;
}

void testVectorElementAccess() {
    std::cout << "\n=== 测试 vector 元素访问 ===" << std::endl;

    std::vector<int> vec = { 10, 20, 30, 40, 50 };

    // 1. operator[]：下标访问，不检查越界
    int a = vec[0]; // 10
    std::cout << "vec[0]: " << a << std::endl;
    // vec[100]; // 未定义行为！可能导致程序崩溃或数据混乱

    // 2. at()：带边界检查的访问
    int b = vec.at(1); // 20
    std::cout << "vec.at(1): " << b << std::endl;
    // vec.at(100); // 抛出 std::out_of_range 异常，安全但稍慢

    // 3. front() 和 back()：访问首尾元素
    int first = vec.front(); // 10
    int last = vec.back();   // 50
    std::cout << "vec.front(): " << first << ", vec.back(): " << last << std::endl;

    // 4. data()：获取底层数组的指针（C++11）
    int* p = vec.data();
    std::cout << "*(p + 2): " << *(p + 2) << std::endl; // 30，可用于与C接口交互
}

void testVectorCapacity() {
    std::cout << "\n=== 测试 vector 容量操作 ===" << std::endl;

    std::vector<int> vec;

    // size()：返回当前元素个数
    // capacity()：返回当前容量（不扩容最多能存的元素数）
    // empty()：判断是否为空
    std::cout << "初始 size: " << vec.size() << ", capacity: " << vec.capacity() << ", empty: " << std::boolalpha << vec.empty() << std::endl;

    // reserve(n)：预分配至少n个元素的容量，只改capacity，不改size
    vec.reserve(20);
    std::cout << "reserve(20)后 size: " << vec.size() << ", capacity: " << vec.capacity() << std::endl;
    // 注意：reserve分配的内存未初始化，直接访问 vec[0] 是未定义行为！

    // resize(n)：调整size为n，会创建/销毁对象
    vec.resize(10);
    std::cout << "resize(10)后 size: " << vec.size() << ", capacity: " << vec.capacity() << std::endl;
    // 如果n < 当前size，会删除末尾的元素
    // 如果n > 当前size，会在末尾添加默认构造的元素

    // resize(n, val)：调整size为n，新增元素用val初始化
    vec.resize(15, 100); // 新增的5个元素都是100
    std::cout << "resize(15, 100)后 size: " << vec.size() << ", capacity: " << vec.capacity() << std::endl;

    // shrink_to_fit()：请求将capacity收缩到与size相等（C++11）
    vec.shrink_to_fit(); // 非强制，编译器可能忽略
    std::cout << "shrink_to_fit()后 size: " << vec.size() << ", capacity: " << vec.capacity() << std::endl;

    // 经典缩容技巧（强制）：利用临时对象交换内存
    std::vector<int>(vec).swap(vec);
    // 原理：创建一个容量=size的临时拷贝，交换指针后临时对象销毁释放旧内存
    std::cout << "强制缩容后 size: " << vec.size() << ", capacity: " << vec.capacity() << std::endl;
}

void testVectorModify() {
    std::cout << "\n=== 测试 vector 修改操作 ===" << std::endl;

    std::vector<int> vec;
    vec.reserve(10); // 好习惯：先预分配

    // 1. push_back：在尾部添加元素
    vec.push_back(1);
    vec.push_back(2);
    std::cout << "push_back后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;

    // 2. emplace_back：在尾部直接构造元素（C++11）
    // 对于复杂对象，emplace_back 比 push_back 更高效，因为它避免了临时对象的拷贝
    std::vector<Person> people;
    // push_back 需要先构造临时对象，再拷贝/移动到容器中
    people.push_back(Person("Alice", 20));
    // emplace_back 直接在容器内存中构造对象，零拷贝
    people.emplace_back("Bob", 25); // 直接传构造函数参数
    std::cout << "people size: " << people.size() << std::endl;

    // 3. pop_back：删除尾部元素
    vec.pop_back();
    std::cout << "pop_back后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;

    // 4. insert：在指定位置插入元素（返回新元素的迭代器）
    auto it = vec.insert(vec.begin(), 0); // 在开头插入0，O(n)复杂度
    std::cout << "insert(0)后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;

    // 5. erase：删除指定位置或区间的元素（返回下一个元素的迭代器）
    it = vec.erase(vec.begin()); // 删除开头元素
    std::cout << "erase(begin())后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;

    // 6. assign：赋值新内容，会清空原有内容
    vec.assign(5, 100); // 变为5个100
    std::cout << "assign(5, 100)后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;
    vec.assign({ 1, 2, 3, 4, 5 }); // 变为初始化列表中的内容
    std::cout << "assign(init_list)后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;

    // 7. swap：交换两个vector的内容（O(1)，只交换指针）
    std::vector<int> other = { 10, 20, 30 };
    vec.swap(other);
    std::cout << "swap后 vec: ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << std::endl;
}

void testVectorIterator() {
    std::cout << "\n=== 测试 vector 迭代器与遍历 ===" << std::endl;

    std::vector<int> vec = { 1, 2, 3, 4, 5 };

    // 正向遍历
    std::cout << "正向遍历: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 反向遍历
    std::cout << "反向遍历: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 范围for循环（C++11），本质是迭代器的语法糖
    std::cout << "范围for循环: ";
    for (const auto& val : vec) { // 推荐加const&，避免拷贝
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void testVectorIteratorInvalidation() {
    std::cout << "\n=== 测试 vector 迭代器失效 ===" << std::endl;

    // 1. 扩容操作导致所有迭代器失效（注释掉避免未定义行为）
    // std::vector<int> vec1;
    // vec1.reserve(3);
    // vec1 = { 1, 2, 3 };
    // auto it1 = vec1.begin();
    // std::cout << *it1 << std::endl; // 正常
    // vec1.push_back(4); // 触发扩容（假设capacity从3变为6）
    // std::cout << *it1 << std::endl; // 崩溃！it1指向的旧内存已被释放

    // 2. 插入操作导致插入点及之后的迭代器失效（注释掉避免未定义行为）
    // std::vector<int> vec2 = { 1, 2, 3, 4, 5 };
    // auto it2 = vec2.begin() + 2; // 指向元素3
    // auto it_end2 = vec2.end();
    // vec2.insert(it2, 100); // 在3前面插入100
    // 此时 it2 和 it_end2 都失效了！

    // 3. 删除操作导致删除点及之后的迭代器失效（注释掉避免未定义行为）
    // std::vector<int> vec3 = { 1, 2, 3, 4, 5 };
    // auto it3 = vec3.begin() + 2; // 指向3
    // vec3.erase(it3); // 删除3
    // 此时 it3 失效了，原本指向4、5的迭代器也失效了

    std::cout << "迭代器失效规则：扩容/插入/删除操作会导致部分或全部迭代器失效，失效的迭代器不能再使用！" << std::endl;
}

#if 0
int main() {
    testVectorConstructors();
    testVectorElementAccess();
    testVectorCapacity();
    testVectorModify();
    testVectorIterator();
    testVectorIteratorInvalidation();

    return 0;
}
#endif