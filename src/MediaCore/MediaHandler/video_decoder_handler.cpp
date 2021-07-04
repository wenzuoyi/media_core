#include "./include/video_decoder_handler.h"

namespace handler {
	VideoDecoderHandlerEvent::VideoDecoderHandlerEvent() = default;

	VideoDecoderHandlerEvent::~VideoDecoderHandlerEvent() = default;

	VideoDecoderHandler::VideoDecoderHandler() = default;

  VideoDecoderHandler::~VideoDecoderHandler() = default;

  VideoDecoderHandlerPtr VideoDecoderHandler::CreateInstance() {
	  return nullptr;
  }
}
