#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <utility>

/*******************************************************************************************************************
* C++ 手写独占智能指针 my_unique_ptr 核心知识点总结
* 对应 std::unique_ptr 实现原理：**独占式智能指针**，同一时间只能有一个指针管理资源，禁止拷贝，仅支持移动。
* 核心特性：自动释放资源、无额外开销、支持自定义删除器、支持动态数组、适配任意资源（内存/文件/句柄）。
*
* 一、核心设计原则
* 1. 独占所有权：一个资源只能被一个 my_unique_ptr 管理，禁止拷贝构造/拷贝赋值；
* 2. 移动语义：通过移动语义转移资源所有权，是唯一的转移方式；
* 3. 自动释放：析构函数自动调用删除器释放资源，杜绝内存泄漏/资源泄漏；
* 4. 自定义删除器：支持自定义资源释放方式（如文件关闭、数组释放、句柄释放）。
*
* 二、核心接口
* 1. 构造/析构：管理裸指针，析构自动释放；
* 2. 移动：移动构造、移动赋值，转移资源所有权；
* 3. 核心操作：
*    - release()：释放所有权，返回裸指针（不再管理资源）；
*    - reset()：释放当前资源，重新管理新资源；
*    - swap()：交换两个智能指针的资源；
*    - get()：获取内部裸指针；
* 4. 运算符重载：*、->（单对象）、[]（数组）、bool 类型转换。
*
* 三、特化版本
* 1. 普通单对象：delete 释放资源；
* 2. 动态数组特化：delete[] 释放资源，支持 [] 下标访问；
* 3. 自定义删除器：适配 FILE*、socket、文件句柄等非内存资源。
*******************************************************************************************************************/


// 通用删除器：单个对象释放
// 说明：这是一个无状态的默认删除器，实现对单个对象的释放（使用 delete）。
// 要求：删除器必须可被拷贝或移动（此实现中通过 std::swap 等操作移动删除器）。
// 注意：如果用户提供自定义删除器，删除器需要保证对传入指针的正确释放逻辑并处理可能的异常。
template<class _Ty>
struct default_deleter {
    void operator()(_Ty* ptr) const {
        delete ptr;
    }
};

// 数组特化删除器：动态数组释放
// 说明：对于托管数组类型（T[]），必须使用 delete[] 来匹配 new[] 的分配方式。
// 该特化确保数组版本的 my_unique_ptr 调用正确的释放操作。
template<class _Ty>
struct default_deleter<_Ty[]> {
    void operator()(_Ty* ptr) const {
        delete[] ptr;
    }
};

// 自定义文件删除器：适配 FILE* 文件指针的资源释放
// 说明：演示如何为非内存资源（如 FILE*）提供自定义删除器。
// 当 my_unique_ptr<FILE, DelFile> 的实例被销毁或 reset 时，DelFile 会被调用并关闭文件。
// 注意：fclose 本身可能失败，但在一般的 RAII 场景下我们只需尝试关闭并忽略错误。
struct DelFile {
    void operator()(FILE* fp) const {
        if (fp != nullptr) {
            fclose(fp);
        }
    }
};

// 自定义前缀my_unique_ptr，替代原有unique_ptr
template<class _Ty, class _Dx = default_deleter<_Ty>>
class my_unique_ptr {
public:
    using pointer = _Ty*;
    using element_type = _Ty;
    using deleter_type = _Dx;
private:
    pointer mPtr;
    deleter_type mDeleter;
public:
    // 构造函数
    // 构造函数：可选地接管一个裸指针的所有权
    // 注意：deleter 使用缺省构造函数构造（可以携带状态的删除器也会被默认构造）
    // 语义：my_unique_ptr<int> p(new int(1)); p 管理该指针，离开作用域时自动释放
    // 该构造函数允许用户传入一个裸指针，并接管其所有权
    explicit my_unique_ptr(pointer p = nullptr) : mPtr(p), mDeleter() {}

    // 禁用拷贝构造与拷贝赋值
    my_unique_ptr(const my_unique_ptr&) = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;

    // 析构函数：自动释放资源
    // 说明：析构时若持有资源则通过删除器释放。这是 RAII 的核心。
    ~my_unique_ptr() {
        if (mPtr != nullptr) {
            reset();
        }
    }

    // 移动构造函数（仅同类型）
    // 说明：移动构造会把 other 的裸指针接管到当前对象，并把 other 置空。
    // 同时把删除器的内部状态（如果有）移动到当前对象。
    my_unique_ptr(my_unique_ptr&& other) noexcept {
        mPtr = other.release();
        mDeleter = std::move(other.mDeleter);
    }

    // 移动赋值函数（仅同类型）
    // 说明：通过先释放自身资源（如果存在），再接管 other 的资源与删除器状态来实现。
    my_unique_ptr& operator=(my_unique_ptr&& other) noexcept {
        if (this->get() != other.get()) {
            reset(other.release());
            mDeleter = std::move(other.mDeleter);
        }
        return *this;
    }
    // 释放所有权：断开指针关联，返回原指针
    // 说明：调用者接收到返回的裸指针后必须负责释放资源。
    pointer release() {
        pointer old = mPtr;
        mPtr = nullptr;
        return old;
    }

    // 重置管理对象
    // 说明：如果当前持有资源则先调用删除器释放，然后接管新的裸指针 ptr。
    void reset(pointer ptr = nullptr) {
        if (mPtr != nullptr) {
            mDeleter(mPtr);
        }
        mPtr = ptr;
    }

    // 交换两个my_unique_ptr资源
    // swap：交换两个智能指针的裸指针与删除器
    // 特点：用作实现异常安全的操作（如赋值的拷贝-交换惯用法），这里为无抛异常操作
    void swap(my_unique_ptr& other) noexcept {
        std::swap(this->mPtr, other.mPtr);
        std::swap(this->mDeleter, other.mDeleter);
    }

    // 获取内部裸指针
    // get：返回内部裸指针（不改变所有权）
    pointer get() const {
        return mPtr;
    }

    // 获取删除器
    // get_deleter：获取删除器对象的引用（可读/可写），有助于访问或修改删除器的内部状态
    _Dx& get_deleter() { return mDeleter; }
    const _Dx& get_deleter() const { return mDeleter; }

    // 布尔转换：判断是否持有有效指针
    // 布尔转换：智能指针在布尔上下文中为真时表示持有非空指针
    explicit operator bool() const {
        return mPtr != nullptr;
    }

    // 运算符重载
    // 解引用运算符：提供类似裸指针的访问方式
    // 警告：对空指针解引用将导致未定义行为
    _Ty& operator*() const {
        return *mPtr;
    }
    pointer operator->() const {
        return get();
    }
};

// 数组版本特化，支持[]运算符访问
template<class _Ty, class _Dx>
class my_unique_ptr<_Ty[], _Dx> {
public:
    using pointer = _Ty*;
    using element_type = _Ty;
    using deleter_type = _Dx;
private:
    pointer mPtr;
    deleter_type mDeleter;
public:
    my_unique_ptr(pointer p = nullptr) : mPtr(p), mDeleter() {}
    my_unique_ptr(const my_unique_ptr&) = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;
    ~my_unique_ptr() {
        if (mPtr != nullptr) reset();
    }
    // 移动相关函数（仅同类型）
    my_unique_ptr(my_unique_ptr&& other) noexcept {
        mPtr = other.release();
        mDeleter = std::move(other.mDeleter);
    }
    my_unique_ptr& operator=(my_unique_ptr&& other) noexcept {
        if (this->get() != other.get()) {
            reset(other.release());
            mDeleter = std::move(other.mDeleter);
        }
        return *this;
    }
    // 数组访问运算符
    _Ty& operator[](std::size_t i) const {
        return get()[i];
    }
    // 基础函数实现
    pointer release() {
        pointer old = mPtr;
        mPtr = nullptr;
        return old;
    }
    void reset(pointer ptr = nullptr) {
        if (mPtr != nullptr) {
            mDeleter(mPtr);
        }
        mPtr = ptr;
    }
    void swap(my_unique_ptr& other) {
        std::swap(mPtr, other.mPtr);
        std::swap(mDeleter, other.mDeleter);
    }
    pointer get() const { return mPtr; }
    explicit operator bool() const { return mPtr != nullptr; }
};

// 自定义文件指针智能指针函数：搭配DelFile删除器使用
my_unique_ptr<FILE, DelFile> pa() {
    // 以只读方式打开文件，失败返回空指针
    FILE* fp = fopen("test.txt", "r");
    // 构造my_unique_ptr返回，自动绑定文件删除器
    return my_unique_ptr<FILE, DelFile>(fp);
}

// 调用示例：无需手动调用fclose，离开作用域自动关闭文件
void test_file_ptr() {
    auto file_ptr = pa();
    if (file_ptr) {
        // 文件操作逻辑
        char buf[1024] = { 0 };
        fread(buf, 1, sizeof(buf), file_ptr.get());
    }
    // 离开作用域，DelFile自动调用fclose关闭文件，无资源泄漏
}

// 测试函数：演示my_unique_ptr的基本使用
void test_my_unique_ptr() {
    printf("--- test_my_unique_ptr start ---\n");

    // 单对象基本功能：构造、解引用
    my_unique_ptr<int> p1(new int(42));
    printf("p1 value = %d\n", *p1);

    // release -> caller负责释放原始指针
    int* raw = p1.release();
    printf("after release, p1 is %s, raw=%d\n", p1 ? "not-null" : "null", raw ? *raw : 0);
    delete raw; // 手动释放

    // 移动构造与移动赋值
    my_unique_ptr<int> p2(new int(100));
    my_unique_ptr<int> p3(std::move(p2));
    printf("after move construct, p2 is %s, p3=%d\n", p2 ? "not-null" : "null", *p3);
    my_unique_ptr<int> p4;
    p4 = std::move(p3);
    printf("after move assign, p3 is %s, p4=%d\n", p3 ? "not-null" : "null", *p4);

    // reset
    p4.reset(new int(200));
    printf("after reset, p4=%d\n", *p4);

    // swap
    my_unique_ptr<int> a(new int(1));
    my_unique_ptr<int> b(new int(2));
    printf("before swap a=%d b=%d\n", *a, *b);
    a.swap(b);
    printf("after swap a=%d b=%d\n", *a, *b);

    // 动态数组版本
    my_unique_ptr<int[]> parr(new int[5]{10, 20, 30, 40, 50});
    printf("parr: %d %d %d %d %d\n", parr[0], parr[1], parr[2], parr[3], parr[4]);
    my_unique_ptr<int[]> parr2(std::move(parr));
    printf("after move, parr is %s, parr2[0]=%d\n", parr ? "not-null" : "null", parr2[0]);

    // 文件指针示例：创建临时文件，验证DelFile删除器能正确关闭文件
    const char* tmpname = "test_temp.txt";
    {
        FILE* wf = fopen(tmpname, "w");
        if (wf) {
            fprintf(wf, "hello world\n");
            fclose(wf);
        }
    }
    FILE* rf = fopen(tmpname, "r");
    my_unique_ptr<FILE, DelFile> fptr(rf);
    if (fptr) {
        char buf[64] = {0};
        size_t n = fread(buf, 1, sizeof(buf)-1, fptr.get());
        printf("read %zu bytes: %s", n, buf);
    }

    // 清理临时文件
    remove(tmpname);

    printf("--- test_my_unique_ptr end ---\n");
}
#if 0
int main() {
    test_my_unique_ptr();
    return 0;
}
#endif