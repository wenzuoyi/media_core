#ifndef VIDEO_DECODER_HANDLER_H_
#define VIDEO_DECODER_HANDLER_H_
#include <memory>
#include "global_media_handler.h"
#include "base_video_handler.h"
namespace handler {
	class VideoDecoderHandler;
	using VideoDecoderHandlerPtr = std::shared_ptr<VideoDecoderHandler>;

  class MEDIA_HANDLER_API_HEADER VideoDecoderHandlerEvent : virtual public BaseVideoHandlerEvent {
  public:
	  VideoDecoderHandlerEvent();
	  virtual ~VideoDecoderHandlerEvent();
  };

  class MEDIA_HANDLER_API_HEADER VideoDecoderHandler {
  public:
    VideoDecoderHandler();
    virtual ~VideoDecoderHandler();
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void InputVideoPackage(VideoPackagePtr video_package) = 0;
    virtual void SetEvent(VideoDecoderHandlerEvent* event) = 0;
    static VideoDecoderHandlerPtr CreateInstance();
  };
}
#endif // VIDEO_DECODER_HANDLER_H_
