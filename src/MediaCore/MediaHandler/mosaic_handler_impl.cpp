#include "mosaic_handler_impl.h"
#include <algorithm>
namespace handler {
	MosaicHandlerImpl::MosaicHandlerImpl() = default;

	MosaicHandlerImpl::~MosaicHandlerImpl() = default;

  void MosaicHandlerImpl::SetEvent(void* event) {
    if (event != event_) {
      event_ = static_cast<MosaicHandlerEvent*>(event);
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
    if (video_frame != nullptr) {
        for (auto i = 0U;  i < matrix_a_.size(); ++i) {
          SwapYUVBlock(i, video_frame);
        }
    }
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
    block_width_ = mosaic_param_->width / mosaic_param_->block_count;
    block_height_ = mosaic_param_->height / mosaic_param_->block_count;
    const auto half_block_count = static_cast<unsigned>(mosaic_param_->block_count * mosaic_param_->block_count / 2);
    for (auto u = 0; u < mosaic_param_->block_count; ++u) {
      for (auto v = 0; v < mosaic_param_->block_count; ++v) {
        if (matrix_a_.size() < half_block_count) {
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
	  auto source_offset = GetBufferOffset(matrix_a_[index]);
	  auto target_offset = GetBufferOffset(matrix_b_[index]);
    for (auto i = 0U; i < block_height_; ++i) {
      source_offset += (video_frame->line_size[0] * block_height_);
      target_offset += (video_frame->line_size[0] * block_height_);
		  std::swap_ranges(video_frame->data[0]->begin() + source_offset, video_frame->data[0]->begin() + source_offset + block_width_, video_frame->data[0]->begin() + target_offset);
    }      
  }

  inline unsigned MosaicHandlerImpl::GetBufferOffset(const std::pair<int, int>& item) const {
	  const auto source_item_x = mosaic_param_->x + item.first * block_width_;
	  const auto source_item_y = mosaic_param_->y + item.second * block_height_;
	  return source_item_x * source_item_y;
  }
}
