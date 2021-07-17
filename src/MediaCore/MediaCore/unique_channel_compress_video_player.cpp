#include "unique_channel_compress_video_player.h"

namespace core {
	UniqueChannelCompressVideoPlayer::UniqueChannelCompressVideoPlayer() = default;

	UniqueChannelCompressVideoPlayer::~UniqueChannelCompressVideoPlayer() = default;

  bool UniqueChannelCompressVideoPlayer::Init(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) {
    if (basic_player_info == nullptr || video_base_info == nullptr) {
      return false;
    }
    video_decoder_handler_ = handler::VideoDecoderHandler::CreateInstance();
    if (video_decoder_handler_ == nullptr) {
      return false;
    }
    video_decoder_handler_->SetEvent(this);
    video_decoder_handler_->Start();
    return UniqueChannelBaseVideoPlayer::Init(basic_player_info, video_base_info);
  }

  void UniqueChannelCompressVideoPlayer::Fini() {
    UniqueChannelBaseVideoPlayer::Fini();
    if (video_decoder_handler_ != nullptr) {
      video_decoder_handler_->Stop();
      video_decoder_handler_ = nullptr;
    }
  }

  void UniqueChannelCompressVideoPlayer::InputVideoPackage(VideoPackagePtr video_package) {
    if (video_decoder_handler_ != nullptr) {
      video_decoder_handler_->InputVideoPackage(video_package);
    }
  }

  void UniqueChannelCompressVideoPlayer::OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type,  VideoFramePtr video_frame) {
	  if (video_frame == nullptr) {
		  return;
	  }
    if (video_handler_type == handler::VideoHandlerType::kVideoDecoder && mosaic_handler_ != nullptr) {
      mosaic_handler_->InputVideoFrame(video_frame);
    }
	  UniqueChannelBaseVideoPlayer::OnTransmitVideoFrame(video_handler_type, video_frame);
  }

  void UniqueChannelCompressVideoPlayer::OnHandlerException(handler::VideoHandlerType video_handler_type,  const std::string& message, int code) {
	  UniqueChannelBaseVideoPlayer::OnHandlerException(video_handler_type, message, code);
  }
}
