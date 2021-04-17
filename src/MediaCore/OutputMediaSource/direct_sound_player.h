#ifndef DIRECT_SOUND_PLAYER_H_
#define DIRECT_SOUND_PLAYER_H_
#include <memory>
#include <vector>
#include <future>
#include <mutex>
#include <Windows.h>
#include <dsound.h>
#include "base_audio_player.h"
namespace output {
	class DirectSoundPlayer final : public BaseAudioPlayer {
	public:
		DirectSoundPlayer() = default;
		virtual ~DirectSoundPlayer() = default;
	protected:
    bool Init() override;
    void Fini() override;
    bool SetParam(AudioOutputParamPtr audio_output_param) override;
    void Play() override;
    void Stop() override;
    bool SetVolume(int volume) override;
    int GetVolume() const override;
	private:
    void FillSoundBufferParam(AudioOutputParamPtr audio_output_param);
		bool ConfigNotifyEvent();
    void CreateAudioPlayedTask();

	  void CopyBufferToHardwareBuffer(void* buffer, unsigned long buffer_size);
		IDirectSound8* direct_sound8_{ nullptr };
		IDirectSoundBuffer* direct_sound_buffer_{ nullptr };
		IDirectSoundBuffer8* direct_sound_buffer8_{nullptr};
		DSBUFFERDESC dsbufferdesc_;
		std::vector<DSBPOSITIONNOTIFY> dsb_position_notify_list_;
		std::vector<HANDLE> notify_event_list_;
		std::future<void> play_audio_task_;
		bool exit_{ false };
    static const int MAX_AUDIO_BUF;
	};
}
#endif // DIRECT_SOUND_PLAYER_H_
