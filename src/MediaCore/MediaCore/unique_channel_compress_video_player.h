#ifndef UNIQUE_CHANNEL_COMPRESS_VIDEO_PLAYER_H_
#define UNIQUE_CHANNEL_COMPRESS_VIDEO_PLAYER_H_
#include "unique_channel_base_video_player.h"
#include "video_decoder_handler.h"
namespace core {
  class UniqueChannelCompressVideoPlayer : public UniqueChannelBaseVideoPlayer,
	                                                                        public handler::VideoDecoderHandlerEvent {
  public:
    UniqueChannelCompressVideoPlayer();
    virtual ~UniqueChannelCompressVideoPlayer();
    bool Init(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) override;
    void Fini() override;
	  virtual void InputVideoPackage(VideoPackagePtr video_package);
  protected:
    void OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type, VideoFramePtr video_frame) override;
    void OnHandlerException(handler::VideoHandlerType video_handler_type, const std::string& message, int code) override;
  private:
	  handler::VideoDecoderHandlerPtr video_decoder_handler_{ nullptr };
  };
  using UniqueChannelCompressVideoPlayerPtr = std::shared_ptr<UniqueChannelCompressVideoPlayer>;
}
#endif // UNIQUE_CHANNEL_COMPRESS_VIDEO_PLAYER_H_
