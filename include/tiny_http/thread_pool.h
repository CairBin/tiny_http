#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count);
    ~ThreadPool();

    // 提交任务到线程池，返回一个 std::future 对象
    template <typename Func, typename... Args>
    auto Submit(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>{

        using ReturnType = typename std::result_of<Func(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        std::future<ReturnType> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(tasks_mutex_);

            if (stop_) {
                throw std::runtime_error("ThreadPool is stopped, cannot submit new tasks.");
            }

            tasks_.emplace([task]() { (*task)(); });
        }
        condition_.notify_one();  // 通知一个线程处理任务
        return result;
    }

private:
  void WorkerThread();  // 工作线程函数

  std::vector<std::thread> workers_;             // 工作线程集合
  std::queue<std::function<void()>> tasks_;      // 任务队列
  std::mutex tasks_mutex_;                       // 保护任务队列的互斥锁
  std::condition_variable condition_;            // 条件变量，用于通知线程
  std::atomic<bool> stop_;                       // 标记线程池是否停止
};

#endif  // THREAD_POOL_H_