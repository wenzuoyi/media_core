#ifndef EVENT_IMPL_H_
#define EVENT_IMPL_H_
#include "./include/event.h"
#include <mutex>
#include <condition_variable>
namespace utils {
  class EventImpl : public Event {
  public:
    explicit EventImpl(bool auto_reset_flag);
    virtual ~EventImpl();
    void Reset() override;
    void Wait() override;
    bool WaitFor(uint64_t seconds, uint64_t milliseconds) override;
    void NotifyOne() override;
    void NotifyAll() override;
  private:
	  std::mutex mutex_;
	  std::condition_variable condition_variable_;
	  bool flag_{ false };
	  bool auto_reset_flag_{ true };
  };
}
#endif // EVENT_IMPL_H_
