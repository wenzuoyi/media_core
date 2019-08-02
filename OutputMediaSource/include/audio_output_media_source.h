#ifndef AUDIO_OUTPUT_MEDIA_SOURCE_H_
#define AUDIO_OUTPUT_MEDIA_SOURCE_H_
#include "global_output_media_source.h"
#include "audio_output_media_source_datatype.h"
namespace output {
	class AudioOutputMediaSource;
	using AudioOutputMediaSourcePtr = std::shared_ptr<AudioOutputMediaSource>;

  class API_HEADER AudioOutputMediaSourceEvent {
  public:
	  AudioOutputMediaSourceEvent() = default;
	  virtual ~AudioOutputMediaSourceEvent() = default;
	  virtual void OnAudioOutputMediaExceptionEvent(unsigned error_code) = 0;
	  virtual void OnTransmitDataEvent(AudioSamplePtr audio_sample) = 0;
  };

	class API_HEADER AudioOutputMediaSource {
	public:
		AudioOutputMediaSource() = default;
		virtual ~AudioOutputMediaSource() = default;
		static AudioOutputMediaSourcePtr CreateInstance();
		virtual void SetAudioOutputMediaSourceEvent(AudioOutputMediaSourceEvent* sink) = 0;
		virtual void SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) = 0;
		virtual void Mute(bool enable) = 0;
		virtual bool IsMute() const = 0;
		virtual void SetVolume(int volume) = 0;
		virtual int GetVolume() const = 0;
		virtual void InputAudioSample(AudioSamplePtr audio_sample) = 0;
	};
}
#endif // AUDIO_OUTPUT_MEDIA_SOURCE_H_
