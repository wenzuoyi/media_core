#include "manual_event.h"
#include <thread>
using namespace std::chrono_literals;
namespace utils {
  ManualEvent::ManualEvent() = default;

  ManualEvent::~ManualEvent() = default;

  void ManualEvent::Wait() {
    while (flag_) {
      std::this_thread::sleep_for(5ms);
    }
  }

  void ManualEvent::Reset() {
	  flag_ = true;
  }

  bool ManualEvent::WaitFor(uint64_t seconds, uint64_t milliseconds) {
	  std::chrono::seconds secs(seconds);
	  std::chrono::milliseconds msecs(milliseconds);
	  auto interval = secs + msecs;
    while(flag_ && interval > std::chrono::milliseconds(5)) {
		  std::this_thread::sleep_for(5ms);
		  interval -= std::chrono::milliseconds(5);
    }
	  return flag_;
  }

  void ManualEvent::NotifyOne() {
	  flag_ = false;
  }

  void ManualEvent::NotifyAll() {
	  NotifyOne();
  }
}
