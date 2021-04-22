#include "./include/mirror_handler.h"
#include "mirror_handler_impl.h"
namespace handler {
  MirrorHandlerEvent::MirrorHandlerEvent() = default;

  MirrorHandlerEvent::~MirrorHandlerEvent() = default;

  MirrorHandler::MirrorHandler() = default;

  MirrorHandler::~MirrorHandler() = default;

  MirrorHandlerPtr MirrorHandler::CreateInstance() {
	  return std::make_shared<MirrorHandlerImpl>();
  }
}
