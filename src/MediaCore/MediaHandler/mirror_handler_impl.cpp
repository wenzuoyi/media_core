#include "mirror_handler_impl.h"
#include <algorithm>
#include <libyuv.h>

namespace handler {
	MirrorHandlerImpl::MirrorHandlerImpl() = default;

	MirrorHandlerImpl::~MirrorHandlerImpl() = default;

  void MirrorHandlerImpl::Start() {
    {
		  Poco::ScopedWriteRWLock lock(read_write_lock_);
      enable_ = false;
    }
    AsyncStart();
  }

  void MirrorHandlerImpl::Stop() {
	  AsyncStop();
	  Poco::ScopedWriteRWLock lock(read_write_lock_);
	  enable_ = false;
  }

  void MirrorHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
	  Push(video_frame);
  }

  void MirrorHandlerImpl::SetEvent(MirrorHandlerEvent* event) {
    if (event != nullptr && event != event_) {
      event_ = event;
    }
  }

  void MirrorHandlerImpl::EnableMirror(bool enable) {
	  Poco::ScopedWriteRWLock lock(read_write_lock_);
    if (enable_ != enable) {
      enable_ = enable;
    }
  }

  bool MirrorHandlerImpl::IsEnableMirror() const {
	  Poco::ScopedReadRWLock lock(read_write_lock_);
	  return enable_;
  }

  void MirrorHandlerImpl::AsyncRun(std::shared_ptr<output::VideoFrame> video_frame) {
	  {
		  Poco::ScopedReadRWLock lock(read_write_lock_);
		  if (video_frame != nullptr && enable_) {
			  video_frame = HandleVideoFrame(video_frame);
		  }
	  }
	  if (event_ != nullptr) {
		  event_->OnTransmitVideoFrame(VideoHandlerType::kMirror, video_frame);
	  }
  }

  VideoFramePtr MirrorHandlerImpl::HandleVideoFrame(VideoFramePtr source) {
	  auto target = std::make_shared<output::VideoFrame>();
	  target->width = source->width;
	  target->height = source->height;
	  target->line_size[0] = source->line_size[0];
	  target->data[0] = std::make_shared<output::Buffer>(source->data[0]->size(), 0);
	  target->line_size[1] = source->line_size[1];
	  target->data[1] = std::make_shared<output::Buffer>(source->data[1]->size(), 0);
	  target->line_size[2] = source->line_size[2];
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
	  libyuv::I420Mirror(source_y, source_y_linesize, source_u, source_u_linesize, source_v, source_v_linesize,
		  target_y, target_y_linesize, target_u, target_u_linesize, target_v, target_v_linesize, target->width, target->height);
	  return target;
  }
}
