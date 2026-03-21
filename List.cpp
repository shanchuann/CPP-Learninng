#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <list>
#include <iomanip>
#include <algorithm>

/*******************************************************************************************************************
* C++ std::list 核心知识点总结
* std::list 是C++标准库的**双向循环链表**容器，底层为非连续内存，是最常用的链表结构，与vector/deque形成互补。
* 核心内容包括：list基本特性、构造函数、遍历方式、基础操作、专属高级操作、迭代器失效规则。
*
* 一、list基本特性
* 1. 底层结构：**双向链表**（非连续内存），每个节点包含数据+前驱指针+后继指针；
* 2. 核心优势：
*    - 任意位置**插入/删除元素 O(1) 时间复杂度**（迭代器有效前提下）；
*    - 迭代器稳定性极高（除删除节点本身外，其余迭代器永不失效）；
*    - 拥有专属链表操作：splice(拼接)、merge(合并)、unique(去重)、sort(排序)、reverse(反转)；
* 3. 核心限制：
*    - **不支持随机访问**（无 operator[] / at() 方法），只能顺序遍历；
*    - 占用内存比vector大（需存储节点指针）；
* 4. 头文件：#include <list>。
*
* 二、构造函数（与其他STL容器用法一致）
* 1. 默认构造：空list；
* 2. 初始化列表构造（C++11）；
* 3. 填充构造：n个指定值；
* 4. 迭代器区间构造：从数组/容器复制；
* 5. 拷贝构造；
* 6. 移动构造（C++11）：转移所有权，零拷贝。
*
* 三、元素访问与遍历
* 1. 仅支持访问**首尾元素**：front() / back()；
* 2. 不支持下标访问 []、at()（非连续内存无法随机访问）；
* 3. 遍历方式：
*    - 正向迭代器遍历（list<T>::iterator）；
*    - 范围for循环（推荐，语法糖）；
*    - 反向迭代器 rbegin()/rend()。
*
* 四、基础修改操作
* 1. 头尾增删：push_front/emplace_front、pop_front、push_back/emplace_back、pop_back（均O(1)）；
* 2. 指定位置插入：insert(iterator, val)（O(1)）；
* 3. 指定位置删除：erase(iterator)（O(1)，仅失效当前迭代器）；
* 4. 清空容器：clear()；
* 5. 赋值操作：assign()。
*
* 五、list 专属高级操作（核心！链表独有）
* 1. splice：**转移元素**，将另一个list的元素移动到当前list，零拷贝、O(1)；
* 2. merge：**有序合并**两个有序list，合并后仍有序，源list清空；
* 3. unique：**连续去重**，删除相邻重复元素（需先排序才能全去重）；
* 4. sort：**链表排序**（不支持std::sort，用成员函数sort）；
* 5. reverse：**反转链表**，O(n)。
*
* 六、容量操作
* 1. size()：获取元素个数；
* 2. empty()：判断是否为空；
* 3. resize()：调整元素数量；
* 4. 无 capacity() / reserve() / data()（非连续内存无容量概念）。
*
* 七、迭代器失效规则（list最大优势）
* 1. 插入元素：**所有迭代器保持有效**；
* 2. 删除元素：**仅被删除节点的迭代器失效**，其余全部有效；
* 3. 对比vector/deque：不会因操作导致全部迭代器失效，稳定性最强。
*******************************************************************************************************************/

// 通用模板打印函数（全局，适配所有STL容器）
template<class _Con>
void PrintList(const _Con& ilist) {
    std::cout << "元素个数:" << std::setw(2) << ilist.size() << " | 数据:";
    for (const auto& x : ilist) {
        std::cout << std::setw(3) << x;
    }
    std::cout << std::endl;
}

// 1. 测试构造函数
void testConstructor() {
    std::cout << "\n1. 测试 list 构造函数" << std::endl;
    const int n = 10;
    int arr[n] = { 1,2,3,4,5,6,7,8,9,10 };

    // 默认构造
    std::list<int> ilista;
    // 初始化列表构造
    std::list<int> ilistb = { 12,23,34,45,56 };
    // 填充构造
    std::list<int> ilistc(5, 23);
    // 迭代器区间构造
    std::list<int> ilistd(arr, arr + n);
    // 拷贝构造
    std::list<int> iliste(ilistb);
    // 移动构造
    std::list<int> ilistf(std::move(ilistb));

    PrintList(ilista);
    PrintList(ilistb);  // 移动后为空
    PrintList(ilistc);
    PrintList(ilistd);
    PrintList(iliste);
    PrintList(ilistf);
}

// 2. 测试遍历方式
void testTraverse() {
    std::cout << "\n2. 测试 list 遍历" << std::endl;
    std::list<int> ilist = { 1,2,3,4,5 };

    // 正向迭代器遍历
    std::cout << "迭代器遍历: ";
    for (std::list<int>::iterator it = ilist.begin(); it != ilist.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 范围for遍历（推荐）
    std::cout << "范围for遍历: ";
    for (const auto& elem : ilist) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// 3. 测试赋值操作
void testAssign() {
    std::cout << "\n3. 测试 list assign 赋值" << std::endl;
    std::list<int> ilist = { 1,2,3 };

    // 批量填充5个10
    ilist.assign(5, 10);
    PrintList(ilist);
    // 区间赋值
    int arr[] = { 6,7,8 };
    ilist.assign(arr, arr + 3);
    PrintList(ilist);
    // 初始化列表赋值
    ilist.assign({ 100,200,300 });
    PrintList(ilist);
}

// 4. 测试专属操作：splice 转移元素
void testSplice() {
    std::cout << "\n4. 测试 list splice 元素转移" << std::endl;
    std::list<int> arlist = { 1,3,5,7,9 };
    std::list<int> brlist = { 2,4,6,8,10 };

    std::cout << "转移前：arlist: ";
    PrintList(arlist);
    std::cout << "转移前：brlist: ";
    PrintList(brlist);

    // 迭代器后移2位，指向元素5
    auto it = arlist.begin();
    std::advance(it, 2);
    // 将brlist所有元素转移到arlist的it位置（零拷贝，O(1)）
    arlist.splice(it, brlist);

    std::cout << "转移后：arlist: ";
    PrintList(arlist);
    std::cout << "转移后：brlist: ";  // brlist变为空
    PrintList(brlist);
}

// 5. 测试专属操作：merge 有序合并
void testMerge() {
    std::cout << "\n5. 测试 list merge 有序合并" << std::endl;
    std::list<int> arlist = { 1,2,3,4,5,6,7,8,9,10 };
    std::list<int> brlist = { 2,4,6,8,10 };

    arlist.merge(brlist);
    std::cout << "合并后：";
    PrintList(arlist);
    std::cout << "源链表：";
    PrintList(brlist);  // 空
}

// 6. 测试专属操作：unique 去重
void testUnique() {
    std::cout << "\n6. 测试 list unique 去重" << std::endl;
    std::list<int> ilist = { 1,2,2,3,3,3,4 };
    std::cout << "去重前：";
    PrintList(ilist);
    ilist.unique();
    std::cout << "去重后：";
    PrintList(ilist);  // 输出：1 2 3 4
}

// 7. 补充：list 基础增删/排序/反转
void testBasicOperate() {
    std::cout << "\n7. 测试 list 基础操作" << std::endl;
    std::list<int> ilist;

    // 头尾插入
    ilist.emplace_back(3);
    ilist.emplace_front(1);
    ilist.insert(ilist.begin(), 0);
    std::cout << "插入后：";
    PrintList(ilist);

    // 排序 + 反转
    ilist.sort();
    std::cout << "排序后：";
    PrintList(ilist);
    ilist.reverse();
    std::cout << "反转后：";
    PrintList(ilist);
}
#if 0
int main() {
    testConstructor();
    testTraverse();
    testAssign();
    testSplice();
    testMerge();
    testUnique();
    testBasicOperate();

    return 0;
}
#endif