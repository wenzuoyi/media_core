#include "base_audio_player.h"
namespace output {
	const int BaseAudioPlayer::AUDIO_FREQUENCY_INTERVAL = 40;

  void BaseAudioPlayer::SetAudioOutputMediaSourceEvent(AudioOutputMediaSourceEvent* sink) {
	  if (sink_ != sink) {
		  sink_ = sink;
	  }
  }

  bool BaseAudioPlayer::SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) {
    max_cache_buffer_size_ = audio_output_param->sample_rate * audio_output_param->channels * (audio_output_param->bits_per_sample / 8);
	  return true;
  }

  void BaseAudioPlayer::Mute() {
	  if (is_mute_) {
		  SetVolume(previous_volume_value_);
	  } else {
		  previous_volume_value_ = GetVolume();
		  SetVolume(0);
	  }
	  is_mute_ = !is_mute_;
  }

  bool BaseAudioPlayer::IsMute() const {
	  return is_mute_;
  }

  bool BaseAudioPlayer::InputAudioSample(AudioSamplePtr audio_sample) {
    if (audio_sample == nullptr) {
      return false;
    }
    {
      std::lock_guard<std::mutex> lock_guard(mutex_);
      if (audio_playing_buffer_.size() > max_cache_buffer_size_ / 2) {
        return false;
      }
      audio_playing_buffer_.insert(audio_playing_buffer_.end(), audio_sample->begin(), audio_sample->end());
    }
    if (sink_ != nullptr) {
      sink_->OnTransmitDataEvent(audio_sample);
    }
    return true;
  }
}
