#ifndef MANUAL_EVENT_H_
#define MANUAL_EVENT_H_
#include "./include/event.h"
#include <atomic>
namespace utils {
  class ManualEvent : public Event {
  public:
    ManualEvent();
    virtual ~ManualEvent();
  private:
    void Wait() override;
    void Reset() override;
    bool WaitFor(uint64_t seconds, uint64_t milliseconds) override;
    void NotifyOne() override;
    void NotifyAll() override;
  private:
	  std::atomic_bool flag_{true};
  };
}
#endif // MANUAL_EVENT_H_
