#include "flip_handler_impl.h"
#include <algorithm>
namespace handler {
  FlipHandlerImpl::FlipHandlerImpl() = default;

  FlipHandlerImpl::~FlipHandlerImpl() = default;

  void FlipHandlerImpl::Start() {
	  orientation_ = 0;
  }

  void FlipHandlerImpl::Stop() {
	  Start();
  }

  void FlipHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
    if (orientation_ != 0) {
      if (IsHorizontalFlip()) {
        HorizontalFlip(video_frame);
      }
      if (IsVerticalFlip()) {
        VerticalFlip(video_frame);
      }
    }
    if (event_ != nullptr) {
      event_->OnTransmitVideoFrame(VideoHandlerType::kFlip, video_frame);
    }
  }

  void FlipHandlerImpl::SetEvent(FlipHandlerEvent* event) {
    if (event != nullptr && event != event_) {
      event_ = event;
    }
  }

  void FlipHandlerImpl::Flip(uint16_t orientation) {
    std::lock_guard<std::mutex> lock(mutex_);
    orientation_ = orientation;
  }

  uint16_t FlipHandlerImpl::State() const {
	  return orientation_;
  }

  inline bool FlipHandlerImpl::IsHorizontalFlip() const {
	  std::lock_guard<std::mutex> lock(mutex_);
	  return (orientation_ >> 8 & 0xff) != 0;
  }

  void FlipHandlerImpl::HorizontalFlip(VideoFramePtr video_frame) {
    if (video_frame == nullptr) {
      return;
    }
    auto y_offset = 0;
	  auto uv_offset = 0;
    for (auto i = 0;i < video_frame->height;++i) {
		  std::reverse(video_frame->data[0]->begin() + y_offset, video_frame->data[0]->begin() + y_offset + video_frame->line_size[0]);
			y_offset += video_frame->line_size[0];
      if (i < video_frame->height / 2) {
		    std::reverse(video_frame->data[1]->begin() + uv_offset, video_frame->data[1]->begin() + uv_offset + video_frame->line_size[1]);
		    std::reverse(video_frame->data[2]->begin() + uv_offset, video_frame->data[2]->begin() + uv_offset + video_frame->line_size[2]);
		    uv_offset += video_frame->line_size[1];
      }
    }
  }

  inline bool FlipHandlerImpl::IsVerticalFlip() const {
	  std::lock_guard<std::mutex> lock(mutex_);
	  return (orientation_ & 0xff) != 0;
  }

  void FlipHandlerImpl::VerticalFlip(VideoFramePtr video_frame) {
    if (video_frame == nullptr) {
      return;
    }
    for (auto i = 0; i < video_frame->height / 2; ++i) {
      auto y_source = i * video_frame->line_size[0];
      auto y_target = (video_frame->height - 1 - i) * video_frame->line_size[0];
      std::swap_ranges(video_frame->data[0]->begin() + y_source,video_frame->data[0]->begin() + y_source + video_frame->line_size[0],video_frame->data[0]->begin() + y_target);
      if (i < video_frame->height / 4) {
        auto uv_source = i * video_frame->line_size[1];
        auto uv_target = (video_frame->height / 2 - 1 - i) * video_frame->line_size[1];
         std::swap_ranges(video_frame->data[1]->begin() + uv_source, video_frame->data[1]->begin() + uv_source + video_frame->line_size[1], video_frame->data[1]->begin() +uv_target);
         std::swap_ranges(video_frame->data[2]->begin() + uv_source, video_frame->data[2]->begin() + uv_source + video_frame->line_size[2], video_frame->data[2]->begin() + uv_target);
      }
    }
  }
}
