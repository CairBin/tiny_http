#include "tiny_http/thread_pool.h"
#include <stdexcept>

ThreadPool::ThreadPool(size_t thread_count) : stop_(false) {
  if (thread_count == 0) {
    throw std::invalid_argument("Thread count must be greater than 0.");
  }

  for (size_t i = 0; i < thread_count; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerThread, this);
  }
}

ThreadPool::~ThreadPool() {
  stop_ = true;
  condition_.notify_all();  // 通知所有线程

  for (std::thread& worker : workers_) {
    if (worker.joinable()) {
      worker.join();  // 等待线程完成
    }
  }
}

void ThreadPool::WorkerThread() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(tasks_mutex_);
      condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

      if (stop_ && tasks_.empty()) {
        return;  // 停止线程
      }

      task = std::move(tasks_.front());
      tasks_.pop();
    }

    task();  // 执行任务
  }
}
