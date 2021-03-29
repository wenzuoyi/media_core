#include "mosaic_handler_impl.h"
#include <algorithm>
namespace handler {
	MosaicHandlerImpl::MosaicHandlerImpl() = default;

	MosaicHandlerImpl::~MosaicHandlerImpl() = default;

  void MosaicHandlerImpl::SetEvent(MosaicHandlerEvent* event) {
    if (event != event_) {
      event_ = event;
    }
  }

  void MosaicHandlerImpl::Start() {
	  enable_ = false;
	  mosaic_param_ = nullptr;
  }

  void MosaicHandlerImpl::Stop() {
	  enable_ = false;
	  mosaic_param_ = nullptr;
  }

  void MosaicHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
    if (enable_ && video_frame != nullptr) {
        for (auto i = 0U;  i < matrix_a_.size(); ++i) {
          SwapYUVBlock(i, video_frame);
        }
    }
    event_->OnTransmitVideoFrame(VideoHandlerType::kMosaic, video_frame);
  }

  void MosaicHandlerImpl::EnableMosaic(bool enable) {
    if (enable_ != enable) {
      enable_ = enable;
    }
  }

  bool MosaicHandlerImpl::SetParam(MosaicParamPtr param) {
    if (param == nullptr) {
      return false;
    }
    mosaic_param_ = param;
	  const auto horizontal_count = mosaic_param_->width / mosaic_param_->per_block_pixel;
	  const auto vertical_count = mosaic_param_->height / mosaic_param_->per_block_pixel;
	  for (auto u = 0; u < horizontal_count; ++u) {
      for (auto v = 0; v < vertical_count; ++v) {
        if (matrix_a_.size() < (horizontal_count * vertical_count / 2)) {
          matrix_a_.emplace_back(std::make_pair(u, v));
        } else {
          matrix_b_.emplace_back(std::make_pair(u, v));
        }
      }
    }
    std::random_shuffle(matrix_a_.begin(), matrix_a_.end());
    std::random_shuffle(matrix_b_.begin(), matrix_b_.end());
    return true;
  }

  bool MosaicHandlerImpl::Clear() {
	  mosaic_param_ = nullptr;    
	  return true;
  }

  void MosaicHandlerImpl::SwapYUVBlock(int index, VideoFramePtr video_frame) {
    if (matrix_a_.empty() || matrix_b_.empty()) {
      return;
    }
	  auto source_offset = GetBufferOffset(matrix_a_[index]);
	  auto target_offset = GetBufferOffset(matrix_b_[index]);
    for (auto i = 0U; i < mosaic_param_->per_block_pixel; ++i) {
      source_offset += (video_frame->line_size[0] * i);
      target_offset += (video_frame->line_size[0] * i);
		  std::swap_ranges(video_frame->data[0]->begin() + source_offset, video_frame->data[0]->begin() + source_offset + mosaic_param_->per_block_pixel, video_frame->data[0]->begin() + target_offset);
		  source_offset /= 4;
		  target_offset /= 4;
		  std::swap_ranges(video_frame->data[1]->begin() + source_offset, video_frame->data[1]->begin() + source_offset + mosaic_param_->per_block_pixel / 4, video_frame->data[1]->begin() + target_offset);
		  std::swap_ranges(video_frame->data[2]->begin() + source_offset, video_frame->data[2]->begin() + source_offset + mosaic_param_->per_block_pixel / 4, video_frame->data[2]->begin() + target_offset);
    }      
  }

  inline unsigned MosaicHandlerImpl::GetBufferOffset(const std::pair<int, int>& item) const {
	  const auto source_item_x = mosaic_param_->x + item.first * mosaic_param_->per_block_pixel;
	  const auto source_item_y = mosaic_param_->y + item.second * mosaic_param_->per_block_pixel;
	  return source_item_x * source_item_y;
  }
}
