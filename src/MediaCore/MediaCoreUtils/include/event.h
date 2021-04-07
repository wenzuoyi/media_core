#ifndef EVENT_H_
#define EVENT_H_
#include "global_media_utils_source.h"
#include <memory>
namespace utils {
	class Event;
	using EventPtr = std::shared_ptr<Event>;
  class MEDIA_CORE_UTILS_API_HEADER Event {
  public:
    Event();
    virtual ~Event();
	  static EventPtr CreateInstance(bool manual_reset);
	  virtual void Reset() = 0;
	  virtual void Wait() = 0;
	  virtual bool WaitFor(uint64_t seconds, uint64_t milliseconds) = 0;
	  virtual void NotifyOne() = 0;
	  virtual void NotifyAll() = 0;
  };

}
#endif // EVENT_H_
