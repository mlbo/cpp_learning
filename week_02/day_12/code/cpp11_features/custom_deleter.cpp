/**
 * @file custom_deleter.cpp
 * @brief 智能指针自定义删除器汇总
 * 
 * 本文件展示：
 * 1. unique_ptr 自定义删除器
 * 2. shared_ptr 自定义删除器
 * 3. 各种资源的删除器实现
 */

#include <iostream>
#include <memory>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <array>

// ============================================
// 辅助打印
// ============================================

#define PRINT_SECTION(title) std::cout << "\n=== " << title << " ===\n"

// ============================================
// 第一部分：unique_ptr 自定义删除器
// ============================================

// 方法1: 函数指针作为删除器
void file_deleter(FILE* f) {
    if (f) {
        std::cout << "  [删除器] 关闭文件\n";
        fclose(f);
    }
}

// 方法2: 函数对象（仿函数）
struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) {
            std::cout << "  [仿函数删除器] 关闭文件\n";
            fclose(f);
        }
    }
};

// 方法3: Lambda 表达式
void demo_unique_ptr_deleters() {
    PRINT_SECTION("1. unique_ptr 自定义删除器");
    
    // 1.1 函数指针
    std::cout << "\n--- 1.1 函数指针删除器 ---\n";
    {
        // 必须指定删除器类型
        std::unique_ptr<FILE, void(*)(FILE*)> file(
            fopen("/tmp/test1.txt", "w"),
            file_deleter
        );
        if (file) {
            fprintf(file.get(), "Hello, function pointer deleter!\n");
            std::cout << "  写入文件成功\n";
        }
    }
    
    // 1.2 仿函数
    std::cout << "\n--- 1.2 仿函数删除器 ---\n";
    {
        // 使用仿函数类型
        std::unique_ptr<FILE, FileDeleter> file(fopen("/tmp/test2.txt", "w"));
        if (file) {
            fprintf(file.get(), "Hello, functor deleter!\n");
            std::cout << "  写入文件成功\n";
        }
    }
    
    // 1.3 Lambda 表达式（最常用）
    std::cout << "\n--- 1.3 Lambda 删除器 ---\n";
    {
        auto deleter = [](FILE* f) {
            if (f) {
                std::cout << "  [Lambda删除器] 关闭文件\n";
                fclose(f);
            }
        };
        
        // 使用 decltype 推导删除器类型
        std::unique_ptr<FILE, decltype(deleter)> file(
            fopen("/tmp/test3.txt", "w"),
            deleter
        );
        if (file) {
            fprintf(file.get(), "Hello, lambda deleter!\n");
            std::cout << "  写入文件成功\n";
        }
    }
    
    // 1.4 数组删除器（默认特化）
    std::cout << "\n--- 1.4 数组删除器 ---\n";
    {
        // unique_ptr<T[]> 自动使用 delete[]
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }
        std::cout << "  数组元素: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
    }
}

// ============================================
// 第二部分：shared_ptr 自定义删除器
// ============================================

void demo_shared_ptr_deleters() {
    PRINT_SECTION("2. shared_ptr 自定义删除器");
    
    // 2.1 Lambda 删除器（更简洁）
    std::cout << "\n--- 2.1 Lambda 删除器 ---\n";
    {
        // shared_ptr 不需要指定删除器类型
        std::shared_ptr<FILE> file(
            fopen("/tmp/test4.txt", "w"),
            [](FILE* f) {
                if (f) {
                    std::cout << "  [shared_ptr删除器] 关闭文件\n";
                    fclose(f);
                }
            }
        );
        if (file) {
            fprintf(file.get(), "Hello, shared_ptr!\n");
            std::cout << "  写入文件成功\n";
        }
        
        // 可以共享
        auto file2 = file;
        std::cout << "  引用计数: " << file.use_count() << "\n";
    }
    
    // 2.2 管理动态数组
    std::cout << "\n--- 2.2 管理动态数组 ---\n";
    {
        // 使用自定义删除器管理数组
        std::shared_ptr<int> arr(
            new int[5]{1, 2, 3, 4, 5},
            [](int* p) {
                std::cout << "  [数组删除器] delete[]\n";
                delete[] p;
            }
        );
        std::cout << "  数组元素: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr.get()[i] << " ";
        }
        std::cout << "\n";
    }
    
    // 2.3 空指针删除器（不释放资源）
    std::cout << "\n--- 2.3 空删除器 ---\n";
    {
        int value = 100;
        // 使用空删除器，不释放栈变量
        std::shared_ptr<int> ptr(&value, [](int*) {
            std::cout << "  [空删除器] 不执行任何操作\n";
        });
        std::cout << "  值: " << *ptr << "\n";
        // value 是栈变量，不需要 delete
    }
}

// ============================================
// 第三部分：常见资源删除器
// ============================================

// POSIX 文件描述符删除器
struct FdDeleter {
    void operator()(int* fd) const;
};

// 前向声明实现
void FdDeleter::operator()(int* fd) const {
    if (fd && *fd >= 0) {
        std::cout << "  [FD删除器] close(" << *fd << ")\n";
        // 实际代码: close(*fd);
        delete fd;
    }
}

// C 风格字符串删除器
struct CStringDeleter {
    void operator()(char* s) const {
        if (s) {
            std::cout << "  [CString删除器] free()\n";
            free(s);
        }
    }
};

void demo_common_deleters() {
    PRINT_SECTION("3. 常见资源删除器");
    
    // 3.1 文件描述符
    std::cout << "\n--- 3.1 文件描述符 ---\n";
    {
        // 模拟文件描述符
        std::unique_ptr<int, FdDeleter> fd(new int(3));  // 假设 fd = 3
        std::cout << "  文件描述符: " << *fd << "\n";
    }
    
    // 3.2 C 风格字符串（malloc 分配）
    std::cout << "\n--- 3.2 C 风格字符串 ---\n";
    {
        char* s = (char*)std::malloc(20);
        std::strcpy(s, "Hello, World!");
        std::unique_ptr<char, CStringDeleter> str(s);
        std::cout << "  字符串: " << str.get() << "\n";
    }
    
    // 3.3 自定义类资源
    std::cout << "\n--- 3.3 数据库连接（示例） ---\n";
    {
        // 模拟数据库连接
        struct DBConnection {
            int handle;
            DBConnection(int h) : handle(h) {}
        };
        
        auto dbDeleter = [](DBConnection* conn) {
            if (conn) {
                std::cout << "  [DB删除器] 关闭连接 handle=" << conn->handle << "\n";
                // 实际代码: db_close(conn->handle);
                delete conn;
            }
        };
        
        std::unique_ptr<DBConnection, decltype(dbDeleter)> db(
            new DBConnection(12345),
            dbDeleter
        );
        std::cout << "  数据库连接 handle: " << db->handle << "\n";
    }
    
    // 3.4 网络套接字（示例）
    std::cout << "\n--- 3.4 网络套接字 ---\n";
    {
        auto socketDeleter = [](int* sock) {
            if (sock && *sock >= 0) {
                std::cout << "  [Socket删除器] closesocket(" << *sock << ")\n";
                // 实际代码: closesocket(*sock);
                delete sock;
            }
        };
        
        std::unique_ptr<int, decltype(socketDeleter)> sock(
            new int(8080),
            socketDeleter
        );
        std::cout << "  套接字端口: " << *sock << "\n";
    }
}

// ============================================
// 第四部分：类型别名简化
// ============================================

// 定义常用别名
template<typename T>
using UniqueFile = std::unique_ptr<FILE, void(*)(FILE*)>;

template<typename T>
using UniqueArray = std::unique_ptr<T[]>;

// 使用别名简化代码
void demo_type_aliases() {
    PRINT_SECTION("4. 使用类型别名");
    
    std::cout << "\n--- 使用预定义类型别名 ---\n";
    
    // 使用别名简化声明
    UniqueFile<int> file(fopen("/tmp/test5.txt", "w"), file_deleter);
    if (file) {
        fprintf(file.get(), "Using type alias!\n");
        std::cout << "  使用别名简化声明成功\n";
    }
    
    // 数组别名
    UniqueArray<int> arr = std::make_unique<int[]>(3);
    arr[0] = 10; arr[1] = 20; arr[2] = 30;
    std::cout << "  数组: " << arr[0] << ", " << arr[1] << ", " << arr[2] << "\n";
}

// ============================================
// 第五部分：完整示例
// ============================================

class ResourceManager {
public:
    // 使用智能指针管理文件
    using FilePtr = std::unique_ptr<FILE, void(*)(FILE*)>;
    
    static FilePtr openFile(const char* filename, const char* mode) {
        return FilePtr(fopen(filename, mode), file_deleter);
    }
    
    // 使用 shared_ptr 管理共享资源
    static std::shared_ptr<char[]> allocateBuffer(size_t size) {
        return std::shared_ptr<char[]>(
            new char[size],
            [](char* p) {
                std::cout << "  [Buffer删除器] 释放缓冲区\n";
                delete[] p;
            }
        );
    }
};

void demo_resource_manager() {
    PRINT_SECTION("5. 资源管理器示例");
    
    // 打开文件
    auto file = ResourceManager::openFile("/tmp/test6.txt", "w");
    if (file) {
        fprintf(file.get(), "Resource manager demo\n");
        std::cout << "  文件写入成功\n";
    }
    
    // 分配缓冲区
    auto buffer = ResourceManager::allocateBuffer(100);
    std::strcpy(buffer.get(), "Hello, Buffer!");
    std::cout << "  缓冲区内容: " << buffer.get() << "\n";
    
    // 共享缓冲区
    auto buffer2 = buffer;
    std::cout << "  缓冲区引用计数: " << buffer.use_count() << "\n";
}

// ============================================
// 主函数
// ============================================

void run_custom_deleter() {
    std::cout << "\n========================================\n";
    std::cout << "  智能指针自定义删除器汇总\n";
    std::cout << "========================================\n";
    
    demo_unique_ptr_deleters();
    demo_shared_ptr_deleters();
    demo_common_deleters();
    demo_type_aliases();
    demo_resource_manager();
    
    std::cout << "\n========================================\n";
    std::cout << "  自定义删除器演示完毕\n";
    std::cout << "========================================\n";
}
