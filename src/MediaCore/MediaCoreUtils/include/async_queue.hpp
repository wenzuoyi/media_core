#ifndef ASYNC_QUEUE_HPP_
#define ASYNC_QUEUE_HPP_
#include <thread>
#include <deque>
#include "shared_mutex.h"
#include "async_workflow.h"
namespace utils {
  template <class T>
  class AsyncQueue : public AsyncWorkflow {
  protected:
	  AsyncQueue() {
      shared_mutex_ = utils::SharedMutex::CreateInstance();
    }

    virtual ~AsyncQueue() {
      shared_mutex_ = nullptr;
    }

    virtual void Push(T frame) {
      utils::WriteLock lock(shared_mutex_);
      queue_.push_back(frame);
    }

    void AsyncExecute() override {
      auto have_frame{false};
      {
        utils::ReadLock lock(shared_mutex_);
        have_frame = queue_.empty();
      }
      if (!have_frame) {
        T frame{nullptr};
        {
          utils::WriteLock lock(shared_mutex_);
          frame = queue_.front();
          queue_.pop_front();
        }
        AsyncRun(frame);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
    }

	  virtual void AsyncRun(T frame) = 0;
  private:    
	  std::deque<T> queue_;
	  utils::SharedMutexPtr shared_mutex_;
  };
}
#endif // ASYNC_QUEUE_HPP_
