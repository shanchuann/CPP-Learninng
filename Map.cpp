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
#include <stdexcept>

//struct Student
//{
//    std::string s_id;
//    std::string s_name;
//    std::string s_sex;
//    int s_age;
//};
//std::ostream& operator<<(std::ostream& out, const struct Student& s)
//{
//    out << "id:" << s.s_id
//        << " name:" << s.s_name
//        << " sex:" << s.s_sex
//        << " age:" << s.s_age;
//    return out;
//}
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
//    std::cout << "=======================================================" << std::endl << std::endl;
//}

#if 0
int main()
{
    test_map_basic();
    return 0;
}
#endif