#ifndef VIDEO_DECODER_HANDLER_IMPL_H_
#define VIDEO_DECODER_HANDLER_IMPL_H_
#include "./include/video_decoder_handler.h"
#include "video_decoder.h"
#include "async_queue.hpp"

namespace handler {
  class VideoDecoderHandlerImpl : public VideoDecoderHandler , public utils::AsyncQueue<VideoPackagePtr> {
  public:
    VideoDecoderHandlerImpl();
    virtual ~VideoDecoderHandlerImpl();
  protected:
    void Start() override;
    void Stop() override;
    void InputVideoPackage(VideoPackagePtr video_package) override;
    void SetEvent(VideoDecoderHandlerEvent* event) override;
    void AsyncRun(VideoPackagePtr frame) override;
  private:
	  VideoDecoderHandlerEvent* event_{ nullptr };
	  VideoDecoderPtr video_decoder_{ nullptr };
	  VideoPackageType current_decoder_type_{ VideoPackageType::kNone };
  };  
}
#endif // VIDEO_DECODER_HANDLER_IMPL_H_
