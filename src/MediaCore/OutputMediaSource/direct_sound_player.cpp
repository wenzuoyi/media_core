#include "direct_sound_player.h"
namespace output {
	const int DirectSoundPlayer::MAX_AUDIO_BUF = 4;
	const int DirectSoundPlayer::AUDIO_FREQUENCY_INTERVAL = 40;

  bool DirectSoundPlayer::Init() {
    if (FAILED(::DirectSoundCreate8(NULL, &direct_sound8_, NULL)))
      return false;
    return true;
  }

  void DirectSoundPlayer::Fini() {
    if (direct_sound_buffer8_ != nullptr) {
      direct_sound_buffer8_->Release();
      direct_sound_buffer8_ = nullptr;
    }
    if (direct_sound_buffer_ != nullptr) {
      direct_sound_buffer_->Release();
      direct_sound_buffer_ = nullptr;
    }
    if (dsbufferdesc_.lpwfxFormat != nullptr) {
      delete[] dsbufferdesc_.lpwfxFormat;
      dsbufferdesc_.lpwfxFormat = nullptr;
    }
    if (direct_sound8_ != nullptr) {
      direct_sound8_->Release();
      direct_sound8_ = nullptr;
    }
  }

  void DirectSoundPlayer::SetAudioOutputMediaSourceEvent(AudioOutputMediaSourceEvent* sink) {
    if (sink != nullptr && sink_ != sink) {
      sink_ = sink;
    }
  }

  bool DirectSoundPlayer::SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) {
    if (direct_sound8_ == nullptr || audio_output_param == nullptr) {
		  return false;
    }	
	  if (FAILED(direct_sound8_->SetCooperativeLevel(audio_output_param->player_wnd, DSSCL_NORMAL))) {
		  return false;
	  }
    if (!FillSoundBufferParam(audio_output_param)) {
		  return false;
    }
    if (!ConfigNotifyEvent()) {
		  return false;
    }
	  return true;
  }

  bool DirectSoundPlayer::FillSoundBufferParam(AudioOutputParamPtr audio_output_param) {
    memset(&dsbufferdesc_, 0, sizeof(DSBUFFERDESC));
    dsbufferdesc_.dwSize = sizeof(DSBUFFERDESC);
    dsbufferdesc_.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
    dsbufferdesc_.dwBufferBytes = audio_output_param->sample_rate * 2;
    dsbufferdesc_.lpwfxFormat = new WAVEFORMATEX();
    dsbufferdesc_.lpwfxFormat->wFormatTag = WAVE_FORMAT_PCM;
    (dsbufferdesc_.lpwfxFormat)->nChannels = audio_output_param->channels;
    (dsbufferdesc_.lpwfxFormat)->nSamplesPerSec = audio_output_param->sample_rate;
    (dsbufferdesc_.lpwfxFormat)->nAvgBytesPerSec = audio_output_param->sample_rate * (audio_output_param->bits_per_sample / 8) *
      audio_output_param->channels;
    (dsbufferdesc_.lpwfxFormat)->nBlockAlign = (audio_output_param->bits_per_sample / 8) * audio_output_param->channels;
    (dsbufferdesc_.lpwfxFormat)->wBitsPerSample = audio_output_param->bits_per_sample;
    (dsbufferdesc_.lpwfxFormat)->cbSize = 0;
    if (FAILED(direct_sound8_->CreateSoundBuffer(&dsbufferdesc_, &direct_sound_buffer_, NULL))) {
      return false;
    }
    if (FAILED(direct_sound_buffer_->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&direct_sound_buffer8_))) {
      return false;
    }
    return true;
  }

  bool DirectSoundPlayer::ConfigNotifyEvent() {
    IDirectSoundNotify8* direct_sound_notify{nullptr};
    if (FAILED(direct_sound_buffer8_->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&direct_sound_notify))) {
      return false;
    }
    for (auto i = 0; i < MAX_AUDIO_BUF; i++) {
      DSBPOSITIONNOTIFY item;
      item.hEventNotify = ::CreateEvent(nullptr, false, false, nullptr);
      item.dwOffset = i * (dsbufferdesc_.dwBufferBytes / 4);
      dsb_position_notify_list_.emplace_back(item);
      notify_event_list_.emplace_back(item.hEventNotify);
    }
    direct_sound_notify->SetNotificationPositions(MAX_AUDIO_BUF, &dsb_position_notify_list_[0]);
    direct_sound_notify->Release();
    return true;
  }

  void DirectSoundPlayer::CreateAudioPlayedTask() {
    play_audio_task_ = std::async(std::launch::async, [this]() {
      auto result = WAIT_OBJECT_0;
      LPVOID buffer = nullptr;
      DWORD buffer_size = 0;
      DWORD offset = 0;
      direct_sound_buffer8_->SetCurrentPosition(0);
      direct_sound_buffer8_->Play(0, 0, DSBPLAY_LOOPING);
      while (!exit_) {
        if ((result >= WAIT_OBJECT_0) && (result <= WAIT_OBJECT_0 + 3)) {
          direct_sound_buffer8_->Lock(offset, (dsbufferdesc_.dwBufferBytes / 4), &buffer, &buffer_size, nullptr, nullptr, 0);
          {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            if (played_audio_buffer_.size() > buffer_size) {
              memcpy(buffer, &played_audio_buffer_[0], buffer_size);
              played_audio_buffer_.erase(played_audio_buffer_.begin(), played_audio_buffer_.begin() + buffer_size);
            }
          }
          direct_sound_buffer8_->Unlock(buffer, buffer_size, nullptr, 0);
          offset += buffer_size;
          offset %= (dsbufferdesc_.dwBufferBytes / 4 * MAX_AUDIO_BUF);
        }
        result = WaitForMultipleObjects(MAX_AUDIO_BUF, &notify_event_list_[0], FALSE, INFINITE);
      }
    });
  }

  void DirectSoundPlayer::CreateFrequencyCollectTask() {
	  frequency_collect_task_ = std::async(std::launch::async, [this]() {
	    while(!exit_) {
        if (direct_sound_buffer8_ == nullptr) {
          unsigned long frequency{0};
          direct_sound_buffer8_->GetFrequency(&frequency);
          if (sink_ != nullptr) {
            sink_->OnSampleFrequency(frequency);
          }
		      std::this_thread::sleep_for(std::chrono::milliseconds(AUDIO_FREQUENCY_INTERVAL));
        }
	    }
	  });
  }


  void DirectSoundPlayer::Play() {
	  exit_ = false;
	  CreateAudioPlayedTask();
	  CreateFrequencyCollectTask();
  }

  void DirectSoundPlayer::Stop() {
	  exit_ = true;
	  play_audio_task_.wait();
	  frequency_collect_task_.wait();
	  direct_sound_buffer8_->Stop();
	  played_audio_buffer_.clear();
  }

  void DirectSoundPlayer::Mute() {
    if (is_mute_) {
      SetVolume(previous_volume_value_);
    } else {
      previous_volume_value_ = GetVolume();
      SetVolume(0);
    }
    is_mute_ = !is_mute_;
  }

  bool DirectSoundPlayer::IsMute() const {
	  return is_mute_;
  }

  bool DirectSoundPlayer::SetVolume(int volume) {
    if (volume < 0) {
      return false;
    }
    if (FAILED(direct_sound_buffer8_->SetVolume(volume))) {
      return false;
    }
	  return true;
  }

  int DirectSoundPlayer::GetVolume() const {
	  if (direct_sound_buffer8_ == nullptr) {
		  return 0;
	  }
	  LONG volume{ 0 };
    if (FAILED(direct_sound_buffer8_->GetVolume(&volume))) {
		  return 0;
    }
	  return volume;
  }

  void DirectSoundPlayer::InputAudioSample(AudioSamplePtr audio_sample) {
    if (audio_sample != nullptr) {
      while (played_audio_buffer_.size() > dsbufferdesc_.dwBufferBytes / 2) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
      std::lock_guard<std::mutex> lock_guard(mutex_);
      played_audio_buffer_.insert(played_audio_buffer_.end(), audio_sample->begin(), audio_sample->end());
    }
	  if (sink_ != nullptr) {
		  sink_->OnTransmitDataEvent(audio_sample);
	  }
  }
}
