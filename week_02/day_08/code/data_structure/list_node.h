/**
 * @file list_node.h
 * @brief 链表节点定义和基本声明
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 本文件定义了单链表节点结构，以及链表操作函数的声明。
 * 支持多种节点类型：单链表、双向链表、带虚拟头节点的链表。
 */

#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <iostream>
#include <vector>
#include <memory>

// ============================================================
// 单链表节点定义
// ============================================================

/**
 * @brief 单链表节点结构
 * 
 * 最基础的链表节点，包含数据域和指向下一个节点的指针。
 */
struct ListNode {
    int val;           ///< 数据域：存储节点值
    ListNode* next;    ///< 指针域：指向下一个节点
    
    /**
     * @brief 构造函数
     * @param x 节点值，默认为0
     * @param n 下一个节点指针，默认为nullptr
     */
    explicit ListNode(int x = 0, ListNode* n = nullptr) 
        : val(x), next(n) {}
    
    /**
     * @brief 析构函数
     * @note 不自动删除next指向的节点，避免级联删除
     */
    ~ListNode() = default;
};

// ============================================================
// 双向链表节点定义
// ============================================================

/**
 * @brief 双向链表节点结构
 */
struct DoublyListNode {
    int val;                    ///< 数据域
    DoublyListNode* prev;       ///< 前驱指针
    DoublyListNode* next;       ///< 后继指针
    
    explicit DoublyListNode(int x = 0) 
        : val(x), prev(nullptr), next(nullptr) {}
};

// ============================================================
// 链表基本操作声明
// ============================================================

namespace list_ops {

// ==================== 创建与销毁 ====================

/**
 * @brief 从vector创建链表
 * @param values 值数组
 * @return 链表头节点指针，如果数组为空返回nullptr
 * 
 * @example
 *   std::vector<int> v = {1, 2, 3, 4, 5};
 *   ListNode* head = createList(v);
 */
ListNode* createList(const std::vector<int>& values);

/**
 * @brief 从初始化列表创建链表
 * @param values 初始化列表
 * @return 链表头节点指针
 */
ListNode* createList(std::initializer_list<int> values);

/**
 * @brief 创建带虚拟头节点的链表
 * @param values 值数组
 * @return 虚拟头节点指针
 * @note 虚拟头节点的next指向真正的头节点
 */
ListNode* createListWithDummy(const std::vector<int>& values);

/**
 * @brief 释放链表内存
 * @param head 链表头节点
 * @note 释放所有节点内存，包括head
 */
void deleteList(ListNode* head);

/**
 * @brief 创建链表的深拷贝
 * @param head 源链表头节点
 * @return 新链表头节点
 */
ListNode* copyList(const ListNode* head);

// ==================== 遍历与访问 ====================

/**
 * @brief 打印链表内容
 * @param head 链表头节点
 * @param os 输出流，默认为std::cout
 */
void printList(const ListNode* head, std::ostream& os = std::cout);

/**
 * @brief 获取链表长度
 * @param head 链表头节点
 * @return 链表节点数量
 */
int getLength(const ListNode* head);

/**
 * @brief 获取链表的第n个节点（从0开始）
 * @param head 链表头节点
 * @param n 索引
 * @return 第n个节点指针，如果索引越界返回nullptr
 */
ListNode* getNode(ListNode* head, int n);
const ListNode* getNode(const ListNode* head, int n);

/**
 * @brief 获取链表最后一个节点
 * @param head 链表头节点
 * @return 最后一个节点指针，如果链表为空返回nullptr
 */
ListNode* getLastNode(ListNode* head);

/**
 * @brief 将链表转换为vector
 * @param head 链表头节点
 * @return 包含所有节点值的vector
 */
std::vector<int> toVector(const ListNode* head);

// ==================== 插入操作 ====================

/**
 * @brief 在链表头部插入节点
 * @param head 链表头节点引用
 * @param val 要插入的值
 * @return 新的头节点
 */
ListNode* insertHead(ListNode*& head, int val);

/**
 * @brief 在链表尾部插入节点
 * @param head 链表头节点引用
 * @param val 要插入的值
 * @return 链表头节点
 */
ListNode* insertTail(ListNode*& head, int val);

/**
 * @brief 在指定节点后插入新节点
 * @param node 指定节点
 * @param val 要插入的值
 * @return 新插入的节点
 * @note 如果node为nullptr，返回nullptr
 */
ListNode* insertAfter(ListNode* node, int val);

/**
 * @brief 在指定位置插入节点
 * @param head 链表头节点引用
 * @param index 插入位置（0表示在头部插入）
 * @param val 要插入的值
 * @return 链表头节点
 */
ListNode* insertAt(ListNode*& head, int index, int val);

// ==================== 删除操作 ====================

/**
 * @brief 删除链表头节点
 * @param head 链表头节点引用
 * @return 新的头节点
 */
ListNode* deleteHead(ListNode*& head);

/**
 * @brief 删除链表尾节点
 * @param head 链表头节点引用
 * @return 链表头节点
 */
ListNode* deleteTail(ListNode*& head);

/**
 * @brief 删除指定节点后的节点
 * @param node 指定节点
 * @return 被删除节点的值，如果没有节点被删除返回-1
 */
int deleteAfter(ListNode* node);

/**
 * @brief 删除第一个值为val的节点
 * @param head 链表头节点引用
 * @param val 要删除的值
 * @return 是否成功删除
 */
bool deleteFirst(ListNode*& head, int val);

/**
 * @brief 删除所有值为val的节点
 * @param head 链表头节点引用
 * @param val 要删除的值
 * @return 删除的节点数量
 */
int deleteAll(ListNode*& head, int val);

// ==================== 查找操作 ====================

/**
 * @brief 查找值为val的第一个节点
 * @param head 链表头节点
 * @param val 目标值
 * @return 找到的节点指针，未找到返回nullptr
 */
ListNode* find(ListNode* head, int val);
const ListNode* find(const ListNode* head, int val);

/**
 * @brief 检查值是否存在于链表中
 * @param head 链表头节点
 * @param val 目标值
 * @return 是否存在
 */
bool contains(const ListNode* head, int val);

/**
 * @brief 查找值为val的节点的前驱节点
 * @param head 链表头节点
 * @param val 目标值
 * @return 前驱节点指针，如果目标是头节点返回nullptr
 */
ListNode* findPrev(ListNode* head, int val);

// ==================== 反转操作 ====================

/**
 * @brief 反转链表（迭代法）
 * @param head 链表头节点
 * @return 反转后的头节点
 */
ListNode* reverse(ListNode* head);

/**
 * @brief 反转链表（递归法）
 * @param head 链表头节点
 * @return 反转后的头节点
 */
ListNode* reverseRecursive(ListNode* head);

/**
 * @brief 反转链表的前n个节点
 * @param head 链表头节点
 * @param n 要反转的节点数量
 * @return 反转后的头节点
 */
ListNode* reverseN(ListNode* head, int n);

/**
 * @brief 反转链表从位置m到n的节点
 * @param head 链表头节点
 * @param m 起始位置（从1开始）
 * @param n 结束位置
 * @return 反转后的头节点
 */
ListNode* reverseBetween(ListNode* head, int m, int n);

// ==================== 合并与拆分 ====================

/**
 * @brief 合并两个有序链表
 * @param l1 第一个有序链表
 * @param l2 第二个有序链表
 * @return 合并后的链表头节点
 */
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2);

/**
 * @brief 寻找链表中点
 * @param head 链表头节点
 * @return 中点节点指针（偶数长度返回前一个中点）
 */
ListNode* findMiddle(ListNode* head);

// ==================== 环检测 ====================

/**
 * @brief 检测链表是否有环
 * @param head 链表头节点
 * @return 是否有环
 */
bool hasCycle(const ListNode* head);

/**
 * @brief 找到环的入口节点
 * @param head 链表头节点
 * @return 环入口节点，无环返回nullptr
 */
ListNode* detectCycle(ListNode* head);

// ==================== 辅助函数 ====================

/**
 * @brief 比较两个链表是否相等
 * @param l1 第一个链表
 * @param l2 第二个链表
 * @return 是否相等
 */
bool isEqual(const ListNode* l1, const ListNode* l2);

/**
 * @brief 判断链表是否为空
 * @param head 链表头节点
 * @return 是否为空
 */
inline bool isEmpty(const ListNode* head) {
    return head == nullptr;
}

} // namespace list_ops

// ============================================================
// 使用unique_ptr的链表节点（现代C++风格）
// ============================================================

namespace modern_list {

/**
 * @brief 使用unique_ptr管理的链表节点
 * 
 * 自动管理内存，但需要注意所有权转移。
 */
struct UniqueListNode {
    int val;
    std::unique_ptr<UniqueListNode> next;
    
    explicit UniqueListNode(int x = 0) : val(x), next(nullptr) {}
};

// 使用unique_ptr的链表创建函数
std::unique_ptr<UniqueListNode> createUniqueList(const std::vector<int>& values);
void printUniqueList(const UniqueListNode* head);

} // namespace modern_list

#endif // LIST_NODE_H
