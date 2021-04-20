#include "abstract_player_object.h"

namespace core {
  AbstractPlayerObject::AbstractPlayerObject() = default;

  AbstractPlayerObject::~AbstractPlayerObject() = default;

  bool AbstractPlayerObject::Initialize(BasicPlayerParamPtr basic_player_info, AudioBaseInfoPtr audio_base_info) {
    if (basic_player_info == nullptr || audio_base_info == nullptr) {
      return false;
    }
    audio_output_ = output::AudioOutputMediaSource::CreateInstance(basic_player_info->audio_render_mode);
    if (audio_output_ == nullptr) {
      return false;
    }
    audio_output_->SetEvent(this);
    auto param = std::make_shared<AudioOutputParam>();
    param->bits_per_sample = audio_base_info->bits_per_sample;
    param->channels = audio_base_info->channels;
    param->sample_rate = audio_base_info->sample_rate;
    param->player_wnd = basic_player_info->hwnd;
    if (!audio_output_->SetParam(param)) {
      return false;
    }
    audio_output_->Play();
    return true;
  }

  bool AbstractPlayerObject::Initialize(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info) {
    if (basic_player_info == nullptr || video_base_info == nullptr) {
      return false;
    }
    mosaic_handler_ = handler::MosaicHandler::CreateInstance();
    if (mosaic_handler_ == nullptr) {
      return false;
    }
    mosaic_handler_->SetEvent(this);
    mosaic_handler_->Start();
    if (!mosaic_handler_->Clear()) {
      return false;
    }
    video_output_ = output::VideoOutputMediaSource::CreateInstance(basic_player_info->video_render_mode);
    video_output_->Init();
    video_output_->SetEvent(this);
    auto param = std::make_shared<output::VideoOutputParam>();
    param->width = video_base_info->width;
    param->height = video_base_info->height;
    param->render_wnd = basic_player_info->hwnd;
    video_output_->SetVideoOutputMediaParam(param);
    return video_output_->Play();
  }


  void AbstractPlayerObject::UnInitialize(MediaModelType type) {
    if (type == MediaModelType::kAll || type == MediaModelType::kAudio) {
      if (audio_output_ != nullptr) {
        audio_output_->Stop();
        audio_output_ = nullptr;
      }
    }
    if (type == MediaModelType::kAll || type == MediaModelType::kVideo) {
      if (video_output_ != nullptr) {
        video_output_->Stop();
        video_output_->Fini();
        video_output_ = nullptr;
      }
      if (mosaic_handler_ != nullptr) {
        mosaic_handler_->Stop();
        mosaic_handler_ = nullptr;
      }
    }
  }

  void AbstractPlayerObject::OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type,  VideoFramePtr video_frame) {
    if (video_frame != nullptr && video_output_ != nullptr) {
      video_output_->InputVideoFrame(video_frame);
    }
  }

  void AbstractPlayerObject::OnAudioOutputException(unsigned error_code) {
  }

  void AbstractPlayerObject::OnAudioTransmitSample(AudioSamplePtr audio_sample) {
  }

  void AbstractPlayerObject::OnAudioSampleFrequency(unsigned long frequency) {
  }

  void AbstractPlayerObject::OnVideoOutputException(unsigned error_code) {
  }

  void AbstractPlayerObject::OnVideoCustomPainting(HDC hdc) {
  }

  void AbstractPlayerObject::OnVideoTransmitFrame(VideoFramePtr video_frame) {
  }
}
