#include "direct_sound_player.h"
#include <cmath>
namespace output {
	const int DirectSoundPlayer::MAX_AUDIO_BUF = 4;

  bool DirectSoundPlayer::Init() {
    return SUCCEEDED(::DirectSoundCreate8(NULL, &direct_sound8_, NULL));
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

  bool DirectSoundPlayer::SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) {
	  BaseAudioPlayer::SetAudioOutputMediaParam(audio_output_param);
    if (direct_sound8_ == nullptr || audio_output_param == nullptr) {
		  return false;
    }	
	  if (FAILED(direct_sound8_->SetCooperativeLevel(audio_output_param->player_wnd, DSSCL_NORMAL))) {
		  return false;
	  }
	  FillSoundBufferParam(audio_output_param);
    return ConfigNotifyEvent();
  }

  void DirectSoundPlayer::FillSoundBufferParam(AudioOutputParamPtr audio_output_param) {	  
    memset(&dsbufferdesc_, 0, sizeof(DSBUFFERDESC));
    dsbufferdesc_.dwSize = sizeof(DSBUFFERDESC);
    dsbufferdesc_.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN;
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
    auto result{false};
    do {
    } while (FAILED(direct_sound8_->CreateSoundBuffer(&dsbufferdesc_, &direct_sound_buffer_, NULL)));
    do {
    } while (FAILED(direct_sound_buffer_->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID*>(&direct_sound_buffer8_))));
  }

  bool DirectSoundPlayer::ConfigNotifyEvent() {
    IDirectSoundNotify8* direct_sound_notify{nullptr};
    if (FAILED(direct_sound_buffer8_->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<LPVOID*>(&direct_sound_notify)))) {
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
          CopyBufferToHardwareBuffer(buffer, buffer_size);
          direct_sound_buffer8_->Unlock(buffer, buffer_size, nullptr, 0);
          offset += buffer_size;
          offset %= (dsbufferdesc_.dwBufferBytes / 4 * MAX_AUDIO_BUF);
        }
        result = WaitForMultipleObjects(MAX_AUDIO_BUF, &notify_event_list_[0], FALSE, INFINITE);
      }
    });
  }

  void DirectSoundPlayer::CopyBufferToHardwareBuffer(void* buffer, unsigned long buffer_size) {
	  memset(buffer, 0, buffer_size);
    std::lock_guard<std::mutex> lock_guard(mutex_);
    if (audio_playing_buffer_.size() > buffer_size) {
      memcpy(buffer, &audio_playing_buffer_[0], buffer_size);
      audio_playing_buffer_.erase(audio_playing_buffer_.begin(), audio_playing_buffer_.begin() + buffer_size);
    } else if (!audio_playing_buffer_.empty()) {
      memcpy(buffer, &audio_playing_buffer_[0], audio_playing_buffer_.size());
      audio_playing_buffer_.clear();
    }
  }

  void DirectSoundPlayer::Play() {
	  exit_ = false;
	  CreateAudioPlayedTask();
  }

  void DirectSoundPlayer::Stop() {
	  exit_ = true;
	  play_audio_task_.wait();
	  direct_sound_buffer8_->Stop();
  }

  bool DirectSoundPlayer::SetVolume(int volume) {
    unsigned long dsVol;
    if (volume == 0)
      dsVol = DSBVOLUME_MIN;
    else if (volume > 100)
      dsVol = DSBVOLUME_MAX;
    else {
      auto decibels = 20.0 * log10(static_cast<double>(volume) / 100.0f);
      dsVol = static_cast<DWORD>(decibels * 100.0);
    }
    return SUCCEEDED(direct_sound_buffer8_->SetVolume(dsVol));
  }

  int DirectSoundPlayer::GetVolume() const {
	  if (direct_sound_buffer8_ == nullptr) {
		  return 0;
	  }
	  LONG volume{ 0 };
    if (FAILED(direct_sound_buffer8_->GetVolume(&volume))) {
		  return 0;
    }
    auto var = static_cast<double>(volume) / 200.0f;
    auto result = 100.0 * std::pow(10.0f, var);
	  return static_cast<int>(result);
  }
}
