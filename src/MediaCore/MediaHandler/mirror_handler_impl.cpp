#include "mirror_handler_impl.h"
#include <algorithm>
namespace handler {
  MirrorHandlerImpl::MirrorHandlerImpl() {
    if (mutex_ != nullptr) {
      mutex_ = utils::SharedMutex::CreateInstance();
    }
  }

  MirrorHandlerImpl::~MirrorHandlerImpl() {
	  mutex_ = nullptr;
  }

  void MirrorHandlerImpl::Start() {
	  utils::WriteLock lock(mutex_);
	  enable_ = false;
  }

  void MirrorHandlerImpl::Stop() {
	  Start();
  }

  void MirrorHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
    {
		  utils::ReadLock lock(mutex_);
      if (video_frame != nullptr && enable_) {
        HandleVideoFrame(video_frame);
      }
    }
    if (event_ != nullptr) {
      event_->OnTransmitVideoFrame(VideoHandlerType::kMirror, video_frame);
    }
  }

  void MirrorHandlerImpl::SetEvent(MirrorHandlerEvent* event) {
    if (event != nullptr && event != event_) {
      event_ = event;
    }
  }

  void MirrorHandlerImpl::EnableMirror(bool enable) {
	  utils::WriteLock lock(mutex_);
    if (enable_ != enable) {
      enable_ = enable;
    }
  }

  bool MirrorHandlerImpl::IsEnableMirror() const {
	  utils::ReadLock lock(mutex_);
	  return enable_;
  }

  void MirrorHandlerImpl::HandleVideoFrame(VideoFramePtr video_frame) {
    for (auto i = 0; i < video_frame->height; ++i) {
      const auto y_source_offset = i * video_frame->line_size[0];
      const auto y_target_offset = y_source_offset + video_frame->line_size[0] / 2;
	    std::reverse(video_frame->data[0]->begin() + y_source_offset, video_frame->data[0]->begin() + y_target_offset);
		  std::reverse(video_frame->data[0]->begin() + y_target_offset, video_frame->data[0]->begin() + y_target_offset + video_frame->line_size[0] / 2);
		  std::swap_ranges(video_frame->data[0]->begin() + y_source_offset, video_frame->data[0]->begin() + y_target_offset, video_frame->data[0]->begin() + y_target_offset);
      if (i < video_frame->height / 2) {
		    const auto uv_source_offset = i * video_frame->line_size[1];
		    const auto uv_target_offset = uv_source_offset + video_frame->line_size[1] / 2;
        for (auto k = 1; k <= 2; ++k) {
		      std::reverse(video_frame->data[k]->begin() + uv_source_offset, video_frame->data[k]->begin() + uv_target_offset);
		      std::reverse(video_frame->data[k]->begin() + uv_target_offset, video_frame->data[k]->begin() + uv_target_offset + video_frame->line_size[1] / 2);
		      std::swap_ranges(video_frame->data[k]->begin() + uv_source_offset, video_frame->data[k]->begin() + uv_target_offset, video_frame->data[k]->begin() + uv_target_offset);         
        }
      }
    }
  }
}
