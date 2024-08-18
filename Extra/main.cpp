#include <iostream>
#include <thread>
#include <future>
#include <chrono>

class MyClass {
public:
    MyClass() {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // ģ���ʼ������
        std::cout << "MyClass initialized." << std::endl;
    }
};

void initialize(std::promise<MyClass*>&& initPromise) {
    MyClass* obj = new MyClass(); // �������󲢳�ʼ��
    initPromise.set_value(obj); // ����ָ��ֵ
}

int main() {
    std::promise<MyClass*> initPromise; // ����promise
    std::future<MyClass*> initFuture = initPromise.get_future(); // ��ȡfuture

    std::thread initThread(initialize, std::move(initPromise)); // ������ʼ���߳�

    MyClass* myObject = nullptr; // ���߳��е�ָ���ʼ��Ϊ��

    // ���̴߳�ӡ��ʼ��״̬
    while (initFuture.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        std::cout << "Initializing..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // ÿ���ӡһ��
    }

    myObject = initFuture.get(); // ��ȡ��ʼ�����ָ��
    initThread.join(); // �ȴ���ʼ���߳����

    // ʹ�ó�ʼ����Ķ���
    if (myObject) {
        std::cout << "Initialization complete. Using myObject." << std::endl;
        // �������ʹ��myObject������������
    }

    delete myObject; // �ͷ��ڴ�
    return 0;
}