#include <thread_pool/thread_pool.h>

thread_pool::ThreadPool::ThreadPool(std::size_t n_threads)
{
    for (std::size_t i = 0; i < n_threads; ++i)
    {
        _threads.push_back(std::jthread{
            [&](){
                while (true)
                {
                    std::unique_lock<std::mutex> cv_lock{ _queue_m };
                    _queue_cv.wait(cv_lock, [&]{ return !_unsafe_queue.empty(); });

                    auto const elem = _unsafe_queue.front();
                    _unsafe_queue.pop();
                    //cv_lock.release();
                    
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
    { // stack for pushing to the queue & acquiring lock
        std::lock_guard<std::mutex> cv_lock{ _queue_m };
        _unsafe_queue.push(task);
    }

    _queue_cv.notify_one();
    return true;
}
