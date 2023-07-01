#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread_safe_queue/thread_safe_queue.h>

#include <functional>
#include <string>
#include <variant>
#include <vector>
#include <thread>

#include <condition_variable>
#include <queue>
#include <mutex>


// TODOs :
//          - Need to return some sort of future from the pool
//          - Need to double check input param types and returns
//          - Need to use a thread safe queue

namespace thread_pool
{
    // The allowed parameter types that can go into a
    // task's function
    using Param = std::variant<int, float, std::string>;

    // Indicates type of task in the queue
    enum class TaskType {
        Execute,
        Stop
    };

    struct Task {
        TaskType type;
        std::function<void(std::vector<Param>)> task;
        std::vector<Param> arguments;
    };

    class ThreadPool
    {
    public:
        ThreadPool(std::size_t n_threads);
        ~ThreadPool();
        bool push(Task const& task);
    
    private:
        TsQueue<Task> _queue;
        std::vector<std::jthread> _threads;
    };
} // namespace thread_pool

#endif // THREAD_POOL_H
