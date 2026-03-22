#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

/*******************************************************************************************************************
* C++ STL 无序关联容器核心知识点总结
* 无序关联容器底层为**哈希表（Hash Table）**，也叫散列表，通过哈希函数将键映射到桶（bucket），实现**平均O(1)查找/插入/删除**。
* 核心容器：unordered_map、unordered_multimap、unordered_set、unordered_multiset。
* 与有序关联容器（map/set）最大区别：**无序、无排序、性能更高**。
*
* 一、核心特性
* 1. 底层结构：哈希表（链地址法解决哈希冲突），元素**无序存储**；
* 2. 时间复杂度：平均 O(1)，最坏 O(n)（哈希冲突严重时）；
* 3. 有序容器对比：
*    - map/set：红黑树，有序，O(log n)，适合需要排序的场景；
*    - 无序容器：哈希表，无序，O(1)，适合纯查找/插入的高性能场景；
* 4. 无 `lower_bound/upper_bound`：无序，不支持范围查找。
*
* 二、四大容器区别
* 1. unordered_map：键唯一，键值对，无序；
* 2. unordered_multimap：键可重复，键值对，无序；
* 3. unordered_set：值唯一，单值，无序；
* 4. unordered_multiset：值可重复，单值，无序。
*
* 三、核心操作
* 1. 插入：emplace（推荐，原位构造）、insert、operator[]（仅map）；
* 2. 查找：find()（返回迭代器）、count()（统计个数）；
* 3. 删除：erase()（按键/迭代器删除）；
* 4. 重复键：仅 multimap/multiset 支持，用 equal_range 遍历所有重复元素。
*
* 四、自定义类型作为键（必须满足两个条件）
* 1. 重载 `operator==`：用于判断两个键是否相等；
* 2. 提供**自定义哈希函数**：为自定义类型生成哈希值。
*
* 五、迭代器失效规则
* 1. 插入元素：**可能导致所有迭代器失效**（重哈希rehash时）；
* 2. 删除元素：**仅被删除节点的迭代器失效**；
* 3. 重哈希：元素数量超过负载因子时自动扩容，所有迭代器失效。
*
* 六、头文件
* - unordered_map / unordered_multimap：#include <unordered_map>
* - unordered_set / unordered_multiset：#include <unordered_set>
*******************************************************************************************************************/

// 1. 简化版哈希表底层结构
// 哈希表节点结构
template <class Value>
struct _hashtable_node {
    _hashtable_node* next;
    Value val;
};

// 哈希表主体结构
template <class value, class key, class HashFcn, class Extractkey, class Equalkey, class Alloc>
class hashtable {
public:
    typedef key key_type;
    typedef value value_type;
    typedef HashFcn hasher;
    typedef Equalkey key_equal;
private:
    typedef _hashtable_node<value> node;
    hasher hash;
    key_equal equals;
    Extractkey get_key;
    // 哈希桶数组
    std::vector<node*, Alloc> buckets;
    // 存储元素总数
    size_t num_elements;
};

// 2. 自定义类型作为键 
// 自定义结构体
struct Student {
    std::string id;
    std::string name;
    // 必须重载 == 运算符，用于判断键是否重复
    bool operator==(const Student& other) const {
        return id == other.id;
    }
};

// 自定义哈希函数，为自定义类型提供哈希映射规则
struct HashStudent {
    size_t operator()(const Student& s) const {
        return std::hash<std::string>()(s.id);
    }
};

// 测试函数（模块化拆分）
// 1. unordered_map 基础操作
void test_unordered_map_basic() {
    std::cout << "unordered_map 基础操作" << std::endl;
    std::unordered_map<std::string, int> str_map = { {"yhping",12}, {"hm",15} };

    // insert 插入键值对
    str_map.insert(std::make_pair("lwy", 17));
    // emplace 原地构造，效率高于 insert
    str_map.emplace("laoguai", 23);
    // 下标运算符插入，键不存在则自动新增
    str_map["libai"] = 20;

    // 遍历（无序输出）
    std::cout << "容器元素：" << std::endl;
    for (auto& x : str_map) {
        std::cout << x.first << " => " << x.second << std::endl;
    }

    // 查找指定键
    auto it = str_map.find("libai");
    if (it != str_map.end()) {
        std::cout << "查找到元素 libai，值：" << it->second << std::endl;
    }

    // 删除指定键
    str_map.erase("hm");
    std::cout << "删除键 hm 后，容器大小：" << str_map.size() << std::endl;
    std::cout << "==========================================" << std::endl << std::endl;
}

// 2. unordered_multimap 重复键操作
void test_unordered_multimap() {
    std::cout << "unordered_multimap 重复键操作" << std::endl;
    std::unordered_multimap<std::string, int> mm{ {"apple",1}, {"apple",2}, {"banana",3} };

    // equal_range 批量获取重复键的所有元素
    auto range = mm.equal_range("apple");
    std::cout << "键 apple 对应的所有值：";
    for (auto iter = range.first; iter != range.second; ++iter) {
        std::cout << iter->second << " ";
    }
    std::cout << std::endl;
    std::cout << "================================================" << std::endl << std::endl;
}

// 3. 自定义类型作为 unordered_map 键
//void test_custom_type_key() {
//    std::cout << "自定义类型作为 unordered_map 键" << std::endl;
//    std::unordered_map<Student, int, HashStudent> stu_map;
//
//    // 插入自定义类型键值对
//    Student s1 = { "2024001", "张三" };
//    Student s2 = { "2024002", "李四" };
//    stu_map.emplace(s1, 90);
//    stu_map.emplace(s2, 85);
//
//    // 查找自定义键
//    auto stu_it = stu_map.find(s1);
//    if (stu_it != stu_map.end()) {
//        std::cout << "学生 " << s1.id << " 的分数：" << stu_it->second << std::endl;
//    }
//    std::cout << "==================================================" << std::endl << std::endl;
//}

// 4. 补充：unordered_set 基础使用
void test_unordered_set() {
    std::cout << "unordered_set 基础操作" << std::endl;
    std::unordered_set<int> uset = { 3,1,4,1,5,9,2,6 };

    std::cout << "去重无序元素：";
    for (int num : uset) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 查找元素
    if (uset.find(5) != uset.end()) {
        std::cout << "元素 5 存在" << std::endl;
    }
    std::cout << "==========================================" << std::endl << std::endl;
}
#if 0
int main() {
    test_unordered_map_basic();
    test_unordered_multimap();
    //test_custom_type_key();
    test_unordered_set();

    return 0;
}
#endif