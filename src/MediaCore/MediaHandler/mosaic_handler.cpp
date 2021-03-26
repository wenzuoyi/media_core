#include "./include/mosaic_handler.h"
namespace handler {
	MosaicHandlerEvent::MosaicHandlerEvent() = default;

  MosaicHandlerEvent::~MosaicHandlerEvent() = default;

  MosaicHandler::MosaicHandler() = default;

	MosaicHandler::~MosaicHandler() = default;

  MosaicHandlerPtr MosaicHandler::CreateInstance() {
	  return nullptr;
  }
}
