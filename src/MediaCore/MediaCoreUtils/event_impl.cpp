#include "event_impl.h"

namespace utils {
  EventImpl::EventImpl(bool auto_reset_flag) : auto_reset_flag_(auto_reset_flag) {
  }

  EventImpl::~EventImpl() = default;

  void EventImpl::Reset() {
    if (!auto_reset_flag_) {
      flag_ = false;
    }
  }

  void EventImpl::Wait() {
	  std::unique_lock<std::mutex> lock(mutex_);
	  condition_variable_.wait(lock, [this]() {
		  return flag_;
	  });
    if (auto_reset_flag_) {
      flag_ = false;
    }
  }

  bool EventImpl::WaitFor(uint64_t seconds, uint64_t milliseconds) {
    std::chrono::seconds secs(seconds);
    std::chrono::milliseconds msecs(milliseconds);
    auto interval = secs + msecs;
    std::unique_lock<std::mutex> lock(mutex_);
    const auto retvalue = condition_variable_.wait_for(lock, interval, [this]() {
      return flag_;
    });
    if (auto_reset_flag_) {
      flag_ = false;
    }
    return retvalue;
  }

  void EventImpl::NotifyOne() {
	  std::unique_lock<std::mutex> lock(mutex_);
	  flag_ = true;
	  condition_variable_.notify_one();
  }

  void EventImpl::NotifyAll() {
	  std::unique_lock<std::mutex> lock(mutex_);
	  flag_ = true;
	  condition_variable_.notify_all();
  }
}
