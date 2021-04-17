#ifndef SDL_SOUND_PLAYER_H_
#define SDL_SOUND_PLAYER_H_
#include "base_audio_player.h"
namespace output {
	class SDLSoundPlayer final :	public BaseAudioPlayer {
	public:
		SDLSoundPlayer() = default;
		virtual ~SDLSoundPlayer() = default;
	protected:
    bool Init() override;
    void Fini() override;
    bool SetParam(AudioOutputParamPtr audio_output_param) override;
    void Play() override;
    void Stop() override;
    bool SetVolume(int volume) override;
    int GetVolume() const override;
	private:
		static void SDL_AudioCallback(void* user_data, uint8_t * stream, int len);
		void AudioCallbackInner(uint8_t * stream, int len);
		int volume_;
	};
}
#endif // SDL_SOUND_PLAYER_H_
