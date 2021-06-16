#include "./include/rotation_handler.h"
#include "rotation_handler_impl.h"
namespace handler {
	RotationHandlerEvent::RotationHandlerEvent() = default; 

  RotationHandlerEvent::~RotationHandlerEvent() = default;

  RotationHandler::RotationHandler() = default;

  RotationHandler::~RotationHandler() = default;

  RotationHandlerPtr RotationHandler::CreateInstance() {
	  return std::make_shared<RotationHandlerImpl>();
  }
}
