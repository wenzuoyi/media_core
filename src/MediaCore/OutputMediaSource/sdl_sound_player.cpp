#include "sdl_sound_player.h"
#include "sdl/SDL.h"
namespace output {
  bool SDLSoundPlayer::Init() {
    volume_ = SDL_MIX_MAXVOLUME;
    return ::SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0;
  }

  void SDLSoundPlayer::Fini() {
    ::SDL_CloseAudio();
    ::SDL_Quit();
  }
  
  bool SDLSoundPlayer::SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) {
	  BaseAudioPlayer::SetAudioOutputMediaParam(audio_output_param);
    SDL_AudioSpec wanted_spec;
	  wanted_spec.freq = static_cast<int>(audio_output_param->sample_rate);
    if (audio_output_param->bits_per_sample == 16) {
		  wanted_spec.format = AUDIO_S16SYS;
    } else {
		  wanted_spec.format = AUDIO_S8;
    }	  
	  wanted_spec.channels = audio_output_param->channels;
	  wanted_spec.silence = 0;
	  wanted_spec.samples = 1024;
	  wanted_spec.callback = SDL_AudioCallback;
	  wanted_spec.userdata = reinterpret_cast<void*>(this);
	  return ::SDL_OpenAudio(&wanted_spec, nullptr) == 0;
  }

  void SDLSoundPlayer::Play() {
    ::SDL_PauseAudio(0);
  }

  void SDLSoundPlayer::Stop() {
    ::SDL_PauseAudio(1);
  }

  bool SDLSoundPlayer::SetVolume(int volume) {
    if (volume < 0) {
      return false;
    }
    volume_ = volume * SDL_MIX_MAXVOLUME / 100;
    return true;
  }

  int SDLSoundPlayer::GetVolume() const {
    return volume_ * 100 / SDL_MIX_MAXVOLUME;
  }

  void SDLSoundPlayer::SDL_AudioCallback(void* user_data, uint8_t* stream, int len) {
	  auto self = reinterpret_cast<SDLSoundPlayer*>(user_data);
	  self->AudioCallbackInner(stream, len);
  }

  void SDLSoundPlayer::AudioCallbackInner(uint8_t* stream, int len) {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    ::SDL_memset(stream, 0, len);
    if (audio_playing_buffer_.empty()) {
      return;
    }
    const auto audio_playing_buffer_size = audio_playing_buffer_.size();
    const auto request_copy_buffersize = static_cast<std::size_t>(len);
    const auto copy_size = (request_copy_buffersize > audio_playing_buffer_size ? audio_playing_buffer_size :
                              request_copy_buffersize);
    ::SDL_MixAudio(stream, reinterpret_cast<Uint8*>(&audio_playing_buffer_[0]), copy_size, volume_);
    audio_playing_buffer_.erase(audio_playing_buffer_.begin(), audio_playing_buffer_.begin() + copy_size);
  }
}

