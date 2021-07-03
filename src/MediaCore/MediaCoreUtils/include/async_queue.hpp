#ifndef ASYNC_QUEUE_HPP_
#define ASYNC_QUEUE_HPP_
#include <thread>
#include <deque>
#include "Poco/RWLock.h"
#include "async_workflow.h"
namespace utils {
  template <class T>
  class AsyncQueue : public AsyncWorkflow {
  protected:
	  AsyncQueue() = default;

	  virtual ~AsyncQueue() = default;

    virtual void Push(T frame) {
      Poco::ScopedWriteRWLock lock(read_write_lock_);
      queue_.push_back(frame);
    }

    void AsyncExecute() override {
      auto have_frame{false};
      {
        Poco::ScopedReadRWLock lock(read_write_lock_);
        have_frame = queue_.empty();
      }
      if (!have_frame) {
        T frame{nullptr};
        {
          Poco::ScopedWriteRWLock lock(read_write_lock_);
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
	  Poco::RWLock read_write_lock_;
  };
}
#endif // ASYNC_QUEUE_HPP_
