/**
 * Day 33: 树路径问题 - 主程序
 */

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>

// 二叉树节点
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 前向声明
void treePathDemo();
void threadPoolDemo();
void emcppDemo();

// 路径问题：所有根到叶子路径
void findAllPaths(TreeNode* root, std::vector<int>& path, std::vector<std::vector<int>>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);
    
    if (root->left == nullptr && root->right == nullptr) {
        result.push_back(path);
    } else {
        findAllPaths(root->left, path, result);
        findAllPaths(root->right, path, result);
    }
    
    path.pop_back();  // 回溯
}

// 删除树
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::cout << "=== Day 33: 树路径问题 ===" << std::endl;
    
    // 1. 树路径问题演示
    std::cout << "\n--- 1. 树路径问题 ---" << std::endl;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    std::vector<std::vector<int>> paths;
    std::vector<int> path;
    findAllPaths(root, path, paths);
    
    std::cout << "所有根到叶子路径:" << std::endl;
    for (const auto& p : paths) {
        std::cout << "  ";
        for (size_t i = 0; i < p.size(); ++i) {
            std::cout << p[i];
            if (i < p.size() - 1) std::cout << "->";
        }
        std::cout << std::endl;
    }
    deleteTree(root);
    
    // 2. 线程池演示
    std::cout << "\n--- 2. 线程池 ---" << std::endl;
    threadPoolDemo();
    
    // 3. EMC++ Item 11-16
    std::cout << "\n--- 3. EMC++ Item 11-16 ---" << std::endl;
    emcppDemo();
    
    std::cout << "\n=== Day 33 学习完成 ===" << std::endl;
    return 0;
}

// 简单线程池
class SimpleThreadPool {
public:
    SimpleThreadPool(size_t numThreads) : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }
    
    ~SimpleThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& w : workers_) w.join();
    }
    
    void enqueue(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.push(std::move(task));
        }
        cv_.notify_one();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;
};

void threadPoolDemo() {
    SimpleThreadPool pool(3);
    std::atomic<int> counter{0};
    
    for (int i = 0; i < 5; ++i) {
        pool.enqueue([&counter, i]() {
            std::cout << "  任务 " << i << " 执行中" << std::endl;
            counter++;
        });
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "完成的任务数: " << counter.load() << std::endl;
}

void emcppDemo() {
    std::cout << "Item 11: 优先使用 = delete 而非 private 未定义" << std::endl;
    std::cout << "Item 12: 覆盖函数使用 override 关键字" << std::endl;
    std::cout << "Item 13: 优先使用 const_iterator" << std::endl;
    std::cout << "Item 14: 不抛异常的函数声明 noexcept" << std::endl;
    std::cout << "Item 15: 尽可能使用 constexpr" << std::endl;
    std::cout << "Item 16: const成员函数要线程安全" << std::endl;
}
