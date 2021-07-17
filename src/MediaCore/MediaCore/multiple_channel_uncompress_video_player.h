#ifndef MULTIPLE_CHANNEL_UNCOMPRESS_VIDEO_PLAYER_H_
#define MULTIPLE_CHANNEL_UNCOMPRESS_VIDEO_PLAYER_H_
#include "multiple_channel_base_video_player.h"
#include "unique_channel_uncompress_video_player.h"

namespace core {
  class MultipleChannelUnCompressVideoPlayer : public MultipleChannelBaseVideoPlayer {
  public:
    MultipleChannelUnCompressVideoPlayer();
    virtual ~MultipleChannelUnCompressVideoPlayer();
    bool Init(MultipleChannelVideoPlayerParamPtr param) override;
    void Fini() override;
    virtual void InputVideoFrame(unsigned channel_id, VideoFramePtr video_frame);
  private:
	  mutable std::map<unsigned, UniqueChannelUnCompressVideoPlayerPtr> multiple_channel_uncompress_video_player_map_;
  };
}
#endif // MULTIPLE_CHANNEL_UNCOMPRESS_VIDEO_PLAYER_H_
