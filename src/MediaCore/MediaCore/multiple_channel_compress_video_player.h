
#ifndef MULTIPLE_CHANNEL_COMPRESS_VIDEO_PLAYER_H_
#define MULTIPLE_CHANNEL_COMPRESS_VIDEO_PLAYER_H_
#include "multiple_channel_base_video_player.h"
#include "unique_channel_compress_video_player.h"
namespace core {
  class MultipleChannelCompressVideoPlayer : public MultipleChannelBaseVideoPlayer {
  public:
    MultipleChannelCompressVideoPlayer();
    virtual ~MultipleChannelCompressVideoPlayer();
    bool Init(MultipleChannelVideoPlayerParamPtr param) override;
    void Fini() override;
	  virtual void InputVideoPackage(unsigned channel_id, VideoPackagePtr video_package);
  private:
	  mutable std::map<unsigned, UniqueChannelCompressVideoPlayerPtr> multiple_channel_compress_video_player_map_;
  };
}
#endif // MULTIPLE_CHANNEL_COMPRESS_VIDEO_PLAYER_H_
