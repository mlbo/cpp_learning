/**
 * @file observer_pattern.cpp
 * @brief 观察者模式中的 weak_ptr 应用
 * 
 * 涵盖内容：
 * 1. 观察者模式基础实现
 * 2. 使用 weak_ptr 管理观察者
 * 3. 通知机制的安全实现
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

// ============================================
// 1. 观察者模式基础实现
// ============================================

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(const std::string& message) = 0;
    virtual std::string name() const = 0;
};

class Subject {
public:
    // 使用 weak_ptr 存储观察者
    void attach(const std::shared_ptr<Observer>& observer) {
        observers_.push_back(observer);
        std::cout << "  📎 附加观察者: " << observer->name() << "\n";
    }
    
    void detach(const std::shared_ptr<Observer>& observer) {
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [&observer](const std::weak_ptr<Observer>& wp) {
                    if (auto sp = wp.lock()) {
                        return sp == observer;
                    }
                    return false;  // 已销毁的观察者保留，稍后清理
                }),
            observers_.end()
        );
        std::cout << "  🔓 分离观察者: " << observer->name() << "\n";
    }
    
    void notify(const std::string& message) {
        std::cout << "  📢 广播消息: \"" << message << "\"\n";
        
        // 清理已销毁的观察者
        cleanExpiredObservers();
        
        // 通知存活的观察者
        for (auto& wp : observers_) {
            if (auto sp = wp.lock()) {
                sp->onNotify(message);
            }
        }
    }
    
    size_t observerCount() const {
        size_t count = 0;
        for (const auto& wp : observers_) {
            if (!wp.expired()) count++;
        }
        return count;
    }

private:
    std::vector<std::weak_ptr<Observer>> observers_;
    
    void cleanExpiredObservers() {
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [](const std::weak_ptr<Observer>& wp) {
                    return wp.expired();
                }),
            observers_.end()
        );
    }
};

// ============================================
// 2. 具体观察者实现
// ============================================

class EmailObserver : public Observer {
public:
    explicit EmailObserver(const std::string& email) 
        : email_(email) {}
    
    void onNotify(const std::string& message) override {
        std::cout << "    📧 [Email:" << email_ << "] " << message << "\n";
    }
    
    std::string name() const override { return "Email:" + email_; }

private:
    std::string email_;
};

class SMSObserver : public Observer {
public:
    explicit SMSObserver(const std::string& phone) 
        : phone_(phone) {}
    
    void onNotify(const std::string& message) override {
        std::cout << "    📱 [SMS:" << phone_ << "] " << message << "\n";
    }
    
    std::string name() const override { return "SMS:" + phone_; }

private:
    std::string phone_;
};

class LogObserver : public Observer {
public:
    explicit LogObserver(const std::string& filename) 
        : filename_(filename) {}
    
    void onNotify(const std::string& message) override {
        std::cout << "    📝 [Log:" << filename_ << "] " << message << "\n";
    }
    
    std::string name() const override { return "Log:" + filename_; }

private:
    std::string filename_;
};

// ============================================
// 3. 观察者模式演示
// ============================================

void demo_observer_basic() {
    std::cout << "\n【1. 观察者模式基础实现】\n";
    
    Subject subject;
    
    auto email = std::make_shared<EmailObserver>("user@example.com");
    auto sms = std::make_shared<SMSObserver>("13800138000");
    auto log = std::make_shared<LogObserver>("app.log");
    
    subject.attach(email);
    subject.attach(sms);
    subject.attach(log);
    
    std::cout << "\n  发送第一条通知:\n";
    subject.notify("系统启动完成");
    
    // 移除一个观察者
    subject.detach(sms);
    
    std::cout << "\n  发送第二条通知:\n";
    subject.notify("系统运行中");
}

// ============================================
// 4. 观察者生命周期管理
// ============================================

void demo_observer_lifetime() {
    std::cout << "\n【2. 观察者生命周期管理】\n";
    std::cout << "  演示 weak_ptr 自动处理已销毁观察者\n";
    
    Subject subject;
    
    auto longLived = std::make_shared<LogObserver>("persistent.log");
    subject.attach(longLived);
    
    {
        auto shortLived = std::make_shared<EmailObserver>("temp@example.com");
        subject.attach(shortLived);
        
        std::cout << "\n  短生命周期观察者存活时:\n";
        subject.notify("消息1 - 两个观察者都收到");
        
        std::cout << "  当前观察者数量: " << subject.observerCount() << "\n";
    }
    
    // shortLived 已销毁
    std::cout << "\n  短生命周期观察者销毁后:\n";
    std::cout << "  发送消息会自动跳过已销毁观察者:\n";
    subject.notify("消息2 - 只有持久观察者收到");
    std::cout << "  当前观察者数量: " << subject.observerCount() << "\n";
}

// ============================================
// 5. 实际应用：事件系统
// ============================================

class Event {
public:
    std::string type;
    std::string data;
    
    Event(const std::string& t, const std::string& d)
        : type(t), data(d) {}
};

class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void onEvent(const Event& event) = 0;
    virtual std::string id() const = 0;
};

class EventDispatcher {
public:
    using ListenerPtr = std::weak_ptr<EventListener>;
    
    void addEventListener(const std::string& eventType,
                         const std::shared_ptr<EventListener>& listener) {
        listeners_[eventType].push_back(listener);
        std::cout << "  ➕ 注册事件监听器: " << eventType 
                  << " -> " << listener->id() << "\n";
    }
    
    void dispatchEvent(const Event& event) {
        std::cout << "  🚀 派发事件: [" << event.type << "] " << event.data << "\n";
        
        auto it = listeners_.find(event.type);
        if (it == listeners_.end()) {
            std::cout << "    无监听器\n";
            return;
        }
        
        auto& listenerList = it->second;
        for (auto& wp : listenerList) {
            if (auto sp = wp.lock()) {
                sp->onEvent(event);
            }
        }
        
        // 清理过期监听器
        cleanExpired(event.type);
    }

private:
    std::unordered_map<std::string, std::vector<ListenerPtr>> listeners_;
    
    void cleanExpired(const std::string& eventType) {
        auto it = listeners_.find(eventType);
        if (it != listeners_.end()) {
            auto& list = it->second;
            list.erase(
                std::remove_if(list.begin(), list.end(),
                    [](const ListenerPtr& wp) { return wp.expired(); }),
                list.end()
            );
        }
    }
};

class ClickHandler : public EventListener {
public:
    void onEvent(const Event& event) override {
        std::cout << "    👆 ClickHandler 处理: " << event.data << "\n";
    }
    std::string id() const override { return "ClickHandler"; }
};

class KeyboardHandler : public EventListener {
public:
    void onEvent(const Event& event) override {
        std::cout << "    ⌨️  KeyboardHandler 处理: " << event.data << "\n";
    }
    std::string id() const override { return "KeyboardHandler"; }
};

void demo_event_system() {
    std::cout << "\n【3. 实际应用：事件系统】\n";
    
    EventDispatcher dispatcher;
    
    auto clickHandler = std::make_shared<ClickHandler>();
    auto keyboardHandler = std::make_shared<KeyboardHandler>();
    
    dispatcher.addEventListener("click", clickHandler);
    dispatcher.addEventListener("keypress", keyboardHandler);
    
    dispatcher.dispatchEvent(Event("click", "按钮被点击"));
    dispatcher.dispatchEvent(Event("keypress", "按下Enter键"));
    dispatcher.dispatchEvent(Event("unknown", "未知事件"));
}

// ============================================
// 主演示函数
// ============================================
void demo_observer_pattern() {
    demo_observer_basic();
    demo_observer_lifetime();
    demo_event_system();
    
    std::cout << "\n💡 观察者模式中的 weak_ptr 要点:\n";
    std::cout << "   1. Subject 不拥有 Observer，使用 weak_ptr\n";
    std::cout << "   2. 通过 lock() 安全通知存活的观察者\n";
    std::cout << "   3. 自动跳过已销毁的观察者\n";
    std::cout << "   4. 避免通知时访问悬空指针\n";
}
