#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

/*******************************************************************************************************************
* C++ STL 容器适配器核心知识点总结
* 容器适配器（Container Adapter）不是独立容器，而是**封装底层序列容器**的接口层，限制/修改接口行为以满足特定数据结构需求。
* 核心适配器：stack（栈）、queue（队列）、priority_queue（优先队列/堆）。
*
* 一、核心特性
* 1. 本质：接口适配器，复用 deque/vector/list 等底层容器的实现；
* 2. 无迭代器：不支持遍历、查找、修改中间元素，仅操作首尾/栈顶元素；
* 3. 底层默认容器：
*    - stack/queue：默认 std::deque（双端队列）；
*    - priority_queue：默认 std::vector（动态数组）；
* 4. 可自定义底层容器：支持替换为 vector/list/deque（需匹配接口要求）。
*
* 二、三大适配器区别
* 1. stack（栈）：
*    - 规则：LIFO 后进先出（Last In First Out）；
*    - 操作：push(入栈)、top(取栈顶)、pop(弹栈)、empty、size；
*    - 限制：仅能访问栈顶元素。
*
* 2. queue（队列）：
*    - 规则：FIFO 先进先出（First In First Out）；
*    - 操作：push(入队)、front(队头)、back(队尾)、pop(出队)；
*    - 限制：仅能访问队头、队尾元素。
*
* 3. priority_queue（优先队列/堆）：
*    - 规则：按优先级出队，而非插入顺序；
*    - 底层：堆（heap）结构，默认**大顶堆**（最大值优先）；
*    - 自定义：可通过 greater<> 改为**小顶堆**（最小值优先）；
*    - 操作：push(入堆)、top(堆顶)、pop(出堆)。
*
* 三、底层容器支持规则
* - stack：支持 back()、push_back()、pop_back() 的容器（deque/vector/list）；
* - queue：支持 front()、back()、push_back()、pop_front() 的容器（deque/list）；
* - priority_queue：支持随机访问的容器（vector/deque，list不支持）。
*******************************************************************************************************************/

// 栈适配器模板简化示意
template<class _Ty, class _C = std::deque<_Ty>>
class stack_demo {
public:
    typedef _C::size_type size_type;
    typedef _C::value_type value_type;
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    value_type& top() { return c.back(); }
    const value_type& top() const { return c.back(); }
    void push(const value_type& _x) { c.push_back(_x); }
    void pop() { c.pop_back(); }
protected:
    _C c;
};

// 优先队列模板简化示意
template<class _Ty, class _Container = std::vector<_Ty>, class _Pr = std::less<typename _Container::value_type>>
class priority_queue_demo {
public:
    typedef _Container::size_type size_type;
    typedef _Container::value_type value_type;
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const value_type& top() const { return c.front(); }
    void push(const value_type& _val) {
        c.push_back(_val);
        std::push_heap(c.begin(), c.end(), comp);
    }
    void pop() {
        std::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
protected:
    _Container c;
    _Pr comp;
};

// 1. stack 栈适配器测试
void test_stack() {
    std::cout << "Stack 栈适配器基础操作" << std::endl;
    // 默认底层容器：deque
    std::stack<int> sta_deque;
    // 指定底层容器为 vector
    std::stack<int, std::vector<int>> sta_vec;
    // 指定底层容器为 list
    std::stack<int, std::list<int>> sta_list;

    // 元素入栈
    sta_deque.push(12);
    sta_deque.push(23);
    sta_deque.push(34);
    // 栈遍历（仅可通过弹栈实现，严格遵循 LIFO 规则）
    while (!sta_deque.empty()) {
        int val = sta_deque.top();
        std::cout << val << " ";
        sta_deque.pop();
    }
    std::cout << "\n==========================================\n" << std::endl;
}

// 2. queue 队列适配器测试
void test_queue() {
    std::cout << "Queue 队列适配器基础操作" << std::endl;
    // 默认底层容器：deque
    std::queue<int> que_deque;
    // 指定底层容器为 list
    std::queue<int, std::list<int>> que_list;

    // 元素入队
    que_deque.push(10);
    que_deque.push(20);
    que_deque.push(30);
    // 队列遍历（仅可通过出队实现，严格遵循 FIFO 规则）
    while (!que_deque.empty()) {
        int val = que_deque.front();
        std::cout << val << " ";
        que_deque.pop();
    }
    std::cout << "\n============================================\n" << std::endl;
}

// 3. priority_queue 优先队列/堆测试
void test_priority_queue() {
    std::cout << "Priority Queue 优先队列操作" << std::endl;
    // 默认大顶堆：数值越大，优先级越高，优先出队
    std::priority_queue<int> pri_que;
    pri_que.push(56);
    pri_que.push(23);
    pri_que.push(78);
    pri_que.push(12);

    std::cout << "大顶堆（默认）输出：";
    while (!pri_que.empty()) {
        int val = pri_que.top();
        std::cout << val << " ";
        pri_que.pop();
    }
    std::cout << std::endl;

    // 自定义小顶堆：数值越小，优先级越高，优先出队
    std::priority_queue<int, std::vector<int>, std::greater<int>> small_heap;
    small_heap.push(56);
    small_heap.push(23);
    small_heap.push(78);
    small_heap.push(12);

    std::cout << "小顶堆（自定义）输出：";
    while (!small_heap.empty()) {
        int val = small_heap.top();
        std::cout << val << " ";
        small_heap.pop();
    }
    std::cout << "\n==================================================\n" << std::endl;
}
#if 0
int main() {
    test_stack();
    test_queue();
    test_priority_queue();

    return 0;
}
#endif