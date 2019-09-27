#ifndef AUDIO_INPUT_MEDIA_SOURCE_DATATYPE_H_
#define AUDIO_INPUT_MEDIA_SOURCE_DATATYPE_H_
#include <vector>
#include <string>
#include <memory>

namespace input {
	using AudioSample = std::vector<unsigned char>;
	using AudioSamplePtr = std::shared_ptr<AudioSample>;

  struct AudioInputMediaSourceParam {
    std::string audio_device_uuid;
    unsigned sample_rate{8000};
    unsigned channel{1};
    unsigned bits_per_sample{16};
  };
  using AudioInputMediaSourceParamPtr = std::shared_ptr<AudioInputMediaSourceParam>;

}
#endif // AUDIO_INPUT_MEDIA_SOURCE_DATATYPE_H_
