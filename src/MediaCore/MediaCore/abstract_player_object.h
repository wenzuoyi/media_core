#ifndef ABSTRACT_PLAYER_OBJECT_H_
#define ABSTRACT_PLAYER_OBJECT_H_
#include "audio_output_media_source.h"
#include "mosaic_handler.h"
#include "flip_handler.h"
#include "mirror_handler.h"
#include "rotation_handler.h"
#include "video_output_media_source.h"
#include "include/base_player_datatype.h"
namespace core {
  class AbstractPlayerObject : public virtual  handler::MosaicHandlerEvent,
                                                public virtual  handler::FlipHandlerEvent,
	                                              public virtual  handler::MirrorHandlerEvent,
	                                              public virtual  handler::RotationHandlerEvent,
                                                public output::AudioOutputMediaSourceEvent,
                                                public output::VideoOutputMediaSourceEvent {
  public:
    AbstractPlayerObject();
    virtual ~AbstractPlayerObject();
  protected:
    enum class MediaModelType {
      kAll = 0,
      kAudio = 1,
      kVideo = 2
    };
	  virtual bool Initialize(BasicPlayerParamPtr basic_player_info, AudioBaseInfoPtr audio_base_info) final;
	  virtual bool Initialize(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) final;
	  virtual void UnInitialize(MediaModelType type) final;
    void OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type, VideoFramePtr video_frame) override;
    void OnAudioOutputException(unsigned error_code) override;
    void OnAudioTransmitSample(AudioSamplePtr audio_sample) override;
    void OnAudioSampleFrequency(unsigned long frequency) override;
    void OnVideoOutputException(unsigned error_code) override;
    void OnVideoCustomPainting(HDC hdc) override;
    void OnVideoTransmitFrame(VideoFramePtr video_frame) override;
	  handler::MosaicHandlerPtr mosaic_handler_{ nullptr };
	  handler::FlipHandlerPtr flip_handler_{ nullptr };
	  handler::MirrorHandlerPtr mirror_handler_{ nullptr };
	  handler::RotationHandlerPtr rotation_handler_{ nullptr };
	  output::AudioOutputMediaSourcePtr audio_output_{ nullptr };
	  output::VideoOutputMediaSourcePtr video_output_{ nullptr };
  };
}
#endif // ABSTRACT_PLAYER_OBJECT_H_
