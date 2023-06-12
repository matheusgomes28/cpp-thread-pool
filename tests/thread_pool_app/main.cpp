#include <thread_pool/thread_pool.h>

#include <iostream>
#include <mutex>
#include <vector>

using thread_pool::Param;
using thread_pool::Task;
using thread_pool::TaskType;
using thread_pool::ThreadPool;

int main(int argc, char** argv)
{
    ThreadPool my_pool{4};

    std::mutex iostream_m;
    Task const my_task1{
        TaskType::Execute,
        [&](std::vector<Param> const&){
            {
                std::lock_guard cout_guard{iostream_m};
                std::cout << "Hi from thread 1\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        },
        {}
    };
    Task const my_task2{
        TaskType::Execute,
        [&](std::vector<Param> const&){
            {
                std::lock_guard cout_guard{iostream_m};
                std::cout << "Hi from thread 2\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        },
        {}
    };
    Task const my_task3{
        TaskType::Execute,
        [&](std::vector<Param> const&){
            {
                std::lock_guard cout_guard{iostream_m};
                std::cout << "Hi from thread 3\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        },
        {}
    };
    Task const my_task4{
        TaskType::Execute,
        [&](std::vector<Param> const&){
            {
                std::lock_guard cout_guard{iostream_m};
                std::cout << "Hi from thread 4\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        },
        {}
    };

    for (int i = 0; i < 10; ++i) { my_pool.push(my_task1); }
    for (int i = 0; i < 10; ++i) { my_pool.push(my_task2); }
    for (int i = 0; i < 10; ++i) { my_pool.push(my_task3); }
    for (int i = 0; i < 10; ++i) { my_pool.push(my_task4); }
}
