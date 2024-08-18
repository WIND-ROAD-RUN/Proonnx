#include <iostream>
#include <thread>
#include <future>
#include <chrono>

class MyClass {
public:
    MyClass() {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟初始化过程
        std::cout << "MyClass initialized." << std::endl;
    }
};

void initialize(std::promise<MyClass*>&& initPromise) {
    MyClass* obj = new MyClass(); // 创建对象并初始化
    initPromise.set_value(obj); // 设置指针值
}

int main() {
    std::promise<MyClass*> initPromise; // 创建promise
    std::future<MyClass*> initFuture = initPromise.get_future(); // 获取future

    std::thread initThread(initialize, std::move(initPromise)); // 启动初始化线程

    MyClass* myObject = nullptr; // 主线程中的指针初始化为空

    // 主线程打印初始化状态
    while (initFuture.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        std::cout << "Initializing..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 每秒打印一次
    }

    myObject = initFuture.get(); // 获取初始化后的指针
    initThread.join(); // 等待初始化线程完成

    // 使用初始化后的对象
    if (myObject) {
        std::cout << "Initialization complete. Using myObject." << std::endl;
        // 这里可以使用myObject进行其他操作
    }

    delete myObject; // 释放内存
    return 0;
}