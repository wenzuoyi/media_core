#include "render_file_reader_impl.h"
#include <regex>
#include <algorithm>
namespace input {
  RenderFileReaderImpl::RenderFileReaderImpl() {
    binary_file_reader_.SetEvent(this);
	  player_pause_control_ = utils::Event::CreateInstance(true);
	  player_pause_control_->NotifyAll();
	  player_singleframe_control_ = utils::Event::CreateInstance(false);
  }

  RenderFileReaderImpl::~RenderFileReaderImpl() {
  }

  void RenderFileReaderImpl::OnRequestFrameSize(uint64_t* frame_size) {
    if (player_pause_control_ != nullptr) {
      player_pause_control_->Wait();
    }
    if (single_frame_flag_ && player_singleframe_control_ != nullptr) {
      player_singleframe_control_->Wait();
    }
    *frame_size = frame_size_;
  }

  void RenderFileReaderImpl::OnPostBinaryData(const std::vector<char>& buffer, uint64_t offset) {
    if (event_ != nullptr) {
      auto package = std::make_shared<VideoPackage>();
      package->base_info.width = with_;
      package->base_info.height = height_;
      if (format_ == RenderFormat::kYUV420) {
        package->base_info.type = VideoPackageType::kYUV420;
      } else if (format_ == RenderFormat::kYUV422) {
        package->base_info.type = VideoPackageType::kYUV422;
      }
      package->data = std::make_shared<Data>(buffer.begin(), buffer.end());
      package->time_stamp = 0;
      event_->OnDemuxVideoPackage(InputMediaType::kRenderFile, package);
    }
  }

  void RenderFileReaderImpl::OnEOF() {
    if (event_ != nullptr) {
      event_->OnEOF(InputMediaType::kRenderFile);
    }
  }

  void RenderFileReaderImpl::OnBOF() {
	  if (event_ != nullptr) {
		  event_->OnBOF(InputMediaType::kRenderFile);
	  }
  }

  void RenderFileReaderImpl::SetEvent(RenderFileReaderEvent* event) {
    if (event_ != event && event != nullptr) {
      event_ = event;
    }
  }

  bool RenderFileReaderImpl::Open(const std::string& uri) {
    std::regex regex{R"([\s\S]+\.(yuv|rgb|yuv420|yuv422)$)"};
    std::smatch matches;
    if (!std::regex_match(uri, matches, regex)) {
      return false;
    }
    return binary_file_reader_.Open(uri);
  }

  void RenderFileReaderImpl::Close() {
	  binary_file_reader_.Close();
  }

  bool RenderFileReaderImpl::Play() {
	  return binary_file_reader_.Start();
  }

  void RenderFileReaderImpl::Stop() {
	  binary_file_reader_.Stop();	  
  }

  void RenderFileReaderImpl::Pause() {
    if (player_pause_control_ != nullptr) {
      player_pause_control_->Reset();
    }
  }

  void RenderFileReaderImpl::Resume() {
    if (player_pause_control_ != nullptr) {
      player_pause_control_->NotifyAll();
    }
    
  }

  bool RenderFileReaderImpl::Seek(int64_t time_stamp) {
	  return false;
  }

  bool RenderFileReaderImpl::NextFrame() {
    single_frame_flag_ = true;
    if (player_singleframe_control_ != nullptr) {
      player_singleframe_control_->NotifyAll();
    }
    return true;
  }

  void RenderFileReaderImpl::Speed(double speed) {
	  binary_file_reader_.Speed(speed);
  }

  void RenderFileReaderImpl::RPlay(bool enable) {	  
	  binary_file_reader_.EnableReversePlayback(enable);
  }

  void RenderFileReaderImpl::PreviousFrame() {
	  RPlay(true);
	  NextFrame();
  }

  void RenderFileReaderImpl::SetFormat(RenderFormat format) {
    if (format != format_) {
      format_ = format;
    }
    GetFrameSize();
  }
  
  void RenderFileReaderImpl::SetResolution(const std::string& resolution) {
    std::regex regex{R"(([0-9]+)[xX]([0-9]+))"};
    std::smatch matches;
    if (std::regex_match(resolution, matches, regex)) {
      if (matches.size() == 2) {
        with_ = std::stoi(matches[0].str());
        height_ = std::stoi(matches[1].str());
      }
    }
    GetFrameSize();
  }

  void RenderFileReaderImpl::GetFrameSize() {
    if (format_ == RenderFormat::kYUV420) {
      frame_size_ = with_ * height_ * 3 / 2;
    } else if (format_ == RenderFormat::kYUV422) {
      frame_size_ = with_ * height_ * 2;
    }
  }
}
