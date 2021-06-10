#include "rotation_handler_impl.h"
#include <math.h>
#include <algorithm>
#include <sstream>
namespace handler {
	const double RotationHandlerImpl::PI = 3.1415926535;

	RotationHandlerImpl::RotationHandlerImpl() = default;

	RotationHandlerImpl::~RotationHandlerImpl() = default;

  void RotationHandlerImpl::Start() {
  }

  void RotationHandlerImpl::Stop() {
  }

  VideoFramePtr RotationHandlerImpl::RotateVideoFrame(VideoFramePtr video_frame) const {
    VideoFramePtr frame{nullptr};
    if (!use_integer_) {
      if (rotation_degree_type_ == RotationDegreeType::kDegree90) {
        frame = Rotate90(video_frame);
      } else if (rotation_degree_type_ == RotationDegreeType::kDegree180) {
        frame = Rotate180(video_frame);
      } else if (rotation_degree_type_ == RotationDegreeType::kDegree270) {
        frame = Rotate270(video_frame);
      }
    } else {
      frame = RotateViaIntegerValue(video_frame);
    }
    return frame;
  }

  void RotationHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
    if (video_frame == nullptr) {
      return;
    }
    VideoFramePtr target{nullptr};
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

  VideoFramePtr RotationHandlerImpl::Rotate90(VideoFramePtr source) {
	  auto old = ::GetTickCount64();
	  auto target = std::make_shared<output::VideoFrame>();
	  target->width = source->height;
	  target->height = source->width;
	  target->line_size[0] = target->width;
	  target->data[0] = std::make_shared<output::Buffer>(source->data[0]->size(), 0);
	  target->line_size[1] = target->width / 2;
	  target->data[1] = std::make_shared<output::Buffer>(source->data[1]->size(), 0);
	  target->line_size[2] = target->width / 2;
	  target->data[2] = std::make_shared<output::Buffer>(source->data[2]->size(), 0);
    for (auto i = 0; i < source->height; ++i) {
      for (auto j = 0; j < source->width; ++j) {
        auto* target_y_field = &(*(target->data[0]))[0];
        auto* source_y_field = &(*(source->data[0]))[0];
        *(target_y_field + (j * target->line_size[0] + i)) = *(source_y_field + (i * source->line_size[0] + j));
        if (i < source->height / 2 && j < source->width / 2) {
          auto* target_u_field = &(*(target->data[1]))[0];
          auto* source_u_field = &(*(source->data[1]))[0];
		      *(target_u_field + (j * target->line_size[1] + i)) = *(source_u_field + (i * source->line_size[1] + j));
          auto* target_v_field = &(*(target->data[2]))[0];
          auto* source_v_field = &(*(source->data[2]))[0];
		      *(target_v_field + (j * target->line_size[2] + i)) = *(source_v_field + (i * source->line_size[2] + j));
        }
      }
	  }
	auto interval = ::GetTickCount64() - old;
	std::ostringstream oss;
	oss << "\r\n: interval = " << interval << "\r\n";
	::OutputDebugStringA(oss.str().c_str());
    
    return target;
  }

  VideoFramePtr RotationHandlerImpl::Rotate180(VideoFramePtr source) {
    for (auto i = 0; i < source->height / 2; ++i) {
      const auto y_source_offset = i * source->line_size[0];
      const auto y_target_offset = (source->height - i - 1) * source->line_size[0];
		  std::swap_ranges(source->data[0]->begin() + y_source_offset, source->data[0]->begin() + y_source_offset + source->line_size[0],  source->data[0]->begin() + y_target_offset);
      if (i < source->height / 4) {
        const auto uv_source_offset = i * source->line_size[1];
		    const auto uv_target_offset = (source->height / 2 - i - 1) * source->line_size[1];
		    std::swap_ranges(source->data[1]->begin() + uv_source_offset, source->data[1]->begin() + uv_source_offset + source->line_size[1], source->data[1]->begin() + uv_target_offset);
			  std::swap_ranges(source->data[2]->begin() + uv_source_offset, source->data[2]->begin() + uv_source_offset + source->line_size[2], source->data[2]->begin() + uv_target_offset);
      }
    }
    return source;
  }

  VideoFramePtr RotationHandlerImpl::Rotate270(VideoFramePtr source) {
	  auto target = Rotate90(source);
	  for (auto i = 0; i < target->height; ++i) {
		  const auto y_offset = i * target->line_size[0];
		  std::reverse(target->data[0]->begin() + y_offset, target->data[0]->begin() + y_offset + target->line_size[0]);
	    if (i < target->height / 2) {
			  const auto uv_offset = i * target->line_size[1];
			  std::reverse(target->data[1]->begin() + uv_offset, target->data[1]->begin() + uv_offset + target->line_size[1]);
			  std::reverse(target->data[2]->begin() + uv_offset, target->data[2]->begin() + uv_offset + target->line_size[2]);
		  }
	  }
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

  VideoFramePtr RotationHandlerImpl::RotateViaIntegerValue(VideoFramePtr source) const {
	  auto target = Clone(source);
	  const auto center_x = target->width / 2;
	  const auto center_y = target->height / 2;
	  target->data[0]->assign(target->data[0]->size(), 0);
	  target->data[1]->assign(target->data[1]->size(), 0x80);
	  target->data[2]->assign(target->data[2]->size(), 0x80);
	  auto* target_y = &((*(target->data[0]))[0]);
	  auto* target_u = &((*(target->data[1]))[0]);
	  auto* target_v = &((*(target->data[2]))[0]);
	  auto* source_y = &((*(source->data[0]))[0]);
	  auto* source_v = &((*(source->data[1]))[0]);
	  auto* source_u = &((*(source->data[2]))[0]);
	  for (auto h = 0; h < source->height; ++h) {
		  for (auto w = 0; w < source->width; ++w) {
			  auto nx = w - center_x;
			  if (nx > source->height / 2 || nx < -source->height / 2) {
				  continue;
			  }
        auto ny = center_y - h;
			  auto ox = nx * cos_alpha_ - ny * sin_alpha_;
			  auto oy = nx * sin_alpha_ + ny * cos_alpha_;
			  ox = ox + center_x;
			  oy = center_y - oy;
			  if (ox < 0 || oy < 0) {
				  continue;
			  }
			  if (ox >= source->width || oy >= source->height) {
				  continue;
			  }
        *(target_y + h * target->width + w) = *(source_y + static_cast<int>(oy * (source->width) + ox));
        *(target_u + (h / 2) * (target->width / 2) + w / 2) = *(source_u + static_cast<int>((oy / 2) * (source->width / 2) + ox / 2));
        *(target_v + (h / 2) * (target->width / 2) + w / 2) = *(source_v + static_cast<int>((oy / 2) * (source->width / 2) + ox / 2));
		  }
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
      rotation_degree_ = 0;
    }
  }

  void RotationHandlerImpl::Rotate(RotationDegreeType rotation_degree_type) {
    if (rotation_degree_type != rotation_degree_type_) {
      rotation_degree_type_ = rotation_degree_type;
      ConvertEnumTypeToInteger(rotation_degree_type_, &rotation_degree_);
    }
  }

  void RotationHandlerImpl::Rotate(int degree) {
    if (rotation_degree_ != degree) {
      rotation_degree_ = degree;
      ConvertIntegerToEnumType(degree, &rotation_degree_type_);
    }
    if (use_integer_) {
      const auto alpha = (degree % 360) * PI / 180.0f;
      sin_alpha_ = sin(alpha);
      cos_alpha_ = cos(alpha);
    }
  }

  inline void RotationHandlerImpl::ConvertEnumTypeToInteger(RotationDegreeType rotation_degree_type, int* degree) {
	  if (rotation_degree_type == RotationDegreeType::kDegreeUnknown) {
		  *degree = 0;
	  } else if (rotation_degree_type == RotationDegreeType::kDegree90) {
		  *degree = 90;
	  } else if (rotation_degree_type == RotationDegreeType::kDegree180) {
		  *degree = 180;
	  } else if (rotation_degree_type == RotationDegreeType::kDegree270) {
		  *degree = 270;
	  }
	  use_integer_ = false;
  }

  inline void RotationHandlerImpl::ConvertIntegerToEnumType(int degree, RotationDegreeType* rotation_degree_type) {
	  use_integer_ = false;
	  if (degree == 0) {
		  *rotation_degree_type = RotationDegreeType::kDegreeUnknown;
	  } else if (degree == 90) {
		  *rotation_degree_type = RotationDegreeType::kDegree90;
	  } else if (degree == 180) {
		  *rotation_degree_type = RotationDegreeType::kDegree180;
	  } else if (degree == 270) {
		  *rotation_degree_type = RotationDegreeType::kDegree270;
	  } else {
		  use_integer_ = true;
	  }
  }
}

