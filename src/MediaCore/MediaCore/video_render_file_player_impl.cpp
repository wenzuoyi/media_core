#include "video_render_file_player_impl.h"
#include "media_core_errormessage.h"
#include "include/media_core_erorcode.h"

namespace core {
  VideoRenderFilePlayerImpl::VideoRenderFilePlayerImpl() = default;

  VideoRenderFilePlayerImpl::~VideoRenderFilePlayerImpl() = default;

  bool VideoRenderFilePlayerImpl::Init(BasicPlayerParamPtr param) {
	  basic_player_param_ = param;
	  render_file_reader_ = input::RenderFileReader::CreateInstance();
    if (render_file_reader_ == nullptr) {
      return false;
    }
    render_file_reader_->SetEvent(this);
    player_status_ = PlayerStatus::kClose;
    return true;
  }

  void VideoRenderFilePlayerImpl::Fini() {
    render_file_reader_ = nullptr;
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->Fini();
    }
    unique_channel_uncompress_video_player_ = nullptr;
  }

  bool VideoRenderFilePlayerImpl::Open(const std::string& uri) {
    if (uri.empty() || render_file_reader_ == nullptr) {
      return false;
    }
    const auto retvalue = render_file_reader_->Open(uri);
    if (retvalue) {
      player_status_ = PlayerStatus::kStop;
    }
    return retvalue;
  }

  void VideoRenderFilePlayerImpl::Close() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Close();
    player_status_ = PlayerStatus::kClose;
  }

  bool VideoRenderFilePlayerImpl::Play() {
    if (render_file_reader_ == nullptr) {
      return false;
    }
    const auto retvalue = render_file_reader_->Play();
    if (retvalue) {
      player_status_ = PlayerStatus::kRun;
    }
    return retvalue;
  }

  void VideoRenderFilePlayerImpl::Stop() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Stop();
    player_status_ = PlayerStatus::kStop;
  }

  void VideoRenderFilePlayerImpl::Pause() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Pause();
    player_status_ = PlayerStatus::kPause;
  }

  void VideoRenderFilePlayerImpl::Resume() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Resume();
    player_status_ = PlayerStatus::kRun;
  }

  bool VideoRenderFilePlayerImpl::Seek(int64_t time_stamp) {
	  if (render_file_reader_ == nullptr) {
		  return false;
	  }
	  return render_file_reader_->Seek(time_stamp);
  }

  bool VideoRenderFilePlayerImpl::NextFrame() {
	  if (render_file_reader_ == nullptr) {
		  return false;
	  }
    const auto retvalue = render_file_reader_->NextFrame();
    if (retvalue) {
      player_status_ = PlayerStatus::kSingleFrameForward;
    }
    return retvalue;
  }

  void VideoRenderFilePlayerImpl::Speed(double speed) {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Speed(speed);
  }

  void VideoRenderFilePlayerImpl::Mute(bool enable) {
    if (event_ != nullptr) {
      event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
    }
  }

  void VideoRenderFilePlayerImpl::SetVolume(int volume) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  int VideoRenderFilePlayerImpl::GetVolume() {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
	  return 0;
  }

  bool VideoRenderFilePlayerImpl::Snapshot(const std::string& url) {
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->Snapshot(url);
    }
    return false;
  }

  bool VideoRenderFilePlayerImpl::IsZoom() const {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return false;
    }
    return unique_channel_uncompress_video_player_->IsZoom();
  }

  void VideoRenderFilePlayerImpl::Zoom(RegionPtr region) {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return;
    }
    unique_channel_uncompress_video_player_->Zoom(region);
  }

  bool VideoRenderFilePlayerImpl::IsSettingMosaic() const {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return false;
    }
    return unique_channel_uncompress_video_player_->IsSettingMosaic();
  }

  void VideoRenderFilePlayerImpl::Mosaic(RegionPtr region) {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return;
    }
    unique_channel_uncompress_video_player_->Mosaic(region);
  }

  bool VideoRenderFilePlayerImpl::IsMirror() const {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return false;
    }
    return unique_channel_uncompress_video_player_->IsMirror();
  }

  void VideoRenderFilePlayerImpl::Mirror(bool enable) {
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->Mirror(enable);
    }
  }

  void VideoRenderFilePlayerImpl::EnableRotation(bool enable) {
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->EnableRotation(enable);
    }
  }

  bool VideoRenderFilePlayerImpl::IsEnableRotation() const {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return false;
    }
    return unique_channel_uncompress_video_player_->IsEnableRotation();
  }

  void VideoRenderFilePlayerImpl::Rotate(RotationOptions options) {
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->Rotate(options);
    }
  }

  RotationOptions VideoRenderFilePlayerImpl::GetRotateType() const {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return RotationOptions::kDegreeUnknown;
    }
    return unique_channel_uncompress_video_player_->GetRotateType();
  }

  bool VideoRenderFilePlayerImpl::SwitchStream(StreamType type) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
	  return false;
  }

  void VideoRenderFilePlayerImpl::ImageScale(AspectRatio ratio) {
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->ImageScale(ratio);
    }
  }

  void VideoRenderFilePlayerImpl::OSD(OSDParamListPtr param) {
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->OSD(param);
    }
  }

  void VideoRenderFilePlayerImpl::RPlay(bool enable) {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Play();
  }

  void VideoRenderFilePlayerImpl::PreviousFrame() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->PreviousFrame();
    player_status_ = PlayerStatus::kSingleFrameBackward;
  }

  void VideoRenderFilePlayerImpl::EnableLoopPlayback(bool enable) {
	  if (render_file_reader_ == nullptr) {
		  return;
	  }
	  render_file_reader_->EnableLoopPlayback(enable);
  }

  void VideoRenderFilePlayerImpl::SetEvent(VideoRenderFilePlayerEvent* event) {
    if (event != nullptr && event_ != event) {
      event_ = event;
    }
  }

  void VideoRenderFilePlayerImpl::SetFormat(RenderFormat format) {
    if (render_file_reader_ != nullptr) {
      render_file_reader_->SetFormat(format);
    }
  }

  void VideoRenderFilePlayerImpl::SetResolution(const std::string& resolution) {
    if (render_file_reader_ != nullptr) {
      render_file_reader_->SetResolution(resolution);
    }
  }

  int VideoRenderFilePlayerImpl::Width() const {
	  return video_base_info_->width;
  }

  int VideoRenderFilePlayerImpl::Height() const {
	  return video_base_info_->height;
  }

  bool VideoRenderFilePlayerImpl::IsValidRegion(const POINT& point) const {
    if (unique_channel_uncompress_video_player_ == nullptr) {
      return false;
    }
    return unique_channel_uncompress_video_player_->IsValidRegion(point);
  }

  BasePlayer::PlayerStatus VideoRenderFilePlayerImpl::Status() const {
	  return player_status_;
  }

  void VideoRenderFilePlayerImpl::OnDemuxException(int error_code, const std::string& error_message) {
    if (event_ != nullptr) {
      event_->OnPlayerException(error_code, error_message);
    }
  }

  void VideoRenderFilePlayerImpl::OnDemuxAudioPackage(input::InputMediaType type, AudioPackagePtr package) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  void VideoRenderFilePlayerImpl::OnDemuxVideoPackage(input::InputMediaType type, VideoPackagePtr package) {
    if (type != input::InputMediaType::kRenderFile) {
      return;
    }
    if (package == nullptr || package->base_info == nullptr || package->data == nullptr) {
      return;
    }
    auto object = CreateYUVObject(package);
    if (event_ != nullptr) {
      event_->OnPreRenderingFrame(object);
    }
    if (object != nullptr && unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->InputVideoFrame(object);
    }
  }

  void VideoRenderFilePlayerImpl::OnVideoBaseInfoChanged(input::InputMediaType type, VideoBaseInfoPtr previous_format, VideoBaseInfoPtr current_format) {
	  video_base_info_ = current_format;
    if (unique_channel_uncompress_video_player_ != nullptr) {
      unique_channel_uncompress_video_player_->Fini();
      unique_channel_uncompress_video_player_ = nullptr;
    }
    if (unique_channel_uncompress_video_player_ == nullptr) {
      unique_channel_uncompress_video_player_ = std::make_shared<UniqueChannelUnCompressVideoPlayer>();
      unique_channel_uncompress_video_player_->SetEvent(this);
      unique_channel_uncompress_video_player_->Init(basic_player_param_, video_base_info_);
    }
  }

  void VideoRenderFilePlayerImpl::OnAudioBaseInfoChanged(input::InputMediaType type, input::AudioBaseInfoPtr previous_format, input::AudioBaseInfoPtr current_format) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  void VideoRenderFilePlayerImpl::OnEOF(input::InputMediaType type) {
    if (event_ != nullptr) {
      event_->OnEOF();
    }
  }

  void VideoRenderFilePlayerImpl::OnBOF(input::InputMediaType type) {
    if (event_ != nullptr) {
      event_->OnBOF();
    }
  }

  void VideoRenderFilePlayerImpl::OnVideoException(const std::string& message, int code, BaseVideoPlayerFilterPtr filter) {
    if (event_ != nullptr) {
      event_->OnPlayerException(code, message);
    }
  }

  void VideoRenderFilePlayerImpl::OnVideoPainting(HDC hdc, BaseVideoPlayerFilterPtr filter) {
    if (event_ != nullptr) {
      event_->OnExtraDisplay(hdc);
    }
  }

  void VideoRenderFilePlayerImpl::OnVideoFrame(VideoFramePtr video_frame, BaseVideoPlayerFilterPtr filter) {
    if (event_ != nullptr) {
      event_->OnPostRenderingFrame(video_frame);
    }
  }

  VideoFramePtr VideoRenderFilePlayerImpl::CreateYUVObject(VideoPackagePtr package) {
	  auto offset = 0;
	  const auto y_stride = package->base_info->width;
	  const auto y_size = y_stride * package->base_info->height;
	  auto y_data = std::make_shared<output::Buffer>(package->data->begin() + offset, package->data->begin() + offset + y_size);
	  offset += y_size;
	  const auto u_stride = y_stride / 2;
	  int u_size = 0;
    if (package->base_info->type == input::VideoPackageType::kYUV420) {
	  	u_size = u_stride * (package->base_info->height / 2);
    } else if (package->base_info->type == input::VideoPackageType::kYUV422)  {
      u_size = u_stride * (package->base_info->height);
    }    
	  auto u_data = std::make_shared<output::Buffer>(package->data->begin() + offset, package->data->begin() + offset + u_size);
	  offset += u_size;
	  const auto v_stride = y_stride / 2;
    int v_size = 0;
    if (package->base_info->type == input::VideoPackageType::kYUV420) {
      v_size = v_stride * (package->base_info->height / 2);
    } else if (package->base_info->type == input::VideoPackageType::kYUV422) {
      v_size = v_stride * package->base_info->height;
    }
	  auto v_data = std::make_shared<output::Buffer>(package->data->begin() + offset, package->data->begin() + offset + v_size);
	  auto video_frame = std::make_shared<output::VideoFrame>();
	  video_frame->width = package->base_info->width;
	  video_frame->height = package->base_info->height;
	  video_frame->data[0] = y_data;
	  video_frame->line_size[0] = y_stride;
	  video_frame->data[1] = u_data;
	  video_frame->line_size[1] = u_stride;
	  video_frame->data[2] = v_data;
	  video_frame->line_size[2] = v_stride;
	  return video_frame;
  }
}
