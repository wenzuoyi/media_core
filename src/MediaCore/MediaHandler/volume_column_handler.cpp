#include "include/volume_column_handler.h"
#include "volume_column_handler_impl.hpp"

namespace handler {
	AudioHandlerPtr VolumeColumnHandler::CreateInstance(unsigned per_sample_bits) {
    AudioHandlerPtr audio_calculator{nullptr};
    if (per_sample_bits == 16) {
      audio_calculator = std::make_shared<VolumeColumnHandlerImpl<int16_t, VolumeColumnHandler>>();
    }
    return audio_calculator;
  }
}
