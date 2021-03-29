#include "./include/mosaic_handler.h"
#include "mosaic_handler_impl.h"
namespace handler {
	MosaicHandlerEvent::MosaicHandlerEvent() {
	}

  MosaicHandlerEvent::~MosaicHandlerEvent() {
  }

  MosaicHandler::MosaicHandler() = default;

	MosaicHandler::~MosaicHandler() = default;

  MosaicHandlerPtr MosaicHandler::CreateInstance() {
	  return std::make_shared<MosaicHandlerImpl>();
  }
}
