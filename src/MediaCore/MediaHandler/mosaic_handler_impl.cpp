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
    {
      std::lock_guard<std::mutex> lock(*this);
      enable_ = false;
      mosaic_param_ = nullptr;
    }
	  AsyncStart();
  }

  void MosaicHandlerImpl::Stop() {
    AsyncStop();
    std::lock_guard<std::mutex> lock(*this);
    enable_ = false;
    mosaic_param_ = nullptr;
  }

  void MosaicHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
	  Push(video_frame);
  }

  void MosaicHandlerImpl::EnableMosaic(bool enable) {
    std::lock_guard<std::mutex> lock(*this);
    if (enable_ != enable) {
      enable_ = enable;
    }
  }

  bool MosaicHandlerImpl::IsSettingMosaic() const {
	  return enable_;
  }

  bool MosaicHandlerImpl::SetParam(MosaicParamPtr param) {
    if (param == nullptr) {
      return false;
    }
	  std::lock_guard<std::mutex> lock(*this);
    mosaic_param_ = param;
    matrix_a_.clear();
    matrix_b_.clear();
	  const auto horizontal_count = mosaic_param_->width / mosaic_param_->per_block_pixel;
	  const auto vertical_count = mosaic_param_->height / mosaic_param_->per_block_pixel;
	  for (auto u = 0; u < horizontal_count; ++u) {
      for (auto v = 0; v < vertical_count; ++v) {
        if (matrix_a_.size() < static_cast<unsigned>(horizontal_count * vertical_count / 2)) {
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
	  std::lock_guard<std::mutex> lock(*this);
    mosaic_param_ = nullptr;    
	  return true;
  }

  void MosaicHandlerImpl::AsyncRun(std::shared_ptr<output::VideoFrame> video_frame) {
	  {
		  std::lock_guard<std::mutex> lock(*this);
		  if (enable_ && video_frame != nullptr && mosaic_param_ != nullptr) {
			  if (mosaic_param_->x + mosaic_param_->width <= video_frame->width && mosaic_param_->y + mosaic_param_->height <= video_frame->height) {
				  for (auto i = 0U; i < matrix_a_.size(); ++i) {
					  SwapYUVBlock(i, video_frame);
				  }
			  }
		  }
	  }
	  event_->OnTransmitVideoFrame(VideoHandlerType::kMosaic, video_frame);
  }

  void MosaicHandlerImpl::SwapYUVBlock(int index, VideoFramePtr video_frame) {
    if (matrix_a_.empty() || matrix_b_.empty()) {
      return;
    }
	  auto source_offset = GetBufferOffset(matrix_a_[index], video_frame->line_size[0]);
	  auto target_offset = GetBufferOffset(matrix_b_[index], video_frame->line_size[0]);
    for (auto i = 0; i < mosaic_param_->per_block_pixel; ++i) {
		  std::swap_ranges(video_frame->data[0]->begin() + source_offset, video_frame->data[0]->begin() + source_offset + mosaic_param_->per_block_pixel, video_frame->data[0]->begin() + target_offset);
		  const auto temp_source_offset = source_offset / 4;
		  const auto temp_target_offset = target_offset / 4;
		  std::swap_ranges(video_frame->data[1]->begin() + temp_source_offset, video_frame->data[1]->begin() + temp_source_offset + mosaic_param_->per_block_pixel / 4, video_frame->data[1]->begin() + temp_target_offset);
		  std::swap_ranges(video_frame->data[2]->begin() + temp_source_offset, video_frame->data[2]->begin() + temp_source_offset + mosaic_param_->per_block_pixel / 4, video_frame->data[2]->begin() + temp_target_offset);
		  source_offset += video_frame->line_size[0];
		  target_offset += video_frame->line_size[0];
    }      
  }

  inline unsigned MosaicHandlerImpl::GetBufferOffset(const std::pair<int, int>& item, int stride) const {
	  const auto source_item_x = mosaic_param_->x + item.first * mosaic_param_->per_block_pixel;
	  const auto source_item_y = mosaic_param_->y + item.second * mosaic_param_->per_block_pixel;
	  return stride* source_item_y + source_item_x ;
  }
}
