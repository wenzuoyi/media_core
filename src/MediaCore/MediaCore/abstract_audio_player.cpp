#include "abstract_audio_player.h"

namespace core {
	AbstractAudioPlayer::AbstractAudioPlayer() = default;

	AbstractAudioPlayer::~AbstractAudioPlayer() = default;

  bool AbstractAudioPlayer::Initialize(BasicPlayerParamPtr basic_player_info, AudioBaseInfoPtr audio_base_info) {
    if (basic_player_info == nullptr || audio_base_info == nullptr) {
      return false;
    }
    volume_column_handler_ = handler::VolumeColumnHandler::CreateInstance(audio_base_info->bits_per_sample);
    if (volume_column_handler_ == nullptr) {
      return false;
    }
    volume_column_handler_->SetAudioSampleChannel(audio_base_info->channels);
    volume_column_handler_->SetEvent(this);
    volume_column_handler_->Start();
    audio_output_media_source_ = output::AudioOutputMediaSource::CreateInstance(basic_player_info->audio_render_mode);
    if (audio_output_media_source_ == nullptr) {
      return false;
    }
    audio_output_media_source_->SetEvent(this);
    auto param = std::make_shared<AudioOutputParam>();
    param->bits_per_sample = audio_base_info->bits_per_sample;
    param->channels = audio_base_info->channels;
    param->sample_rate = audio_base_info->sample_rate;
    param->player_wnd = basic_player_info->hwnd;
    if (!audio_output_media_source_->SetParam(param)) {
      return false;
    }
    audio_output_media_source_->Play();
    return true;
  }

  void AbstractAudioPlayer::UnInitialize() {
    if (audio_output_media_source_ != nullptr) {
      audio_output_media_source_->Stop();
      audio_output_media_source_->Fini();
      audio_output_media_source_ = nullptr;
    }
    if (volume_column_handler_ != nullptr) {
      volume_column_handler_->Stop();
      volume_column_handler_ = nullptr;
    }
  }

  void AbstractAudioPlayer::OnAudioOutputException(unsigned error_code) {
  }

  void AbstractAudioPlayer::OnAudioTransmitSample(AudioSamplePtr audio_sample) {
    if (volume_column_handler_ != nullptr) {
      volume_column_handler_->InputAudioSample(audio_sample);
    }
  }

  void AbstractAudioPlayer::OnAudioSampleFrequency(unsigned long frequency) {
  }

  void AbstractAudioPlayer::OnTransmitAudioSample(handler::AudioHandlerType audio_event, AudioSamplePtr audio_sample) {
  }

  void AbstractAudioPlayer::OnSampleVolumeRatio(std::vector<double>&& ratio_array) {
  }
}
