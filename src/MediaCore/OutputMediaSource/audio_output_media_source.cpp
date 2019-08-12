#include "include/audio_output_media_source.h"
#include "direct_sound_player.h"
namespace output {
  AudioOutputMediaSourcePtr AudioOutputMediaSource::CreateInstance() {
	  return std::make_shared<DirectSoundPlayer>();
  }
}
