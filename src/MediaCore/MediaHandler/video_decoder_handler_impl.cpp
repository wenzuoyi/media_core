#include "video_decoder_handler_impl.h"

namespace handler {
	VideoDecoderHandlerImpl::VideoDecoderHandlerImpl() = default;

	VideoDecoderHandlerImpl::~VideoDecoderHandlerImpl() = default;

  void VideoDecoderHandlerImpl::Start() {
	  current_decoder_type_ = VideoPackageType::kNone;
	  AsyncStart();
  }

  void VideoDecoderHandlerImpl::Stop() {
    AsyncStop();
    if (video_decoder_ != nullptr) {
      video_decoder_->Fini();
      video_decoder_ = nullptr;
    }
  }

  void VideoDecoderHandlerImpl::InputVideoPackage(VideoPackagePtr video_package) {
    if (video_package != nullptr) {
      Push(video_package);
    }
  }

  void VideoDecoderHandlerImpl::SetEvent(VideoDecoderHandlerEvent* event) {
    if (event != nullptr && event_ != event) {
      event_ = event;
    }
  }

  void VideoDecoderHandlerImpl::AsyncRun(VideoPackagePtr frame) {
    if (frame == nullptr || frame->base_info == nullptr || frame->data == nullptr) {
      return;
    }
    if (frame->base_info->type != current_decoder_type_) {
      if (video_decoder_ != nullptr) {
        video_decoder_->Fini();
      }
      video_decoder_ = nullptr;
    }
    if (video_decoder_ == nullptr) {
      video_decoder_ = VideoDecoder::CreateInstance();
      video_decoder_->Init(frame->base_info->type);
      current_decoder_type_ = frame->base_info->type;
    }
    auto decoding_frame = video_decoder_->Decode(frame);
    if (decoding_frame != nullptr) {
      for (auto item : *decoding_frame) {
        if (event_ != nullptr) {
          event_->OnTransmitVideoFrame(VideoHandlerType::kVideoDecoder, item);
        }
      }
    }
  }
}
