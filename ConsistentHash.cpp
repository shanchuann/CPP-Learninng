#include<iostream>
#include<sstream>
#include<string>
#include<map>
#include<set>

// 一致性哈希核心类
class ConsistentHash {
private:
    // 虚拟节点映射：key=虚拟节点哈希值，value=真实物理节点IP+端口
    std::map<uint32_t, std::string> serverNodes;
    // 真实物理节点集合
    std::set<std::string> physicalNodes;
    // 单个物理节点对应的虚拟节点数量
    int virtualNodeNum;

    // FNV1_32_HASH哈希函数实现
    uint32_t FNV1_32_HASH(const std::string& key);

public:
    // 构造函数：默认虚拟节点数为1，初始化基础物理节点
    ConsistentHash(int vnm = 1);
    // 析构函数：清空节点映射
    ~ConsistentHash();
    // 初始化哈希环，映射所有虚拟节点
    void Initialize();
    // 添加新的物理节点
    void AddNewPhysicalNode(const std::string& nodeip);
    // 删除物理节点（同步删除对应虚拟节点）
    void DelPhysicalNode(const std::string& nodeip);
    // 根据key获取目标物理节点
    std::string GetServerIndex(const std::string& key);
    // 负载均衡统计：查看各节点负载占比
    void StaticPerf(const std::string& label, int left, int right);
};
// FNV1_32_HASH哈希函数：高离散度，无符号32位哈希值
uint32_t ConsistentHash::FNV1_32_HASH(const std::string& key) {
    const uint32_t p = 16777619;
    uint32_t hash = 2166136261;
    for (int idx = 0; idx < key.size(); ++idx) {
        hash = (hash ^ key[idx]) * p;
    }
    // 哈希扰动，提升离散度
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    return hash;
}

// 构造函数：初始化默认物理节点，设置虚拟节点数
ConsistentHash::ConsistentHash(int vnm) {
    virtualNodeNum = vnm;
    // 初始化4个默认物理节点
    physicalNodes.insert("192.168.1.101:12345");
    physicalNodes.insert("192.168.1.102:12346");
    physicalNodes.insert("192.168.1.103:12347");
    physicalNodes.insert("192.168.1.104:12348");
}

// 析构函数
ConsistentHash::~ConsistentHash() {
    serverNodes.clear();
    physicalNodes.clear();
}

// 初始化哈希环：遍历所有物理节点，生成对应虚拟节点并映射
void ConsistentHash::Initialize() {
    for (auto& node : physicalNodes) {
        for (int i = 0; i < virtualNodeNum; ++i) {
            // 虚拟节点命名规则：真实节点#VN+序号
            std::stringstream nodeKey;
            nodeKey << node << "#VN" << i;
            uint32_t hashVal = FNV1_32_HASH(nodeKey.str());
            serverNodes.insert({ hashVal, node });
        }
    }
}

// 添加物理节点：同步生成对应虚拟节点
void ConsistentHash::AddNewPhysicalNode(const std::string& nodeip) {
    auto insertRes = physicalNodes.insert(nodeip);
    if (insertRes.second) {
        for (int i = 0; i < virtualNodeNum; ++i) {
            std::stringstream nodeKey;
            nodeKey << nodeip << "#VN" << i;
            uint32_t hashVal = FNV1_32_HASH(nodeKey.str());
            serverNodes.insert({ hashVal, nodeip });
        }
    }
}

// 删除物理节点：同步删除所有关联虚拟节点
void ConsistentHash::DelPhysicalNode(const std::string& nodeip) {
    int eraseRes = physicalNodes.erase(nodeip);
    if (eraseRes == 1) {
        for (int i = 0; i < virtualNodeNum; ++i) {
            std::stringstream nodeKey;
            nodeKey << nodeip << "#VN" << i;
            uint32_t hashVal = FNV1_32_HASH(nodeKey.str());
            serverNodes.erase(hashVal);
        }
    }
}

// 核心寻址方法：根据key顺时针查找目标节点
std::string ConsistentHash::GetServerIndex(const std::string& key) {
    uint32_t hashVal = FNV1_32_HASH(key);
    // 找到第一个大于等于key哈希值的虚拟节点
    auto it = serverNodes.lower_bound(hashVal);
    // 遍历到环末尾，返回第一个节点
    if (it == serverNodes.end()) {
        if (serverNodes.empty()) {
            std::cout << "集群无可用节点！" << std::endl;
            return "";
        }
        return serverNodes.begin()->second;
    }
    return it->second;
}

// 负载统计：统计指定范围内key的节点分配占比
void ConsistentHash::StaticPerf(const std::string& label, int left, int right) {
    std::map<std::string, int> nodeCount;
    int total = right - left + 1;
    for (int i = left; i <= right; ++i) {
        std::string nodeIp = GetServerIndex(std::to_string(i));
        nodeCount[nodeIp]++;
    }
    std::cout << "======= " << label << " =======" << std::endl;
    for (auto& item : nodeCount) {
        double rate = (double)item.second / total * 100;
        std::cout << "节点IP：" << item.first << " 负载占比：" << rate << "%" << std::endl;
    }
    std::cout << std::endl;
}

#if 0
int main() {
    // 测试1：虚拟节点数为1，无虚拟节点优化
    ConsistentHash cha(1);
    cha.Initialize();
    cha.StaticPerf("虚拟节点数=1（无优化）", 0, 65536);

    // 测试2：虚拟节点数=400，优化负载均衡
    ConsistentHash chb(400);
    chb.Initialize();
    chb.StaticPerf("虚拟节点数=400（优化后）", 0, 65536);

    // 测试新增节点
    chb.AddNewPhysicalNode("192.168.1.105:12349");
    chb.StaticPerf("新增节点后（虚拟节点400）", 0, 65536);

    // 测试删除节点
    chb.DelPhysicalNode("192.168.1.102:12346");
    chb.StaticPerf("删除节点后（虚拟节点400）", 0, 65536);

    return 0;
}
#endif