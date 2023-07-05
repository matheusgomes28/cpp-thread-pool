#include <thread_pool/thread_pool.h>

#include <iostream>
#include <mutex>
#include <vector>

using thread_pool::Param;
using thread_pool::Task;
using thread_pool::TaskType;
using thread_pool::ThreadPool;

Task make_mock_task(std::mutex& m, int i)
{
    Task const my_task1{
        TaskType::Execute,
        [&m, i](std::vector<Param> const&){
            {
                std::lock_guard cout_guard{m};
                std::cout << "Hi from thread " << std::this_thread::get_id()
                          << " request " << i << '\n';
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        },
        {}
    };

    return my_task1;
}

int main(int argc, char** argv)
{
    ThreadPool my_pool{4};
    std::mutex iostream_m;

    int job_n = 0;
    for (int i = 0; i < 100; ++i)
    {
        my_pool.push({
            TaskType::Execute, // TaskType
            [&iostream_m, job_n](std::vector<Param> const&){ // Lambda
                {
                    std::lock_guard cout_guard{iostream_m};
                    std::cout << "Hi from thread " << std::this_thread::get_id()
                            << " request " << job_n << '\n';
                }
                std::this_thread::sleep_for(std::chrono::milliseconds{50});
            },
            {} // Arguments
        });
        job_n++;
    }
}
