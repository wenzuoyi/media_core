#ifndef AUDIO_HANDLER_H_
#define AUDIO_HANDLER_H_
#include "global_media_handler.h"
#include "audio_handler_datatype.h"
namespace handler {
	class AudioHandler;
	using AudioHandlerPtr = std::shared_ptr<AudioHandler>;
  enum class AudioHandlerType {
    kS16AudioColumnHandler = 16
  };
  class API_HEADER AudioHandlerEvent {
  public:
	  AudioHandlerEvent() = default;
	  virtual ~AudioHandlerEvent() = default;
	  virtual void OnTransmitAudioSample(AudioHandlerType audio_event, AudioSamplePtr audio_sample) = 0;
	  virtual void OnSampleVolumeRatio(AudioHandlerType audio_event, std::vector<double>&& ratio_array) = 0;
  };
	class API_HEADER AudioHandler {
	public:
		AudioHandler() = default;
		virtual ~AudioHandler() = default;
		static AudioHandlerPtr CreateInstance(AudioHandlerType audio_calculator_type);
		virtual void SetAudioSampleParam(int channel) = 0;
		virtual void SetAudioHandlerEvent(AudioHandlerEvent* sink) = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual bool InputAudioSample(AudioSamplePtr audio_sample) = 0;
	};
}
#endif // AUDIO_HANDLER_H_
