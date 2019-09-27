#include "include/audio_output_media_source.h"
#include "direct_sound_player.h"
#include "sdl_sound_player.h"
namespace output {
  AudioOutputMediaSourcePtr AudioOutputMediaSource::CreateInstance(PlayingMode playing_mode) {
	  AudioOutputMediaSourcePtr instance{ nullptr };
    switch (playing_mode) {
      case PlayingMode::kDirectSound: {
        instance = std::make_shared<DirectSoundPlayer>();
        break;
      }
      case PlayingMode::kSDL: {
        instance = std::make_shared<SDLSoundPlayer>();
        break;
      }
      default: {
        instance = std::make_shared<SDLSoundPlayer>();
      }
    }
	  return instance;
  }
}
