#ifndef AUDIO_HANDLER_H_
#define AUDIO_HANDLER_H_
#include "global_media_handler.h"
#include "audio_handler_datatype.h"
namespace handler {
  class MEDIA_HANDLER_API_HEADER BaseAudioHandlerEvent {
  public:
	  BaseAudioHandlerEvent() = default;
	  virtual ~BaseAudioHandlerEvent() = default;
	  virtual void OnTransmitAudioSample(AudioHandlerType audio_event, AudioSamplePtr audio_sample) = 0;
  };
	class MEDIA_HANDLER_API_HEADER BaseAudioHandler {
	public:
		BaseAudioHandler() = default;
		virtual ~BaseAudioHandler() = default;
		virtual void SetEvent(void* event) = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void InputAudioSample(AudioSamplePtr audio_sample) = 0;
	};
}
#endif // AUDIO_HANDLER_H_
