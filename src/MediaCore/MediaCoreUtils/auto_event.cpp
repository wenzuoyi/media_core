#include "auto_event.h"

namespace utils {
  AutoEvent::AutoEvent() = default;

  AutoEvent::~AutoEvent() = default;

  void AutoEvent::Reset() {
  }

  void AutoEvent::Wait() {
	  std::unique_lock<std::mutex> lock(mutex_);
	  condition_variable_.wait(lock);
  }

  bool AutoEvent::WaitFor(uint64_t seconds, uint64_t milliseconds) {
	  std::chrono::seconds secs(seconds);
	  std::chrono::milliseconds msecs(milliseconds);
	  auto interval = secs + msecs;
	  std::unique_lock<std::mutex> lock(mutex_);
	  return std::cv_status::timeout != condition_variable_.wait_for(lock, interval);
  }

  void AutoEvent::NotifyOne() {
	  condition_variable_.notify_one();
  }

  void AutoEvent::NotifyAll() {
	  condition_variable_.notify_all();
  }
}
