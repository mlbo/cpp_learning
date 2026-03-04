/**
 * 堆数据结构演示
 * 展示堆的基本操作和原理
 */

#include "heap_demo.h"
#include <iostream>
#include <vector>
#include <algorithm>

// 手动实现一个小顶堆
class MinHeap {
private:
    std::vector<int> data;
    
    // 获取父节点索引
    int parent(int i) { return (i - 1) / 2; }
    
    // 获取左子节点索引
    int left(int i) { return 2 * i + 1; }
    
    // 获取右子节点索引
    int right(int i) { return 2 * i + 2; }
    
    // 向上调整
    void heapifyUp(int i) {
        while (i > 0 && data[i] < data[parent(i)]) {
            std::swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }
    
    // 向下调整
    void heapifyDown(int i) {
        int size = data.size();
        int smallest = i;
        
        while (true) {
            int l = left(i);
            int r = right(i);
            
            if (l < size && data[l] < data[smallest]) {
                smallest = l;
            }
            if (r < size && data[r] < data[smallest]) {
                smallest = r;
            }
            
            if (smallest == i) break;
            
            std::swap(data[i], data[smallest]);
            i = smallest;
        }
    }
    
public:
    // 插入
    void push(int val) {
        data.push_back(val);
        heapifyUp(data.size() - 1);
    }
    
    // 弹出堆顶
    void pop() {
        if (data.empty()) return;
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) {
            heapifyDown(0);
        }
    }
    
    // 获取堆顶
    int top() {
        return data.empty() ? -1 : data[0];
    }
    
    // 大小
    int size() {
        return data.size();
    }
    
    // 是否为空
    bool empty() {
        return data.empty();
    }
    
    // 打印堆结构
    void print() {
        std::cout << "  堆数组: [";
        for (int i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

void heapDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        堆 Heap 数据结构              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 小顶堆演示 ==========
    std::cout << "\n--- 1. 小顶堆操作演示 ---" << std::endl;
    
    MinHeap minHeap;
    
    std::cout << "  插入: 5, 3, 8, 1, 2" << std::endl;
    minHeap.push(5);
    minHeap.print();
    minHeap.push(3);
    minHeap.print();
    minHeap.push(8);
    minHeap.print();
    minHeap.push(1);
    minHeap.print();
    minHeap.push(2);
    minHeap.print();
    
    std::cout << "\n  弹出堆顶:" << std::endl;
    while (!minHeap.empty()) {
        std::cout << "    弹出: " << minHeap.top() << std::endl;
        minHeap.pop();
        minHeap.print();
    }
    
    // ========== 2. 堆的性质 ==========
    std::cout << "\n--- 2. 堆的性质 ---" << std::endl;
    std::cout << "  小顶堆：每个节点 ≤ 子节点" << std::endl;
    std::cout << "  大顶堆：每个节点 ≥ 子节点" << std::endl;
    std::cout << "  完全二叉树：用数组存储" << std::endl;
    
    // ========== 3. 数组索引关系 ==========
    std::cout << "\n--- 3. 数组索引关系 ---" << std::endl;
    std::cout << "  对于节点 i：" << std::endl;
    std::cout << "    父节点: (i-1) / 2" << std::endl;
    std::cout << "    左子节点: 2*i + 1" << std::endl;
    std::cout << "    右子节点: 2*i + 2" << std::endl;
    
    // ========== 4. 时间复杂度 ==========
    std::cout << "\n--- 4. 时间复杂度 ---" << std::endl;
    std::cout << "  插入: O(log n) - 向上调整" << std::endl;
    std::cout << "  删除堆顶: O(log n) - 向下调整" << std::endl;
    std::cout << "  获取堆顶: O(1)" << std::endl;
    std::cout << "  建堆: O(n) - 从下往上调整" << std::endl;
    
    // ========== 5. STL make_heap演示 ==========
    std::cout << "\n--- 5. STL heap操作 ---" << std::endl;
    
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    std::cout << "  原数组: [";
    for (int n : v) std::cout << n << " ";
    std::cout << "]" << std::endl;
    
    // 建大顶堆
    std::make_heap(v.begin(), v.end());
    std::cout << "  make_heap后: [";
    for (int n : v) std::cout << n << " ";
    std::cout << "]" << std::endl;
    
    // 添加元素
    v.push_back(10);
    std::push_heap(v.begin(), v.end());
    std::cout << "  push_heap(10)后: [";
    for (int n : v) std::cout << n << " ";
    std::cout << "]" << std::endl;
    
    // 弹出堆顶
    std::pop_heap(v.begin(), v.end());
    std::cout << "  pop_heap后: [";
    for (int n : v) std::cout << n << " ";
    std::cout << "]" << std::endl;
    std::cout << "  弹出的元素: " << v.back() << std::endl;
    v.pop_back();
}
