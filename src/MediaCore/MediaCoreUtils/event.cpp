#include "include/event.h"
#include "auto_event.h"
#include "manual_event.h"
namespace utils {
	Event::Event() = default;

	Event::~Event() = default;

  EventPtr Event::CreateInstance(bool manual_reset) {
	  if (manual_reset) {
		  return std::make_shared<ManualEvent>();
	  }
	  return  std::make_shared<AutoEvent>();
  }
}
