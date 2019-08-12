#ifndef AUDIO_INPUT_MEDIA_SOURCE_H_
#define AUDIO_INPUT_MEDIA_SOURCE_H_
#include <functional>
#include "audio_input_media_source_datatype.h"
#include "global_input_media_source.h"

namespace input {
  class API_HEADER AudioInputMediaSourceEvent {
  public:
    AudioInputMediaSourceEvent() = default;
    virtual ~AudioInputMediaSourceEvent() = default;
    virtual void OnAudioInputMediaExceptionEvent(int error_code) = 0;
    virtual void OnCaptureAudioSample(AudioSamplePtr audio_sample) = 0;
  };

  class AudioInputMediaSource;
  using AudioInputMediaSourcePtr = std::shared_ptr<AudioInputMediaSource>;

  class API_HEADER AudioInputMediaSource {
  public:
    AudioInputMediaSource() = default;
    virtual ~AudioInputMediaSource() = default;
    static AudioInputMediaSourcePtr CreateInstance();
    virtual void SetAudioInputMediaSourceEvent(AudioInputMediaSourceEvent* sink) = 0;
    virtual void EnumAllAudioDevice(std::function<void(std::string, std::string)> enum_device_callback) = 0;
    virtual void SetAudioInputMediaSourceParam(AudioInputMediaSourceParamPtr param) = 0;
    virtual void StartCapture() = 0;
    virtual void StopCapture() = 0;
  };
}
#endif // AUDIO_INPUT_MEDIA_SOURCE_H_
