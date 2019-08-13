#ifndef AUDIO_CALCULATOR_DATATYPE_H_
#define AUDIO_CALCULATOR_DATATYPE_H_
#include <vector>
#include <memory>
namespace handler {
	using AudioSample = std::vector<unsigned char>;
	using AudioSamplePtr = std::shared_ptr<AudioSample>;
}
#endif // AUDIO_CALCULATOR_DATATYPE_H_
