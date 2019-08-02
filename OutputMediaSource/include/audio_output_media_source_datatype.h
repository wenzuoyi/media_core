#ifndef AUDIO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
#define AUDIO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
#include <memory>
#include <vector>
namespace output {
	using AudioSample = std::vector<unsigned char>;
	using AudioSamplePtr = std::shared_ptr<AudioSample>;

  struct AudioOutputParam {
    unsigned sample_rate{8000};
    unsigned bits_per_sample{16};
    unsigned channels{2};
  };
  using AudioOutputParamPtr = std::shared_ptr<AudioOutputParam>;
}
#endif // AUDIO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
