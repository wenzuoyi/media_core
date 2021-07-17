#ifndef UNIQUE_CHANNEL_UNCOMPRESS_VIDEO_PLAYER_H_
#define UNIQUE_CHANNEL_UNCOMPRESS_VIDEO_PLAYER_H_
#include "unique_channel_base_video_player.h"
namespace core {
  class UniqueChannelUnCompressVideoPlayer : public UniqueChannelBaseVideoPlayer {
  public:
    UniqueChannelUnCompressVideoPlayer();
    virtual ~UniqueChannelUnCompressVideoPlayer();
    bool Init(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) override;
    void Fini() override;
    void InputVideoFrame(VideoFramePtr video_frame);
  protected:
    void OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type, VideoFramePtr video_frame) override;
  };
  using UniqueChannelUnCompressVideoPlayerPtr = std::shared_ptr<UniqueChannelUnCompressVideoPlayer>;
}
#endif // UNIQUE_CHANNEL_UNCOMPRESS_VIDEO_PLAYER_H_
