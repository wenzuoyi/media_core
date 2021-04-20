#ifndef AUDIO_HANDLER_DATATYPE_H_
#define AUDIO_HANDLER_DATATYPE_H_
#include <vector>
#include <memory>
namespace handler {
	enum class AudioHandlerType {
		kAudioColumn = 0
	};
	using AudioSample = std::vector<unsigned char>;
	using AudioSamplePtr = std::shared_ptr<AudioSample>;
}
#endif // AUDIO_HANDLER_DATATYPE_H_
