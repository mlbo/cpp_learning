/**
 * 条件变量(Condition Variable)演示
 * 用于线程间的等待/通知机制
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

// 生产者-消费者示例
void producerConsumerDemo() {
    std::cout << "=== 条件变量演示：生产者-消费者 ===" << std::endl;
    
    std::queue<int> dataQueue;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;
    
    // 生产者线程
    std::thread producer([&]() {
        for (int i = 1; i <= 5; ++i) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                dataQueue.push(i);
                std::cout << "生产: " << i << std::endl;
            }
            cv.notify_one();  // 通知消费者
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            finished = true;
        }
        cv.notify_one();
    });
    
    // 消费者线程
    std::thread consumer([&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]{ return !dataQueue.empty() || finished; });
            
            while (!dataQueue.empty()) {
                int data = dataQueue.front();
                dataQueue.pop();
                std::cout << "消费: " << data << std::endl;
            }
            
            if (finished) break;
        }
    });
    
    producer.join();
    consumer.join();
    
    std::cout << "生产者-消费者演示完成" << std::endl;
}

// 线程安全队列
template<typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(value));
        }
        cv_.notify_one();
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]{ return !queue_.empty(); });
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }

private:
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> queue_;
};

int main() {
    producerConsumerDemo();
    return 0;
}
