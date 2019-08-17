#include "include/audio_handler.h"
#include "async_audio_handler.h"
#include "volume_column_handler.h"

namespace handler {
	AudioHandlerPtr AudioHandler::CreateInstance(AudioHandlerType audio_calculator_type) {
		AudioHandlerPtr audio_calculator{ nullptr };
    switch (audio_calculator_type) {
      case AudioHandlerType::kS16AudioColumnHandler: {
        audio_calculator = std::make_shared<VolumeColumnHandler<int16_t>>();
        break;
      }
      default: {
      }
    }
	  return audio_calculator;
  }
}
