#ifndef ASYNC_RUNNABLE_H_
#define ASYNC_RUNNABLE_H_
#include <atomic>
#include <thread>
#include <deque>
#include <Poco/ThreadPool.h>
#include <Poco/Runnable.h>
#include "shared_mutex.h"
namespace handler {

  template <class T>
  class AsyncRunnable : public Poco::Runnable {
  protected:
	  AsyncRunnable() {
      shared_mutex_ = utils::SharedMutex::CreateInstance();
    }

    virtual ~AsyncRunnable() {
      shared_mutex_ = nullptr;
    }

    virtual void AsyncStart() {
      reference_count_ = 0;
      exit_ = false;
	    RequestAsyncTask();
    }

    virtual void AsyncStop() {
      exit_ = true;
      while (reference_count_ > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
      }
    }

    virtual void Push(T frame) {
      utils::WriteLock lock(shared_mutex_);
      queue_.push_back(frame);
    }

	  virtual void AsyncRun(T frame) = 0;

  private:
    void run() override final {
      if (exit_) {
        --reference_count_;
        return;
      }
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
      RequestAsyncTask();
      --reference_count_;
    }

    void RequestAsyncTask() {
      ++reference_count_;
      Poco::ThreadPool::defaultPool().start(*this);
    }

    std::string name_;
	  std::deque<T> queue_;
	  utils::SharedMutexPtr shared_mutex_;
	  std::atomic_int reference_count_{ 0 };
	  std::atomic_bool exit_{ false };    
  };
}
#endif // ASYNC_RUNNABLE_H_
