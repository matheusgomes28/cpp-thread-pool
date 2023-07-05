#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H
#include <condition_variable>
#include <mutex>
#include <queue>

namespace thread_pool
{
    template <typename T>
    class TsQueue
    {
    public:
        void push(T const& val)
        {
            std::lock_guard<std::mutex> queue_lock{_queue_mutex};
            _queue.push(val);
            _queue_cv.notify_one();
        }

        T pop()
        {
            std::unique_lock<std::mutex> queue_lock{_queue_mutex};
            _queue_cv.wait(queue_lock, [&]{ return !_queue.empty(); });

            T ret = _queue.front();
            _queue.pop();
            return ret;
        }

    private:
        std::queue<T> _queue;
        std::condition_variable _queue_cv;
        std::mutex _queue_mutex;
    };
} // namespace thread_pool
#endif // THREAD_SAFE_QUEUE_H