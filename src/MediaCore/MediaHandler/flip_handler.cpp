#include "include/flip_handler.h"
#include "flip_handler_impl.h"
namespace handler {
	FlipHandlerEvent::FlipHandlerEvent() = default;

	FlipHandlerEvent::~FlipHandlerEvent() = default;

	FlipHandler::FlipHandler() = default;

	FlipHandler::~FlipHandler() = default;

  FlipHandlerPtr FlipHandler::CreateInstance() {
	  return std::make_shared<FlipHandlerImpl>();
  }
}
