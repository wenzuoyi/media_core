#include "include/volume_column_handler.h"
#include "volume_column_handler_impl.hpp"

namespace handler {
	VolumeColumnHandlerPtr VolumeColumnHandler::CreateInstance(unsigned per_sample_bits) {
		VolumeColumnHandlerPtr audio_calculator{nullptr};
    if (per_sample_bits == 16) {
      audio_calculator = std::make_shared<VolumeColumnHandlerImpl<int16_t>>();
    }
    return audio_calculator;
  }
}
