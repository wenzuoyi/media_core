#ifndef AUDIO_OUTPUT_MEDIA_SOURCE_H_
#define AUDIO_OUTPUT_MEDIA_SOURCE_H_
#include "global_output_media_source.h"
#include "audio_output_media_source_datatype.h"
namespace output {
	class AudioOutputMediaSource;
	using AudioOutputMediaSourcePtr = std::shared_ptr<AudioOutputMediaSource>;

  class OUTPUT_MEDIA_API_HEADER AudioOutputMediaSourceEvent {
  public:
	  AudioOutputMediaSourceEvent() = default;
	  virtual ~AudioOutputMediaSourceEvent() = default;
	  virtual void OnAudioOutputMediaExceptionEvent(unsigned error_code) = 0;
	  virtual void OnTransmitDataEvent(AudioSamplePtr audio_sample) = 0;
	  virtual void OnSampleFrequency(unsigned long frequency) = 0;
  };

	class OUTPUT_MEDIA_API_HEADER AudioOutputMediaSource {
	public:
		AudioOutputMediaSource() = default;
		virtual ~AudioOutputMediaSource() = default;
		static AudioOutputMediaSourcePtr CreateInstance(PlayingMode playing_mode);
		virtual bool Init() = 0;
		virtual void Fini() = 0;
		virtual void SetAudioOutputMediaSourceEvent(AudioOutputMediaSourceEvent* sink) = 0;
		virtual bool SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) = 0;
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual void Mute() = 0;
		virtual bool IsMute() const = 0;
		virtual bool SetVolume(int volume) = 0;
		virtual int GetVolume() const = 0;
		virtual bool InputAudioSample(AudioSamplePtr audio_sample) = 0;
	};
}
#endif // AUDIO_OUTPUT_MEDIA_SOURCE_H_
