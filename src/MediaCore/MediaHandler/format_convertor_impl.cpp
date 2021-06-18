#include "format_convertor_impl.h"

extern "C" {
  #include "libswscale/swscale.h"
  #include "libavutil/frame.h"
  #include "libavutil/imgutils.h"
}

namespace handler {
  FormatConvertorImpl::FormatConvertorImpl() = default;

  FormatConvertorImpl::~FormatConvertorImpl() = default;

  void FormatConvertorImpl::Start() {
	  AsyncStart();
  }

  void FormatConvertorImpl::Stop() {
	  AsyncStop();
    if (frame_ != nullptr) {
      ::av_frame_free(&frame_);
      frame_ = nullptr;
    }
    if (image_convert_context_ != nullptr) {
      ::sws_freeContext(image_convert_context_);
    }
  }

  void FormatConvertorImpl::InputVideoFrame(VideoFramePtr video_frame) {
	  Push(video_frame);
  }

  void FormatConvertorImpl::SetEvent(FormatConvertorEvent* event) {
    if (event != event_) {
      event_ = event;
    }
  }

  void FormatConvertorImpl::SetBasicFormatInfo(ConvertorBasicParamPtr param) {
    if (param == nullptr) {
      return;
    }
    Stop();
    auto source_format = ColorSpace2PixelFormat(param->source_format);
    auto target_format = ColorSpace2PixelFormat(param->target_format);
    image_convert_context_ = sws_getContext(param->source_width, param->source_height, source_format,
                                            param->target_width, param->target_height, target_format, SWS_BICUBIC,
                                            nullptr, nullptr, nullptr);
    frame_ = av_frame_alloc();
    auto* buffer = static_cast<uint8_t*>(av_malloc(av_image_get_buffer_size(target_format, param->target_width, param->target_height, 1)));
    av_image_fill_arrays(frame_->data, frame_->linesize, buffer, target_format, param->target_width, param->target_height, 1);
  }

  void FormatConvertorImpl::AsyncRun(std::shared_ptr<output::VideoFrame> video_frame) {
	  if (video_frame == nullptr || image_convert_context_ == nullptr) {
		  return;
	  }
	  uint8_t* yuv[3] = { nullptr };
	  yuv[0] = &(*(video_frame->data[0]))[0];
	  yuv[1] = &(*(video_frame->data[1]))[0];
	  yuv[2] = &(*(video_frame->data[2]))[0];
	  int yuv_stride[3] = { 0 };
	  yuv_stride[0] = video_frame->line_size[0];
	  yuv_stride[1] = video_frame->line_size[1];
	  yuv_stride[2] = video_frame->line_size[2];
	  ::sws_scale(image_convert_context_, yuv, yuv_stride, video_frame->width, video_frame->height, frame_->data, frame_->linesize);
	  auto frame = std::make_shared<output::VideoFrame>();
	  frame->data[0] = std::make_shared<output::Buffer>(frame_->data[0], frame_->data[0] + frame_->linesize[0]);
	  frame->data[1] = std::make_shared<output::Buffer>(frame_->data[1], frame_->data[1] + frame_->linesize[1]);
	  frame->data[2] = std::make_shared<output::Buffer>(frame_->data[2], frame_->data[2] + frame_->linesize[2]);
	  frame->line_size[0] = frame_->linesize[0];
	  frame->line_size[1] = frame_->linesize[1];
	  frame->line_size[2] = frame_->linesize[2];
	  frame->width = frame_->width;
	  frame->height = frame_->height;
	  if (event_ != nullptr) {
		  event_->OnTransmitVideoFrame(VideoHandlerType::kFormatConvert, frame);
	  }
  }

  inline AVPixelFormat FormatConvertorImpl::ColorSpace2PixelFormat(output::ColorSpace color_space) {
    if (color_space == output::ColorSpace::kBGRA) {
      return AVPixelFormat::AV_PIX_FMT_BGRA;
    }
    //if (color_space == output::ColorSpace::kYUV420P) {
    //  return AVPixelFormat::AV_PIX_FMT_YUV420P;
    //}
    return AVPixelFormat::AV_PIX_FMT_YUV420P;
  }
}
