#ifndef AUTO_EVENT_H_
#define AUTO_EVENT_H_
#include "./include/event.h"
#include <mutex>
#include <condition_variable>
namespace utils {
  class AutoEvent : public Event {
  public:
    AutoEvent();
    virtual ~AutoEvent();
    void Reset() override;
    void Wait() override;
    bool WaitFor(uint64_t seconds, uint64_t milliseconds) override;
    void NotifyOne() override;
    void NotifyAll() override;
  private:
	  std::mutex mutex_;
	  std::condition_variable condition_variable_;
  };
}
#endif // AUTO_EVENT_H_
