#include "unique_channel_base_video_player.h"

namespace core {
	UniqueChannelBaseVideoPlayerEvent::UniqueChannelBaseVideoPlayerEvent() = default;

	UniqueChannelBaseVideoPlayerEvent::~UniqueChannelBaseVideoPlayerEvent() = default;

  UniqueChannelBaseVideoPlayer::UniqueChannelBaseVideoPlayer() = default;

	UniqueChannelBaseVideoPlayer::~UniqueChannelBaseVideoPlayer() = default;

  void UniqueChannelBaseVideoPlayer::SetEvent(UniqueChannelBaseVideoPlayerEvent* event, BaseVideoPlayerFilterPtr filter) {
    event_ = event;
    filter_ = filter;
  }

  bool UniqueChannelBaseVideoPlayer::Init(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) {
    mirror_handler_ = handler::MirrorHandler::CreateInstance();
    if (mirror_handler_ == nullptr) {
      return false;
    }
    mirror_handler_->SetEvent(this);
    mirror_handler_->Start();
    mosaic_handler_ = handler::MosaicHandler::CreateInstance();
    if (mosaic_handler_ == nullptr) {
      return false;
    }
    mosaic_handler_->SetEvent(this);
    mosaic_handler_->Start();
    if (!mosaic_handler_->Clear()) {
      return false;
    }
    rotation_handler_ = handler::RotationHandler::CreateInstance();
    if (rotation_handler_ == nullptr) {
      return false;
    }
    rotation_handler_->SetEvent(this);
    rotation_handler_->Start();
    snapshot_handler_ = handler::SnapshotHandler::CreateInstance();
    if (snapshot_handler_ == nullptr) {
      return false;
    }
    snapshot_handler_->SetEvent(this);
    snapshot_handler_->Start();
    video_output_ = output::VideoOutputMediaSource::CreateInstance(basic_player_info->video_render_mode);
    video_output_->Init();
    video_output_->SetEvent(this);
    auto param = std::make_shared<output::VideoOutputParam>();
    param->width = video_base_info->width;
    param->height = video_base_info->height;
    param->render_wnd = basic_player_info->hwnd;
    video_output_->SetVideoOutputMediaParam(param);
    return video_output_->Play();
  }

  void UniqueChannelBaseVideoPlayer::Fini() {
    if (video_output_ != nullptr) {
      video_output_->Stop();
      video_output_->Fini();
      video_output_ = nullptr;
    }
    if (mosaic_handler_ != nullptr) {
      mosaic_handler_->Stop();
      mosaic_handler_ = nullptr;
    }
    if (mirror_handler_ != nullptr) {
      mirror_handler_->Stop();
      mirror_handler_ = nullptr;
    }
    if (rotation_handler_ != nullptr) {
      rotation_handler_->Stop();
      rotation_handler_ = nullptr;
    }
    if (snapshot_handler_ != nullptr) {
      snapshot_handler_->Stop();
      snapshot_handler_ = nullptr;
    }
  }

  bool UniqueChannelBaseVideoPlayer::Snapshot(const std::string& url) {
	  if (snapshot_handler_ == nullptr) {
		  return false;
	  }
	  return snapshot_handler_->Save(url);
  }

  bool UniqueChannelBaseVideoPlayer::IsZoom() const {
	  if (video_output_ == nullptr) {
		  return false;
	  }
	  return video_output_->IsROIEnable();
  }

  void UniqueChannelBaseVideoPlayer::Zoom(RegionPtr region) {
	  if (video_output_ == nullptr) {
		  return;
	  }
	  video_output_->EnableROI(region != nullptr);
	  if (region != nullptr) {
		  video_output_->UpdateROI(*region);
	  }
  }

  bool UniqueChannelBaseVideoPlayer::IsSettingMosaic() const {
	  if (mosaic_handler_ == nullptr) {
		  return false;
	  }
	  return mosaic_handler_->IsSettingMosaic();
  }

  void UniqueChannelBaseVideoPlayer::Mosaic(RegionPtr region) {
    if (mosaic_handler_ == nullptr) {
      return;
    }
    mosaic_handler_->EnableMosaic(region != nullptr);
    if (region != nullptr) {
      auto mosaic = std::make_shared<handler::MosaicParam>();
      mosaic->x = region->left;
      mosaic->y = region->top;
      mosaic->width = region->right - region->left;
      mosaic->height = region->bottom - region->top;
      mosaic_handler_->SetParam(mosaic);
    } else {
      mosaic_handler_->Clear();
    }
  }

  bool UniqueChannelBaseVideoPlayer::IsMirror() const {
	  if (mirror_handler_ == nullptr) {
		  return false;
	  }
	  return mirror_handler_->IsEnableMirror();
  }

  void UniqueChannelBaseVideoPlayer::Mirror(bool enable) {
	  if (mirror_handler_ != nullptr) {
		  mirror_handler_->EnableMirror(enable);
	  }
  }

  bool UniqueChannelBaseVideoPlayer::IsEnableRotation() const {
	  if (rotation_handler_ == nullptr) {
		  return false;
	  }
	  return rotation_handler_->GetEnableRotation();
  }

  void UniqueChannelBaseVideoPlayer::EnableRotation(bool enable) {
	  if (rotation_handler_ != nullptr) {
		  rotation_handler_->EnableRotation(enable);
	  }
  }

  void UniqueChannelBaseVideoPlayer::Rotate(RotationOptions options) {
	  if (rotation_handler_ != nullptr) {
		  rotation_handler_->Rotate(static_cast<handler::RotationDegreeType>(options));
	  }
  }

  RotationOptions UniqueChannelBaseVideoPlayer::GetRotateType() const {
	  if (rotation_handler_ == nullptr) {
		  return RotationOptions::kDegreeUnknown;
	  }
	  return static_cast<RotationOptions>(rotation_handler_->GetRotateType());
  }

  void UniqueChannelBaseVideoPlayer::ImageScale(AspectRatio ratio) {
	  if (video_output_ == nullptr) {
		  return;
	  }
	  video_output_->SetDisplayRatio(ratio);
  }

  void UniqueChannelBaseVideoPlayer::OSD(OSDParamListPtr param) {
	  if (video_output_ == nullptr) {
		  return;
	  }
	  video_output_->SetOSD(param);
  }

  bool UniqueChannelBaseVideoPlayer::IsValidRegion(const POINT& point) const {
	  if (video_output_ == nullptr) {
		  return false;
	  }
	  return video_output_->IsValidRendingArea(point);
  }

  void UniqueChannelBaseVideoPlayer::OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type,  VideoFramePtr video_frame) {
	  if (video_handler_type == handler::VideoHandlerType::kMosaic && mirror_handler_ != nullptr) {
		  mirror_handler_->InputVideoFrame(video_frame);
	  }
	  if (video_handler_type == handler::VideoHandlerType::kMirror && rotation_handler_ != nullptr) {
		  rotation_handler_->InputVideoFrame(video_frame);
	  }
	  if (video_handler_type == handler::VideoHandlerType::kRotate && snapshot_handler_ != nullptr) {
		  snapshot_handler_->InputVideoFrame(video_frame);
	  }
	  if (video_handler_type == handler::VideoHandlerType::kSnapshot && video_output_ != nullptr) {
		  if (!video_output_->Renderable(video_frame)) {
			  auto param = video_output_->GetVideoOutputMediaParam();
			  video_output_->Stop();
			  param->width = video_frame->width;
			  param->height = video_frame->height;
			  video_output_->SetVideoOutputMediaParam(param);
			  video_output_->Play();
		  }
		  video_output_->InputVideoFrame(video_frame);
	  }
  }

  void UniqueChannelBaseVideoPlayer::OnHandlerException(handler::VideoHandlerType video_handler_type,   const std::string& message, int code) {
	  if (event_ != nullptr) {
		  event_->OnVideoException(message, code, filter_);
	  }
  }

  void UniqueChannelBaseVideoPlayer::OnVideoOutputException(unsigned error_code) {
	  if (event_ != nullptr) {
		  event_->OnVideoException(std::string("output exception:"), error_code, filter_);
	  }
  }

  void UniqueChannelBaseVideoPlayer::OnVideoCustomPainting(HDC hdc) {
	  if (event_ != nullptr) {
		  event_->OnVideoPainting(hdc, filter_);
	  }
  }

  void UniqueChannelBaseVideoPlayer::OnVideoTransmitFrame(VideoFramePtr video_frame) {
	  if (event_ != nullptr) {
		  event_->OnVideoFrame(video_frame, filter_);
	  }
  }
}

