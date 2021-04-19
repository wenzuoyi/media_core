#include "render_file_player_impl.h"
#include "media_core_errormessage.h"
#include "include/media_core_erorcode.h"

namespace core {
  RenderFilePlayerImpl::RenderFilePlayerImpl() = default;

  RenderFilePlayerImpl::~RenderFilePlayerImpl() {
    
  }

  bool RenderFilePlayerImpl::Init(BasicPlayerParamPtr param) {
	  basic_player_param_ = param;
	  render_file_reader_ = input::RenderFileReader::CreateInstance();
    if (render_file_reader_ == nullptr) {
      return false;
    }
    render_file_reader_->SetEvent(this);
    player_status_ = PlayerStatus::kClose;
    return true;
  }

  void RenderFilePlayerImpl::Fini() {
	  render_file_reader_ = nullptr;
	  AbstractPlayerObject::UnInitialize(MediaModelType::kVideo);
  }

  bool RenderFilePlayerImpl::Open(const std::string& uri) {
    if (uri.empty() || render_file_reader_ == nullptr) {
      return false;
    }
    const auto retvalue = render_file_reader_->Open(uri);
    if (retvalue) {
      player_status_ = PlayerStatus::kStop;
    }
    return retvalue;
  }

  void RenderFilePlayerImpl::Close() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Close();
    player_status_ = PlayerStatus::kClose;
  }

  bool RenderFilePlayerImpl::Play() {
    if (render_file_reader_ == nullptr) {
      return false;
    }
    const auto retvalue = render_file_reader_->Play();
    if (retvalue) {
      player_status_ = PlayerStatus::kRun;
    }
    return retvalue;
  }

  void RenderFilePlayerImpl::Stop() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Stop();
    player_status_ = PlayerStatus::kStop;
  }

  void RenderFilePlayerImpl::Pause() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Pause();
    player_status_ = PlayerStatus::kPause;
  }

  void RenderFilePlayerImpl::Resume() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Resume();
    player_status_ = PlayerStatus::kRun;
  }

  bool RenderFilePlayerImpl::Seek(int64_t time_stamp) {
	  if (render_file_reader_ == nullptr) {
		  return false;
	  }
	  return render_file_reader_->Seek(time_stamp);
  }

  bool RenderFilePlayerImpl::NextFrame() {
	  if (render_file_reader_ == nullptr) {
		  return false;
	  }
    const auto retvalue = render_file_reader_->NextFrame();
    if (retvalue) {
      player_status_ = PlayerStatus::kSingleFrameForward;
    }
    return retvalue;
  }

  void RenderFilePlayerImpl::Speed(double speed) {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Speed(speed);
  }

  void RenderFilePlayerImpl::Mute(bool enable) {
    if (event_ != nullptr) {
      event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
    }
  }

  void RenderFilePlayerImpl::SetVolume(int volume) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  int RenderFilePlayerImpl::GetVolume() {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
	  return 0;
  }

  bool RenderFilePlayerImpl::Snapshot(const std::string& url) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
	  return false;
  }

  bool RenderFilePlayerImpl::IsZoom() const {
    if (video_output_ == nullptr) {
      return false;
    }
    return video_output_->IsROIEnable();
  }

  void RenderFilePlayerImpl::Zoom(RegionPtr region) {
    if (video_output_ == nullptr) {
      return;
    }
    video_output_->EnableROI(region != nullptr);
    if (region != nullptr) {
      video_output_->UpdateROI(*region);
    }
  }

  void RenderFilePlayerImpl::Mosaic(RegionPtr region) {
    if (mosaic_handler_ == nullptr) {
      return;
    }
    mosaic_handler_->EnableMosaic(region != nullptr);
    auto mosaic = std::make_shared<handler::MosaicParam>();
    mosaic->x = region->left;
    mosaic->y = region->top;
    mosaic->width = region->right - region->left;
    mosaic->height = region->bottom - region->top;
    mosaic_handler_->SetParam(mosaic);
  }

  void RenderFilePlayerImpl::Flip() {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  void RenderFilePlayerImpl::Mirror() {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  void RenderFilePlayerImpl::Rotate(RotationOptions options) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  bool RenderFilePlayerImpl::SwitchStream(StreamType type) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
	  return false;
  }

  void RenderFilePlayerImpl::ImageScale(AspectRatio ratio) {
    if (video_output_ == nullptr) {
      return;
    }
    video_output_->SetDisplayRatio(ratio);
  }

  void RenderFilePlayerImpl::OSD(OSDParamListPtr param) {
	  if (video_output_ == nullptr) {
		  return;
	  }
	  video_output_->SetOSD(param);
  }

  void RenderFilePlayerImpl::RPlay(bool enable) {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Play();
  }

  void RenderFilePlayerImpl::PreviousFrame() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->PreviousFrame();
    player_status_ = PlayerStatus::kSingleFrameBackward;
  }

  void RenderFilePlayerImpl::EnableLoopPlayback(bool enable) {
	  if (render_file_reader_ == nullptr) {
		  return;
	  }
	  render_file_reader_->EnableLoopPlayback(enable);
  }

  void RenderFilePlayerImpl::SetEvent(RenderFilePlayerEvent* event) {
    if (event != nullptr && event_ != event) {
      event_ = event;
    }
  }

  void RenderFilePlayerImpl::SetFormat(RenderFormat format) {
    if (render_file_reader_ != nullptr) {
      render_file_reader_->SetFormat(format);
    }
  }

  void RenderFilePlayerImpl::SetResolution(const std::string& resolution) {
    if (render_file_reader_ != nullptr) {
      render_file_reader_->SetResolution(resolution);
    }
  }

  int RenderFilePlayerImpl::Width() const {
	  return video_base_info_->width;
  }

  int RenderFilePlayerImpl::Height() const {
	  return video_base_info_->height;
  }

  bool RenderFilePlayerImpl::IsValidRegion(const POINT& point) const {
    if (video_output_ == nullptr) {
      return false;
    }
    return video_output_->IsValidRendingArea(point);
  }

  BasePlayer::PlayerStatus RenderFilePlayerImpl::Status() const {
	  return player_status_;
  }

  void RenderFilePlayerImpl::OnDemuxException(int error_code, const std::string& error_message) {
    if (event_ != nullptr) {
      event_->OnPlayerException(error_code, error_message);
    }
  }

  void RenderFilePlayerImpl::OnDemuxAudioPackage(input::InputMediaType type, AudioPackagePtr package) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  void RenderFilePlayerImpl::OnDemuxVideoPackage(input::InputMediaType type, VideoPackagePtr package) {
    if (type != input::InputMediaType::kRenderFile) {
      return;
    }
    if (package == nullptr || package->base_info == nullptr || package->data == nullptr) {
      return;
    }
    if (mosaic_handler_ == nullptr) {
      return;
    }
    auto object = CreateYUVObject(package);
    if (object != nullptr) {
      mosaic_handler_->InputVideoFrame(object);
    }
  }

  void RenderFilePlayerImpl::OnVideoBaseInfoChanged(input::InputMediaType type, VideoBaseInfoPtr previous_format, VideoBaseInfoPtr current_format) {
	  video_base_info_ = current_format;
	  AbstractPlayerObject::UnInitialize(MediaModelType::kVideo);
	  AbstractPlayerObject::Initialize(basic_player_param_, video_base_info_);
  }

  void RenderFilePlayerImpl::OnAudioBaseInfoChanged(input::InputMediaType type, input::AudioBaseInfoPtr previous_format, input::AudioBaseInfoPtr current_format) {
	  if (event_ != nullptr) {
		  event_->OnPlayerException(ERROR_CODE_UNSUPPORTED_METHOD, std::string(error_message[ERROR_CODE_UNSUPPORTED_METHOD]));
	  }
  }

  void RenderFilePlayerImpl::OnEOF(input::InputMediaType type) {
    if (event_ != nullptr) {
      event_->OnEOF();
    }
  }

  void RenderFilePlayerImpl::OnBOF(input::InputMediaType type) {
    if (event_ != nullptr) {
      event_->OnBOF();
    }
  }

  void RenderFilePlayerImpl::OnVideoCustomPainting(HDC hdc) {
    if (event_ != nullptr) {
      event_->OnExtraDisplay(hdc);
    }
  }

  VideoFramePtr RenderFilePlayerImpl::CreateYUVObject(VideoPackagePtr package) {
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
