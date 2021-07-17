#include "multiple_channel_base_video_player.h"

namespace core {
	MultipleChannelBaseVideoPlayer::MultipleChannelBaseVideoPlayer() = default;

	MultipleChannelBaseVideoPlayer::~MultipleChannelBaseVideoPlayer() = default;

	bool MultipleChannelBaseVideoPlayer::Execute(unsigned channel_id, std::function<bool(UniqueChannelBaseVideoPlayerPtr)>&& method) const {
		auto result{ false };
		auto iter = multiple_channel_base_video_player_map_.find(channel_id);
		if (iter != multiple_channel_base_video_player_map_.end()) {
			auto object = iter->second;
      if (object != nullptr) {
			  result = method(object);
      }
		}
		return result;
	}

  void MultipleChannelBaseVideoPlayer::ExecuteEx(unsigned channel_id, std::function<void(UniqueChannelBaseVideoPlayerPtr)>&& method) const {
    auto iter = multiple_channel_base_video_player_map_.find(channel_id);
    if (iter == multiple_channel_base_video_player_map_.end()) {
      return;
    }
    auto object = iter->second;
    if (object != nullptr) {
      method(object);
    }
  }

  void MultipleChannelBaseVideoPlayer::SetEvent(MultipleChannelBaseVideoPlayerEvent* event) {
	  event_ = event;
  }

  void MultipleChannelBaseVideoPlayer::Fini() {
	  for (auto iter = multiple_channel_base_video_player_map_.begin(); iter != multiple_channel_base_video_player_map_.end(); ++iter) {
		  auto object = iter->second;
		  object->Fini();
	  }
	  multiple_channel_base_video_player_map_.clear();
  }

  bool MultipleChannelBaseVideoPlayer::Snapshot(unsigned channel_id, const std::string& url) {
    return Execute(channel_id, [url](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      return unique_channel_base_video_player->Snapshot(url);
    });
  }

  bool MultipleChannelBaseVideoPlayer::IsZoom(unsigned channel_id) const {
	  return Execute(channel_id, [](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      return unique_channel_base_video_player->IsZoom();
    });
  }

  void MultipleChannelBaseVideoPlayer::Zoom(unsigned channel_id, RegionPtr region) {
    ExecuteEx(channel_id, [region](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      unique_channel_base_video_player->Zoom(region);
    });
  }

  bool MultipleChannelBaseVideoPlayer::IsSettingMosaic(unsigned channel_id) const {
    return Execute(channel_id, [](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      return unique_channel_base_video_player->IsSettingMosaic();
    });
  }

  void MultipleChannelBaseVideoPlayer::Mosaic(unsigned channel_id, RegionPtr region) {
    ExecuteEx(channel_id, [region](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      unique_channel_base_video_player->Mosaic(region);
    });
  }

  bool MultipleChannelBaseVideoPlayer::IsMirror(unsigned channel_id) const {
    return Execute(channel_id, [](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      return unique_channel_base_video_player->IsMirror();
    });
  }

  void MultipleChannelBaseVideoPlayer::Mirror(unsigned channel_id, bool enable) {
    ExecuteEx(channel_id, [enable](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      unique_channel_base_video_player->Mirror(enable);
    });
  }

  bool MultipleChannelBaseVideoPlayer::IsEnableRotation(unsigned channel_id) const {
    return Execute(channel_id, [](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      return unique_channel_base_video_player->IsEnableRotation();
    });
  }

  void MultipleChannelBaseVideoPlayer::EnableRotation(unsigned channel_id, bool enable) {
    ExecuteEx(channel_id, [enable](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      unique_channel_base_video_player->EnableRotation(enable);
    });
  }

  void MultipleChannelBaseVideoPlayer::Rotate(unsigned channel_id, RotationOptions options) {
	  ExecuteEx(channel_id, [options](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
		  unique_channel_base_video_player->Rotate(options);
	  });
  }

  RotationOptions MultipleChannelBaseVideoPlayer::GetRotateType(unsigned channel_id) const {
    RotationOptions result = RotationOptions::kDegreeUnknown;
    ExecuteEx(channel_id, [&result](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      result = unique_channel_base_video_player->GetRotateType();
    });
    return result;
  }

  void MultipleChannelBaseVideoPlayer::ImageScale(unsigned channel_id, AspectRatio ratio) {
	  ExecuteEx(channel_id, [&ratio](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
		  unique_channel_base_video_player->ImageScale(ratio);
	  });
  }

  void MultipleChannelBaseVideoPlayer::OSD(unsigned channel_id, OSDParamListPtr param) {
    ExecuteEx(channel_id, [param](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      unique_channel_base_video_player->OSD(param);
    });
  }

  bool MultipleChannelBaseVideoPlayer::IsValidRegion(unsigned channel_id, const POINT& point) const {
    return Execute(channel_id, [point](UniqueChannelBaseVideoPlayerPtr unique_channel_base_video_player) {
      return unique_channel_base_video_player->IsValidRegion(point);
    });
  }

  void MultipleChannelBaseVideoPlayer::OnVideoException(const std::string& message, int code, BaseVideoPlayerFilterPtr filter) {
    if (event_ != nullptr && filter != nullptr) {
      event_->OnVideoException(filter->channel_id, message, code);
    }
  }

  void MultipleChannelBaseVideoPlayer::OnVideoPainting(HDC hdc, BaseVideoPlayerFilterPtr filter) {
    if (event_ != nullptr && filter != nullptr) {
      event_->OnVideoPainting(filter->channel_id, hdc);
    }
  }

  void MultipleChannelBaseVideoPlayer::OnVideoFrame(VideoFramePtr video_frame, BaseVideoPlayerFilterPtr filter) {
    if (event_ != nullptr && filter != nullptr) {
      event_->OnVideoFrame(filter->channel_id, video_frame);
    }
  }
}
