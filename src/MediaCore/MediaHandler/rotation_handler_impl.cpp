#include "rotation_handler_impl.h"
#include "libyuv.h"
namespace handler {    
	RotationHandlerImpl::RotationHandlerImpl() = default;

	RotationHandlerImpl::~RotationHandlerImpl() = default;

  void RotationHandlerImpl::Start() {
	  AsyncStart();
  }

  void RotationHandlerImpl::Stop() {
	  AsyncStop();
  }

  VideoFramePtr RotationHandlerImpl::RotateVideoFrame(VideoFramePtr video_frame) const {
    VideoFramePtr frame{nullptr};
    if (rotation_degree_type_ == RotationDegreeType::kDegree0) {
      frame = RotateViaLibYUV(video_frame, true, libyuv::kRotate0);
    } else if (rotation_degree_type_ == RotationDegreeType::kDegree90) {
      frame = RotateViaLibYUV(video_frame, false, libyuv::kRotate90);
    } else if (rotation_degree_type_ == RotationDegreeType::kDegree180) {
      frame = RotateViaLibYUV(video_frame, true, libyuv::kRotate180);
    } else if (rotation_degree_type_ == RotationDegreeType::kDegree270) {
      frame = RotateViaLibYUV(video_frame, false, libyuv::kRotate270);
    }
    return frame;
  }

  void RotationHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
	  Push(video_frame);
  }
  
  VideoFramePtr RotationHandlerImpl::RotateViaLibYUV(VideoFramePtr source, bool flag, libyuv::RotationMode mode) const {
	  auto target = std::make_shared<output::VideoFrame>();
	  target->width = (flag ? source->width : source->height);
	  target->height = (flag ? source->height : source->width);
	  target->line_size[0] = target->width;
	  target->data[0] = std::make_shared<output::Buffer>(source->data[0]->size(), 0);
	  target->line_size[1] = target->width / 2;
	  target->data[1] = std::make_shared<output::Buffer>(source->data[1]->size(), 0);
	  target->line_size[2] = target->width / 2;
	  target->data[2] = std::make_shared<output::Buffer>(source->data[2]->size(), 0);
	  auto* const target_y = &(*(target->data[0]))[0];
	  auto target_y_linesize = target->line_size[0];
	  auto* const target_u = &(*(target->data[1]))[0];
	  auto target_u_linesize = target->line_size[1];
	  auto* const target_v = &(*(target->data[2]))[0];
	  auto target_v_linesize = target->line_size[2];
	  auto* const source_y = &(*(source->data[0]))[0];
	  auto source_y_linesize = source->line_size[0];
	  auto* const source_u = &(*(source->data[1]))[0];
	  auto source_u_linesize = source->line_size[1];
	  auto* const source_v = &(*(source->data[2]))[0];
	  auto source_v_linesize = source->line_size[2];
	  libyuv::I420Rotate(source_y, source_y_linesize, source_u, source_u_linesize, source_v, source_v_linesize, target_y,
		  target_y_linesize, target_u, target_u_linesize, target_v, target_v_linesize, source->width,
		  source->height, mode);
	  return target;
  }

  VideoFramePtr RotationHandlerImpl::Clone(VideoFramePtr source) {
	  auto target = std::make_shared<output::VideoFrame>();
	  target->height = source->height;
	  target->width = source->width;
    for (auto i = 0U; i < 3; ++i) {
      target->line_size[i] = source->line_size[i];
	    target->data[i] = std::make_shared<output::Buffer>(source->data[i]->begin(), source->data[i]->end());
    }
    return target;
  }

  void RotationHandlerImpl::SetEvent(RotationHandlerEvent* event) {
    if (event != event_) {
      event_ = event;
    }
  }

  void RotationHandlerImpl::EnableRotation(bool enable) {
    if (enable != enable_) {
      enable_ = enable;
      rotation_degree_type_ = RotationDegreeType::kDegreeUnknown;
    }
  }

  void RotationHandlerImpl::Rotate(RotationDegreeType rotation_degree_type) {
    if (rotation_degree_type != rotation_degree_type_) {
      rotation_degree_type_ = rotation_degree_type;
    }
  }

  bool RotationHandlerImpl::GetEnableRotation() const {
	  return enable_;
  }

  RotationDegreeType RotationHandlerImpl::GetRotateType() {
	  return rotation_degree_type_;
  }

  void RotationHandlerImpl::AsyncRun(VideoFramePtr video_frame) {
    if (video_frame == nullptr) {
      return;
    }
	  VideoFramePtr target{ nullptr };
	  if (enable_) {
		  target = RotateVideoFrame(video_frame);
	  }
	  if (target == nullptr) {
		  target = video_frame;
	  }
	  if (event_ != nullptr) {
		  event_->OnTransmitVideoFrame(VideoHandlerType::kRotate, target);
	  }
  }
}

