#include "multiple_channel_uncompress_video_player.h"

namespace core {
	MultipleChannelUnCompressVideoPlayer::MultipleChannelUnCompressVideoPlayer() = default;

	MultipleChannelUnCompressVideoPlayer::~MultipleChannelUnCompressVideoPlayer() = default;

  bool MultipleChannelUnCompressVideoPlayer::Init(MultipleChannelVideoPlayerParamPtr param) {
    if (param == nullptr) {
      return false;
    }
    auto success_count{0U};
    for (auto iter = param->begin(); iter != param->end(); ++iter) {
      auto chanel_id = iter->first;
      auto sub_param = iter->second;
      auto basic_player_param = sub_param->first;
      auto video_base_info = sub_param->second;
      auto object = std::make_shared<UniqueChannelUnCompressVideoPlayer>();
      auto filter = std::make_shared<BaseVideoFilter>();
      filter->channel_id = chanel_id;
      filter->user_data = nullptr;
      object->SetEvent(this, filter);
      if (object->Init(basic_player_param, video_base_info)) {
        multiple_channel_base_video_player_map_.insert(std::make_pair(chanel_id, object));
        multiple_channel_uncompress_video_player_map_.insert(std::make_pair(chanel_id, object));
        ++success_count;
      }
    }
    return success_count == param->size();
  }

  void MultipleChannelUnCompressVideoPlayer::Fini() {
	  MultipleChannelBaseVideoPlayer::Fini();
	  multiple_channel_uncompress_video_player_map_.clear();
  }

  void MultipleChannelUnCompressVideoPlayer::InputVideoFrame(unsigned channel_id, VideoFramePtr video_frame) {
    auto iter = multiple_channel_uncompress_video_player_map_.find(channel_id);
    if (iter == multiple_channel_uncompress_video_player_map_.end()) {
      return;
    }
    auto object = iter->second;
    if (object != nullptr) {
      object->InputVideoFrame(video_frame);
    }
  }
}

