#define _CRT_SECURE_NO_WARNINGS
#include<deque>
#include<iostream>
#include<iomanip>
#include<vector>
#include<iterator>

/*******************************************************************************************************************
* C++ std::deque 核心知识点总结
* std::deque（double-ended queue，双端队列）是C++标准库中的动态容器，采用**分段连续内存**结构，支持随机访问、高效的头尾增删，是vector的重要补充。
* 核心内容包括：deque基本特性、构造函数、元素访问、修改操作（重点是头部增删O(1)）、容量操作、迭代器与失效规则、与vector的转换。
*
* 一、deque基本特性
* 1. 核心定义：双端队列容器，采用**分段连续内存**（由多个固定大小的缓冲区组成，通过中控器管理）；
* 2. 特点：
*    - 支持随机访问（O(1)时间复杂度，通过中控器计算缓冲区位置）；
*    - 头部/尾部插入/删除高效（平均O(1)，仅需调整中控器或缓冲区指针，无需移动大量元素）；
*    - 中间插入/删除低效（O(n)，需移动元素）；
*    - 无 `capacity()`/`reserve()`（分段内存不需要预分配连续空间）；
*    - 迭代器失效规则比vector更严格；
* 3. 头文件：#include <deque>。
*
* 二、构造函数
* 1. 默认构造：创建空deque；
*    语法：std::deque<T> dq;
* 2. 填充构造：创建包含n个值为val的元素；
*    语法：std::deque<T> dq(n, val); 或 std::deque<T> dq(n);（val为默认值）
* 3. 范围构造：从迭代器区间复制元素；
*    语法：std::deque<T> dq(begin, end);
* 4. 初始化列表构造（C++11）：用初始化列表创建；
*    语法：std::deque<T> dq = {v1, v2, v3, ...};
* 5. 拷贝构造：复制另一个deque；
*    语法：std::deque<T> dq(other_dq);
* 6. 移动构造（C++11）：转移另一个deque的所有权，不拷贝；
*    语法：std::deque<T> dq(std::move(other_dq));（other_dq变为空）。
*
* 三、元素访问
* 1. operator[]：下标访问，不检查越界；
*    语法：dq[index]; 特点：O(1)，高效但不安全；
* 2. at()：带边界检查的访问；
*    语法：dq.at(index); 特点：O(1)，越界抛出std::out_of_range异常；
* 3. front()/back()：访问首尾元素；
*    语法：dq.front(); dq.back(); 特点：O(1)，空deque调用未定义行为；
* 4. 迭代器访问：通用遍历方式，兼容STL算法；
*    语法：std::deque<T>::iterator it = dq.begin();
* 5. 范围for循环（C++11）：迭代器的语法糖，最简洁；
*    语法：for (const auto& val : dq);
* 6. 注意：deque**没有data()**（因为是分段连续内存，无单一连续底层数组）。
*
* 四、修改操作
* 1. 头部增删（deque专属O(1)操作）：
*    - emplace_front：在头部直接构造元素（C++11）；
*      语法：dq.emplace_front(args...);
*    - push_front：在头部添加元素；
*      语法：dq.push_front(val);
*    - pop_front：删除头部元素；
*      语法：dq.pop_front();
* 2. 尾部增删（和vector用法一致）：
*    - emplace_back：在尾部直接构造元素（C++11）；
*      语法：dq.emplace_back(args...);
*    - push_back：在尾部添加元素；
*      语法：dq.push_back(val);
*    - pop_back：删除尾部元素；
*      语法：dq.pop_back();
* 3. 中间插入/删除（效率低，尽量少用）：
*    - insert：在指定位置插入元素；
*      语法：dq.insert(pos, val); 特点：返回新元素的迭代器，O(n)复杂度；
*    - erase：删除指定位置或区间的元素；
*      语法：dq.erase(pos); 或 dq.erase(begin, end); 特点：返回下一个元素的迭代器，O(n)复杂度；
* 4. 批量操作：
*    - assign：赋值新内容，会清空原有内容；
*      语法：dq.assign(n, val); 或 dq.assign(init_list); 或 dq.assign(begin, end);
*    - clear：清空所有元素；
*      语法：dq.clear();
*    - swap：交换两个deque的内容；
*      语法：dq.swap(other_dq); 特点：O(1)，仅交换中控器和缓冲区指针，不拷贝元素。
*
* 五、容量操作
* 1. size()：返回当前元素个数；
*    语法：dq.size();
* 2. empty()：判断容器是否为空，比size()==0更高效；
*    语法：dq.empty();
* 3. max_size()：返回容器理论上能容纳的最大元素个数；
*    语法：dq.max_size();
* 4. resize(n)：调整元素数量；
*    语法：dq.resize(n); 或 dq.resize(n, val);（新增元素用val初始化）
* 5. shrink_to_fit()：请求释放未使用的缓冲区，减少内存占用（C++11，非强制）；
*    语法：dq.shrink_to_fit();
* 6. 注意：deque**没有capacity()/reserve()**（分段内存不需要预分配连续空间）。
*
* 六、迭代器与失效规则
* 1. 迭代器类型：
*    - begin()/end()：正向迭代器；
*    - rbegin()/rend()：反向迭代器；
* 2. 迭代器失效规则（比vector更严格）：
*    - 头部/尾部插入：**所有迭代器、引用、指针失效**；
*    - 中间插入：插入点及之后的迭代器、引用、指针失效；
*    - 中间删除：删除点及之后的迭代器、引用、指针失效；
*    - 注意：失效的迭代器不能再使用，否则触发未定义行为；
* 3. 循环删除正确写法：利用erase返回下一个有效迭代器。
*
* 七、与vector的转换
* 1. deque转vector：
*    - 方式1：范围构造直接转换；
*      语法：std::vector<T> vec(dq.begin(), dq.end());
*    - 方式2：预分配内存+assign，避免vector扩容；
*      语法：std::vector<T> vec; vec.reserve(dq.size()); vec.assign(dq.begin(), dq.end());
*    - 方式3：移动语义转换，零拷贝（原deque失效）；
*      语法：std::vector<T> vec(std::make_move_iterator(dq.begin()), std::make_move_iterator(dq.end()));
* 2. vector转deque：
*    - 范围构造直接转换；
*      语法：std::deque<T> dq(vec.begin(), vec.end());
*******************************************************************************************************************/

void PrintDeque(const std::deque<int>& dq, const std::string& desc) {
    std::cout << desc << " | 元素数量: " << std::setw(2) << dq.size() << " | 元素: ";
    for (int x : dq) std::cout << std::setw(3) << x;
    std::cout << "\n";
}

void testDequeConstructors() {
    std::cout << "\n=== 测试 deque 构造函数 ===" << std::endl;

    // 1. 默认构造：空deque
    std::deque<int> dq1;
    PrintDeque(dq1, "默认构造");

    // 2. 填充构造：指定数量默认值/指定值
    std::deque<int> dq2(5);
    std::deque<int> dq3(5, 23);
    PrintDeque(dq2, "填充构造(默认值)");
    PrintDeque(dq3, "填充构造(指定值)");

    // 3. 范围构造：从数组复制
    int arr[] = { 12, 23, 34, 45, 56 };
    std::deque<int> dq4(arr, arr + 5);
    PrintDeque(dq4, "范围构造(数组)");

    // 4. 初始化列表构造
    std::deque<int> dq5 = { 1, 2, 3, 4, 5, 6, 7 };
    PrintDeque(dq5, "初始化列表构造");

    // 5. 拷贝构造与移动构造
    std::deque<int> dq6(dq5);
    PrintDeque(dq6, "拷贝构造");
    std::deque<int> dq7(std::move(dq6)); // dq6现在变为空
    PrintDeque(dq7, "移动构造");
    PrintDeque(dq6, "移动后原dq6");
}

void testDequeElementAccess() {
    std::cout << "\n测试 deque 元素访问" << std::endl;

    std::deque<int> dq;
    for (int i = 0; i < 5; ++i) {
        dq.emplace_back(i); // 尾部构造元素0-4
    }
    int n = dq.size();

    // 1. operator[] 下标访问：O(1)，不检查越界
    std::cout << "operator[] 访问: ";
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(3) << dq[i];
    }
    std::cout << "\n";
    // 注意：dq[10] 是未定义行为，不会报错但可能导致程序崩溃

    // 2. at() 成员函数：O(1)，带边界检查
    std::cout << "at() 访问:       ";
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(3) << dq.at(i);
    }
    std::cout << "\n";
    // 注意：dq.at(10) 会抛出 std::out_of_range 异常，安全但有轻微性能开销

    // 3. 迭代器访问：通用遍历方式，兼容STL算法
    std::cout << "迭代器访问:      ";
    std::deque<int>::iterator it = dq.begin();
    for (; it != dq.end(); ++it) {
        std::cout << std::setw(3) << *it;
    }
    std::cout << "\n";

    // 4. 范围for循环（C++11）：迭代器的语法糖，最简洁
    std::cout << "范围for访问:     ";
    for (const auto& x : dq) { // 推荐加const&，避免拷贝
        std::cout << std::setw(3) << x;
    }
    std::cout << "\n";

    // 首尾元素专属访问
    std::cout << "首元素 front(): " << dq.front() << "\n";
    std::cout << "尾元素 back():  " << dq.back() << "\n";
}

void testDequeModify() {
    std::cout << "\n测试 deque 修改操作" << std::endl;

    std::deque<int> dq;
    // 1. 尾部增删：和vector用法一致
    dq.emplace_back(1);
    dq.emplace_back(2);
    dq.push_back(3);
    PrintDeque(dq, "尾部插入元素后");

    dq.pop_back();
    PrintDeque(dq, "尾部删除元素后");

    // 2. 头部增删：deque专属O(1)操作
    dq.emplace_front(0);
    dq.push_front(-1);
    PrintDeque(dq, "头部插入元素后");

    dq.pop_front();
    PrintDeque(dq, "头部删除元素后");

    // 3. 中间插入/删除：效率低，尽量少用
    auto mid_it = dq.begin() + dq.size() / 2;
    dq.insert(mid_it, 100);
    PrintDeque(dq, "中间插入元素后");

    auto erase_it = dq.begin() + 2;
    dq.erase(erase_it);
    PrintDeque(dq, "中间删除元素后");

    // 4. 批量操作
    dq.assign(4, 66);
    PrintDeque(dq, "assign批量赋值后");

    dq.clear();
    PrintDeque(dq, "clear清空容器后");
}

void testDequeCapacity() {
    std::cout << "\n测试 deque 容量操作" << std::endl;

    std::deque<int> dq = { 1, 2, 3, 4, 5 };

    // size()：返回当前元素个数
    std::cout << "size(): " << dq.size() << "\n";

    // empty()：判断容器是否为空，比size()==0更高效
    std::cout << "empty(): " << std::boolalpha << dq.empty() << "\n";

    // max_size()：返回容器理论上能容纳的最大元素个数
    std::cout << "max_size(): " << dq.max_size() << "\n";

    // resize(n)：调整元素数量
    dq.resize(8);  // 元素不足8个，补充默认值0，变为 [1,2,3,4,5,0,0,0]
    PrintDeque(dq, "resize(8)调整后");
    dq.resize(3);  // 元素超过3个，删除末尾元素，变为 [1,2,3]
    PrintDeque(dq, "resize(3)调整后");

    // shrink_to_fit()：释放未使用的缓冲区，减少内存占用（C++11）
    dq.shrink_to_fit();
    std::cout << "shrink_to_fit()后 size(): " << dq.size() << "\n";

    // swap()：交换两个deque的内容，O(1)，仅交换指针
    std::deque<int> other = { 10, 20, 30 };
    dq.swap(other);
    PrintDeque(dq, "swap交换后");
}

void testDequeIteratorInvalidation() {
    std::cout << "\n测试 deque 迭代器失效" << std::endl;

    // 1. 头部插入导致所有迭代器失效（注释掉避免未定义行为）
    // std::deque<int> dq1;
    // for (int i = 0; i < 5; ++i) {
    //     dq1.emplace_back(i);
    // }
    // std::deque<int>::iterator it1 = dq1.begin();
    // std::cout << "插入前迭代器值：" << *it1 << std::endl;
    // dq1.push_front(10);
    // std::cout << *it1 << std::endl; // 错误：访问失效迭代器，程序崩溃/异常

    // 2. 迭代器失效正确修复写法
    std::cout << "迭代器失效正确修复写法：\n";
    std::deque<int> dq2 = { 0,1,2,3,4 };
    dq2.push_front(10);
    // 插入后重新获取迭代器，避免失效
    auto it_new = dq2.begin();
    std::cout << "修复后迭代器值：" << *it_new << std::endl;

    // 循环删除正确写法：利用erase返回值
    std::cout << "循环删除偶数后：";
    for (auto it = dq2.begin(); it != dq2.end(); ) {
        if (*it % 2 == 0) {
            // erase返回下一个有效迭代器
            it = dq2.erase(it);
        }
        else {
            ++it;
        }
    }
    PrintDeque(dq2, "循环删除后");
}

void testDequeVectorConversion() {
    std::cout << "\n测试 deque 与 vector 的转换" << std::endl;

    // 场景1：deque转vector（先双端插入，再转vector做随机访问）
    std::deque<int> dq;
    // 双端高效插入
    for (int i = 1; i <= 5; ++i) {
        dq.emplace_front(i);
        dq.emplace_back(i + 5);
    }
    std::cout << "原deque元素：";
    for (int num : dq) std::cout << num << " ";
    std::cout << std::endl;

    // 方式1：范围构造直接转换
    std::vector<int> vec1(dq.begin(), dq.end());
    std::cout << "范围构造转vec1：";
    for (int num : vec1) std::cout << num << " ";
    std::cout << std::endl;

    // 方式2：预分配内存+assign，避免vector扩容
    std::vector<int> vec2;
    vec2.reserve(dq.size());
    vec2.assign(dq.begin(), dq.end());
    std::cout << "预分配+assign转vec2：";
    for (int num : vec2) std::cout << num << " ";
    std::cout << std::endl;

    // 方式3：移动语义转换，零拷贝（原deque失效）
    std::vector<int> vec3(std::make_move_iterator(dq.begin()), std::make_move_iterator(dq.end()));
    std::cout << "移动语义转vec3：";
    for (int num : vec3) std::cout << num << " ";
    std::cout << std::endl;
    std::cout << "移动后原deque size：" << dq.size() << std::endl;

    // 场景2：vector转deque
    std::vector<int> src_vec = { 10,20,30,40,50 };
    std::deque<int> dq2(src_vec.begin(), src_vec.end());
    std::cout << "vector转deque后元素：";
    for (int num : dq2) std::cout << num << " ";
    std::cout << std::endl;
}

#if 0
int main() {
    testDequeConstructors();
    testDequeElementAccess();
    testDequeModify();
    testDequeCapacity();
    testDequeIteratorInvalidation();
    testDequeVectorConversion();

    return 0;
}
#endif