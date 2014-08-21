#ifndef CHAT_SERVER_TASK_POOL_H
#define CHAT_SERVER_TASK_POOL_H

#include <functional>
#include <atomic>
#include <thread>
#include <memory>
#include <avxer/BlockingQueue.h>

class TaskPool {
public:
  typedef std::function<void()> task_t;

  TaskPool() {
    stop_execute_ = false;
    task_thread_ = std::thread([this]() {
      while (!stop_execute_) {
        tasks_.Take()();
      }
    });
  }

  ~TaskPool() {
    if (task_thread_.joinable()) {
      tasks_.Put([this]() {
        stop_execute_ = true;
      });
      task_thread_.join();
    }
  }

  void PutTask(const task_t& task) {
    tasks_.Put(task);
  }

  void PutTask(task_t&& task) {
    tasks_.Put(std::move(task));
  }

  TaskPool(const TaskPool&) = delete;
  TaskPool& operator=(const TaskPool&) = delete;

private:
  volatile std::atomic_bool stop_execute_;
  avxer::BlockingQueue<task_t> tasks_;
  std::thread task_thread_;
};

#endif /* CHAT_SERVER_TASK_POOL_H */
