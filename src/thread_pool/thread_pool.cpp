#include <thread_pool/thread_pool.h>

thread_pool::ThreadPool::ThreadPool(std::size_t n_threads)
{
    for (std::size_t i = 0; i < n_threads; ++i)
    {
        _threads.push_back(std::jthread{
            [&](){
                while (true)
                {
                    auto const elem = _queue.pop();
                    
                    // At this stage we don't really change the
                    // queue anymore so we can definitely let the
                    // mutex go
                    switch (elem.type) {
                    case TaskType::Execute:
                        // TODO : Some sort of input checking
                        elem.task(elem.arguments);
                        break;
                    case TaskType::Stop:
                        return;
                    }
                }
            }
        });
    }
}

thread_pool::ThreadPool::~ThreadPool()
{
    Task const stop_task{TaskType::Stop, {}, {}};
    for (std::size_t i = 0; i < _threads.size(); ++i)
    {
        push(stop_task);
    }
}

bool thread_pool::ThreadPool::push(Task const& task)
{
    _queue.push(task);
    return true;
}
