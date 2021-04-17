#include "render_file_player_impl.h"
#include "media_core_errormessage.h"
#include "include/media_core_erorcode.h"

namespace core {
  RenderFilePlayerImpl::RenderFilePlayerImpl() = default;

  RenderFilePlayerImpl::~RenderFilePlayerImpl() = default;

  bool RenderFilePlayerImpl::Init(BasicPlayerParamPtr param) {
	  basic_player_param_ = param;
	  render_file_reader_ = input::RenderFileReader::CreateInstance();
    if (render_file_reader_ == nullptr) {
      return false;
    }
    render_file_reader_->SetEvent(this);
    return true;
  }

  void RenderFilePlayerImpl::Fini() {
	  render_file_reader_ = nullptr;
  }

  bool RenderFilePlayerImpl::Open(const std::string& uri) {
    if (uri.empty() || render_file_reader_ == nullptr) {
      return false;
    }
    return render_file_reader_->Open(uri);
  }

  void RenderFilePlayerImpl::Close() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Close();
  }

  bool RenderFilePlayerImpl::Play() {
    if (render_file_reader_ == nullptr) {
      return false;
    }
    return render_file_reader_->Play();
  }

  void RenderFilePlayerImpl::Stop() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Stop();
  }

  void RenderFilePlayerImpl::Pause() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Pause();
  }

  void RenderFilePlayerImpl::Resume() {
    if (render_file_reader_ == nullptr) {
      return;
    }
    render_file_reader_->Resume();
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
	  return render_file_reader_->NextFrame();
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
  }

  void RenderFilePlayerImpl::OnVideoBaseInfoChanged(input::InputMediaType type, VideoBaseInfoPtr previous_format, VideoBaseInfoPtr current_format) {
	  video_base_info_ = current_format;
	  AbstractPlayerObject::UnInitialize(MediaModelType::kVideo);
	  AbstractPlayerObject::Initialize(basic_player_param_, video_base_info_);
  }

  void RenderFilePlayerImpl::OnAudioBaseInfoChanged(input::InputMediaType type, input::AudioBaseInfoPtr previous_format, input::AudioBaseInfoPtr current_format) {
	  audio_base_info_ = current_format;
	  AbstractPlayerObject::UnInitialize(MediaModelType::kAudio);
	  AbstractPlayerObject::Initialize(basic_player_param_, audio_base_info_);
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
}
