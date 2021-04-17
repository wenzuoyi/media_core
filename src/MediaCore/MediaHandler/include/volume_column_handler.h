#ifndef VOLUME_COLUMN_HANDLER_H_
#define VOLUME_COLUMN_HANDLER_H_
#include "global_media_handler.h"
#include "base_audio_handler.h"
namespace handler {
	class VolumeColumnHandler;
	using VolumeColumnHandlerPtr = std::shared_ptr<VolumeColumnHandler>;
  class MEDIA_HANDLER_API_HEADER VolumeColumnHandlerEvent : public BaseAudioHandlerEvent {
  public:
	  VolumeColumnHandlerEvent() = default;
	  virtual ~VolumeColumnHandlerEvent() = default;
	  virtual void OnSampleVolumeRatio(std::vector<double>&& ratio_array) = 0;
  };
  class MEDIA_HANDLER_API_HEADER VolumeColumnHandler : public BaseAudioHandler {
  public:
    VolumeColumnHandler() = default;
    virtual ~VolumeColumnHandler() = default;
	  static VolumeColumnHandlerPtr CreateInstance(unsigned per_sample_bits);
	  virtual void SetAudioSampleChannel(int channel) = 0;
  };
}

#endif // VOLUME_COLUMN_HANDLER_H_
