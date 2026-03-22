#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

// 学生信息结构体
struct Student
{
    std::string s_id;
    std::string s_name;
    std::string s_sex;
    int s_age;
};

std::ostream& operator<<(std::ostream& out, const struct Student& s)
{
    out << "id:" << s.s_id
        << " name:" << s.s_name
        << " sex:" << s.s_sex
        << " age:" << s.s_age;
    return out;
}

// DNS记录结构体
struct DNSRecord
{
    std::string ipAddress;
    std::string type;
};

// 全局DNS记录容器
std::multimap<std::string, DNSRecord> dnsRecords;

// 1. std::pair 基础使用示例
void test_pair()
{
    std::cout << "std::pair 示例" << std::endl;
    std::pair<std::string, int> na1("Zhang", 12);
    std::pair<std::string, int> na2 = std::make_pair("Zhang", 23);

    std::cout << "na1.first: " << na1.first << std::endl;
    std::cout << "na1.second: " << na1.second << std::endl;

    // C++17结构化绑定
    auto [name, age] = na1;
    std::cout << "结构化绑定 name: " << name << std::endl;
    std::cout << "结构化绑定 age: " << age << std::endl;

    std::cout << "na1 < na2 比较结果: " << (na1 < na2) << std::endl;
    std::cout << std::endl << std::endl;
}

// 2. std::map 基础操作示例
//void test_map_basic()
//{
//    std::cout << "std::map 基础操作示例" << std::endl;
//    std::vector<struct Student> svec = {
//        {"2024001", "Li Ming", "man", 12},
//        {"2024002", "Wang Wei", "woman", 13},
//        {"2024003", "Zhao Lei", "man", 11},
//        {"2024004", "Chen Jie", "man", 10},
//        {"2024005", "Liu Yang", "woman", 11},
//        {"2024006", "Zhou Fang", "man", 13},
//        {"2024008", "Sun Hao", "man", 17}
//    };
//
//    std::map<std::string, int> simap;
//
//    int n = svec.size();
//
//    for (int i = 0; i < n; ++i)
//    {
//        simap.emplace(svec[i].s_id, i);
//    }
//
//    std::cout << "map有序遍历结果：" << std::endl;
//    for (auto& x : simap)
//    {
//        std::cout << "x.first:" << x.first << " x.second:" << x.second << std::endl;
//        std::cout << svec[x.second] << std::endl;
//    }
//
//    std::cout << std::endl << "at()与operator[]访问测试：" << std::endl;
//    try {
//        std::cout << "simap.at(\"2024001\"): " << simap.at("2024001") << std::endl;
//    }
//    catch (const std::out_of_range& e) {
//        std::cout << "异常信息：" << e.what() << std::endl;
//    }
//    simap["2024999"];
//    std::cout << "map插入冗余键后size: " << simap.size() << std::endl;
//    std::cout << std::endl << std::endl;
//}


// 3. std::map 范围查找示例（修复迭代器有效性判断）
void test_map_bound()
{
    std::cout << "std::map 范围查找示例" << std::endl;
    typedef std::map<int, std::string> ServerMap;
    ServerMap sermap = {
        {2000, "node1"},
        {23000, "node2"},
        {480000, "node3"},
        {980000, "node4"}
    };

    std::cout << "----- lower_bound 测试 -----" << std::endl;
    for (int i = 1998; i < 2002; ++i)
    {
        auto it = sermap.lower_bound(i);
        if (it != sermap.end())
        {
            std::cout << "i:" << i << " ip:" << it->first << " node:" << it->second << std::endl;
        }
        else
        {
            std::cout << "i:" << i << " 无匹配元素" << std::endl;
        }
    }

    std::cout << std::endl << "----- upper_bound 测试 -----" << std::endl;
    for (int i = 1998; i < 2002; ++i)
    {
        auto it = sermap.upper_bound(i);
        if (it != sermap.end())
        {
            std::cout << "i:" << i << " ip:" << it->first << " node:" << it->second << std::endl;
        }
        else
        {
            std::cout << "i:" << i << " 无匹配元素" << std::endl;
        }
    }

    std::cout << std::endl << "----- equal_range 测试 -----" << std::endl;
    auto itpair = sermap.equal_range(2000);
    if (itpair.first != sermap.end()) {
        std::cout << "匹配元素：" << itpair.first->first << " " << itpair.first->second << std::endl;
    }
    else
    {
        std::cout << "无匹配元素" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

// 4. std::map 单词计数示例
void test_map_wordcount()
{
    std::cout << "std::map 单词计数示例" << std::endl;
    typedef std::map<std::string, int> wordMap;
    const char* filename = "shanchuan.txt";
    std::ifstream in(filename);
    if (!in)
    {
        fprintf(stderr, "无法打开文件 %s\n", filename);
        std::cout << std::endl << std::endl;
        return;
    }

    wordMap wordmap;
    std::string word;
    while (in >> word)
    {
        wordmap[word]++;
    }

    std::cout << "单词计数结果（有序）：" << std::endl;
    for (const auto& w : wordmap)
    {
        std::cout << w.first << ":" << w.second << std::endl;
    }
    std::cout << std::endl << std::endl;
}

// 5. std::multimap 基础操作示例
void test_multimap_basic()
{
    std::cout << "std::multimap 基础操作示例" << std::endl;
    typedef std::map<int, std::string> ISMap;
    typedef std::multimap<int, std::string> ISMulMap;

    ISMap ismap;
    ISMulMap ismulmap;

    std::cout << "----- map 插入测试 -----" << std::endl;
    auto it = ismap.insert(std::make_pair(23, "Zhang"));
    std::cout << "map首次插入结果：" << it.second << " 值：" << it.first->second << std::endl;
    it = ismap.insert(std::make_pair(23, "shanchuan"));
    std::cout << "map重复插入结果：" << it.second << " 值：" << it.first->second << std::endl;

    std::cout << std::endl << "----- multimap 插入测试 -----" << std::endl;
    auto mit = ismulmap.insert(std::make_pair(23, "Zhang"));
    std::cout << "multimap首次插入值：" << mit->second << std::endl;
    mit = ismulmap.insert(std::make_pair(23, "shanchuan"));
    std::cout << "multimap重复插入值：" << mit->second << std::endl;

    std::cout << std::endl << "multimap全部元素：" << std::endl;
    for (const auto& elem : ismulmap) {
        std::cout << elem.first << " " << elem.second << std::endl;
    }
    std::cout << std::endl << std::endl;
}

// 6. std::multimap DNS记录示例
void addDNSRecord(const std::string& domain, const DNSRecord& record)
{
    dnsRecords.emplace(domain, record);
}

std::vector<DNSRecord> findDNS(const std::string& domain)
{
    std::vector<DNSRecord> results;
    auto range = dnsRecords.equal_range(domain);
    for (auto it = range.first; it != range.second; ++it)
    {
        results.push_back(it->second);
    }
    return results;
}

void test_multimap_dns()
{
    std::cout << "std::multimap DNS记录示例" << std::endl;
    addDNSRecord("baidu.com", { "192.168.23.1", "IPv4" });
    addDNSRecord("baidu.com", { "2002::c0a8:1701", "IPv6" });
    addDNSRecord("tulun.net", { "192.168.0.23", "IPv4" });
    addDNSRecord("tulun.net", { "2002::c0a8:17", "IPv4" });

    std::string domainToQuery = "baidu.com";
    std::cout << "域名" << domainToQuery << "的DNS记录：" << std::endl;
    auto records = findDNS(domainToQuery);
    for (const auto& record : records)
    {
        std::cout << "IP:" << record.ipAddress << ",类型:" << record.type << std::endl;
    }
    std::cout << std::endl << std::endl;
}

// 7. std::set 示例
void test_set()
{
    std::cout << "std::set 示例" << std::endl;
    const int n = 20;
    std::vector<int> ivec;
    std::set<int> iset;
    ivec.reserve(n);
    srand((unsigned int)time(NULL));

    for (int i = 0; i < n; ++i)
    {
        ivec.push_back(rand() % 20);
    }

    std::cout << "原始vector无序重复数据：" << std::endl;
    for (int num : ivec)
    {
        std::cout << "\t" << num;
    }
    std::cout << std::endl << std::endl;

    iset.insert(ivec.begin(), ivec.end());
    std::cout << "set去重并升序排列后数据：" << std::endl;
    for (int num : iset)
    {
        std::cout << "\t" << num;
    }
    std::cout << std::endl;

    int target = 10;
    if (iset.find(target) != iset.end()) {
        std::cout << "元素" << target << "存在于set中" << std::endl;
    }
    else {
        std::cout << "元素" << target << "不存在于set中" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

// 8. std::multiset 示例
void test_multiset()
{
    std::cout << "std::multiset 示例" << std::endl;
    std::multiset<int> scoreSet;
    scoreSet.insert(85);
    scoreSet.insert(92);
    scoreSet.insert(78);
    scoreSet.insert(85);
    scoreSet.insert(92);
    scoreSet.insert(88);
    scoreSet.insert(78);

    std::cout << "学生成绩有序升序列表：" << std::endl;
    for (int score : scoreSet)
    {
        std::cout << score << " ";
    }
    std::cout << std::endl << std::endl;

    int target = 85;
    std::cout << "分数" << target << "出现的总次数：" << scoreSet.count(target) << std::endl;

    auto range = scoreSet.equal_range(92);
    std::cout << "所有92分成绩：";
    for (auto it = range.first; it != range.second; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    auto delIt = scoreSet.find(85);
    if (delIt != scoreSet.end()) {
        scoreSet.erase(delIt);
    }
    std::cout << "删除单个85分后，剩余85分次数：" << scoreSet.count(85) << std::endl;
    std::cout << std::endl << std::endl;
}
#if 0
// 主函数
int main()
{
    // 项目属性必须开启C++17标准
    test_pair();
	// test_map_basic(); // 不知为何总会抛出异常，单独放入Map.cpp
    test_map_bound();
    // test_map_wordcount(); // 需提前创建shanchuan.txt
    test_multimap_basic();
    test_multimap_dns();
    test_set();
    test_multiset();
    return 0;
}
#endif