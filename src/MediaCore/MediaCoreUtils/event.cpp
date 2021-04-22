#include "include/event.h"
#include "event_impl.h"
namespace utils {
	Event::Event() = default;

	Event::~Event() = default;

  EventPtr Event::CreateInstance(bool manual_reset) {	  
	  return  std::make_shared<EventImpl>(!manual_reset);
  }
}
