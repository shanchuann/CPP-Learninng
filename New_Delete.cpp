#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

/*******************************************************************************************************************
* C++动态内存分配（Dynamic Memory Allocation）
* 动态内存分配是指在程序运行时根据需求分配和释放内存的机制，主要通过C++的new/delete运算符、C语言的malloc/free函数
* 以及特殊的placement new（定位new）实现。
*
* 一、new与delete运算符（C++推荐）
* 1. 单个对象的分配与释放
*    - 分配：new 类型(初始化值) → 自动计算内存大小，分配内存并初始化（基本类型直接赋值，类类型调用构造函数）
*    - 释放：delete 指针 → 释放内存（类类型先调用析构函数）
*
* 2. 数组的分配与释放
*    - 分配：new 类型[数组大小]{初始化列表} → 分配数组内存，支持初始化（C++11及以上）
*    - 释放：delete[] 指针 → 必须与new[]配对，确保数组中每个元素（类类型）的析构函数被调用
*
* 二、placement new（定位new）
* 定义：在已分配的内存空间上构造对象，不分配新内存，仅调用对象的构造函数。
* 语法：new(内存地址) 类型(初始化值)
* 特点：
*    - 内存需提前分配（可在栈、堆或全局内存）；
*    - 无需用delete释放内存（因内存不是new分配的），类类型需显式调用析构函数；
*    - 适用于内存池、固定缓冲区等场景，提高内存复用效率。
*
* 三、malloc与free（C语言兼容）
* 1. 分配：void* malloc(字节数) → 仅分配指定大小的原始内存，不初始化，返回void*（需强转）
* 2. 释放：free(指针) → 仅释放内存，不调用析构函数（类类型有风险）
* 3. 与new的核心区别：malloc是函数，仅管内存分配；new是运算符，兼顾内存分配+初始化（构造函数）。
*
* 关键原则：
* - 配对使用：new → delete；new[] → delete[]；malloc → free（不混用，避免内存泄漏或崩溃）
* - 避免野指针：释放内存后将指针置为nullptr，防止二次释放
*******************************************************************************************************************/

// 1. 单个对象的new/delete使用
void singleObjectDemo() {
    std::cout << "----- 单个对象的new/delete -----" << std::endl;
    // new的过程：1.计算int大小 2.分配内存 3.初始化值10 4.返回int*
    int* pInt = new int(10);  // 分配并初始化单个int
    std::cout << "new int(10) = " << *pInt << std::endl;

    // 直接调用operator new（底层内存分配函数，类似malloc）
    int* pOpNew = (int*)::operator new(sizeof(int));  // 仅分配内存，不初始化
    *pOpNew = 20;  // 手动初始化
    std::cout << "::operator new分配的int = " << *pOpNew << std::endl;

    // 释放内存
    delete pInt;       // 释放new分配的内存
    ::operator delete(pOpNew);  // 释放operator new分配的内存（对应操作）
    pInt = nullptr;    // 避免野指针
    pOpNew = nullptr;
    std::cout << std::endl;
}

// 2. 数组的new[]/delete[]使用
void arrayDemo() {
    std::cout << "----- 数组的new[]/delete[] -----" << std::endl;
    size_t size = 10;

    // 分配未初始化的int数组
    int* arr = new int[size];  // new[]分配数组
    for (size_t i = 0; i < size; i++) {
        arr[i] = i + 1;  // 手动初始化
    }
    std::cout << "未初始化数组（手动赋值）: ";
    for (size_t i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // 分配并初始化的int数组（C++11列表初始化）
    int* arrInit = new int[size] {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    std::cout << "初始化数组（列表赋值）: ";
    for (size_t i = 0; i < size; i++) {
        std::cout << arrInit[i] << " ";
    }
    std::cout << std::endl;

    // C语言的malloc/free分配数组（兼容使用）
    int* arrMalloc = (int*)malloc(size * sizeof(int));  // 需显式计算字节数，返回void*
    if (arrMalloc != nullptr) {  // malloc失败返回NULL，需检查
        free(arrMalloc);  // 释放malloc分配的内存
    }

    // 释放数组内存（必须用delete[]，与new[]配对）
    delete[] arr;
    delete[] arrInit;
    arr = nullptr;
    arrInit = nullptr;
    std::cout << std::endl;
}

// 3. placement new（定位new）使用
void placementNewDemo() {
    std::cout << "----- placement new（定位new） -----" << std::endl;
    // 在栈上分配一块缓冲区（也可以是堆或全局内存）
    char buffer[128];  // 栈上的内存块，大小128字节
    std::cout << "缓冲区起始地址: " << (void*)buffer << std::endl;

    // 在缓冲区指定位置构造int（不分配新内存，仅初始化）
    int* p1 = new(buffer) int(10);  // 从buffer起始位置构造int
    // 偏移sizeof(int)位置，构造第二个int
    int* p2 = new(buffer + sizeof(int)) int(20);
    // 偏移sizeof(double)位置（double通常8字节），构造double
    double* p3 = new(buffer + sizeof(double)) double(3.14);

    // 验证数据
    std::cout << "buffer起始位置的int: " << *p1 << "（地址: " << (void*)p1 << "）" << std::endl;
    std::cout << "偏移int大小的int: " << *p2 << "（地址: " << (void*)p2 << "）" << std::endl;
    std::cout << "偏移double大小的double: " << *p3 << "（地址: " << (void*)p3 << "）" << std::endl;

    // 注意：placement new不分配内存，无需用delete释放buffer
    // 若为类类型，需显式调用析构函数（基本类型可忽略）
    // p1->~int();  // 基本类型析构无意义，可省略
    std::cout << std::endl;
}

// 4. new与malloc的核心区别对比
void newVsMalloc() {
    std::cout << "----- new与malloc的核心区别 -----" << std::endl;
    // new的特性：自动计算大小、返回类型指针、初始化（构造函数）
    int* pNew = new int(12);
    // malloc的特性：需显式指定大小、返回void*（需强转）、不初始化
    int* pMalloc = (int*)malloc(sizeof(int));
    *pMalloc = 12;  // 手动初始化

    std::cout << "new分配的int: " << *pNew << std::endl;
    std::cout << "malloc分配的int: " << *pMalloc << std::endl;

    // 释放：new对应delete，malloc对应free
    delete pNew;    // 释放+（类类型）析构
    free(pMalloc);  // 仅释放
    pNew = nullptr;
    pMalloc = nullptr;
}

#if 0
int main() {
    singleObjectDemo();    // 单个对象的new/delete
    arrayDemo();           // 数组的new[]/delete[]
    placementNewDemo();    // placement new（定位new）
    newVsMalloc();         // new与malloc的区别对比
    return 0;
}
#endif