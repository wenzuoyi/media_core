#include "unique_channel_uncompress_video_player.h"

namespace core {
	UniqueChannelUnCompressVideoPlayer::UniqueChannelUnCompressVideoPlayer() = default;

	UniqueChannelUnCompressVideoPlayer::~UniqueChannelUnCompressVideoPlayer() = default;

  bool UniqueChannelUnCompressVideoPlayer::Init(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) {
	  if (basic_player_info == nullptr || video_base_info == nullptr) {
		  return false;
	  }
	  return UniqueChannelBaseVideoPlayer::Init(basic_player_info, video_base_info);
  }

  void UniqueChannelUnCompressVideoPlayer::Fini() {
	  UniqueChannelBaseVideoPlayer::Fini();
  }

  void UniqueChannelUnCompressVideoPlayer::OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type, VideoFramePtr video_frame) {
	  if (video_frame == nullptr) {
		  return;
	  }
	  UniqueChannelBaseVideoPlayer::OnTransmitVideoFrame(video_handler_type, video_frame);
  }

  void UniqueChannelUnCompressVideoPlayer::InputVideoFrame(VideoFramePtr video_frame) {
    if (mosaic_handler_ != nullptr) {
      mosaic_handler_->InputVideoFrame(video_frame);
    }
  }
}

