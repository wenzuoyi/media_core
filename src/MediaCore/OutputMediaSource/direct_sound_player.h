#ifndef DIRECT_SOUND_PLAYER_H_
#define DIRECT_SOUND_PLAYER_H_
#include <memory>
#include <vector>
#include <future>
#include <mutex>
#include <Windows.h>
#include <dsound.h>
#include "include/audio_output_media_source.h"
namespace output {
	class DirectSoundPlayer final : public AudioOutputMediaSource {
	public:
		DirectSoundPlayer() = default;
		virtual ~DirectSoundPlayer() = default;
	protected:
    bool Init() override;
    void Fini() override;
    void SetAudioOutputMediaSourceEvent(AudioOutputMediaSourceEvent* sink) override;
    bool SetAudioOutputMediaParam(AudioOutputParamPtr audio_output_param) override;
    void Play() override;
    void Stop() override;
    void Mute() override;
    bool IsMute() const override;
    bool SetVolume(int volume) override;
    int GetVolume() const override;
    void InputAudioSample(AudioSamplePtr audio_sample) override;
	private:
		bool FillSoundBufferParam(AudioOutputParamPtr audio_output_param);
		bool ConfigNotifyEvent();
    void CreateAudioPlayedTask();
	  void CreateFrequencyCollectTask();
    AudioOutputMediaSourceEvent* sink_{ nullptr };
		IDirectSound8* direct_sound8_{ nullptr };
		IDirectSoundBuffer* direct_sound_buffer_{ nullptr };
		IDirectSoundBuffer8* direct_sound_buffer8_{nullptr};
		DSBUFFERDESC dsbufferdesc_;
		std::vector<DSBPOSITIONNOTIFY> dsb_position_notify_list_;
		std::vector<HANDLE> notify_event_list_;
		std::vector<unsigned char> played_audio_buffer_;
		std::future<void> play_audio_task_;
		std::future<void> frequency_collect_task_;
		std::mutex mutex_;
		bool exit_{ false };
		bool is_mute_{ false };
		int previous_volume_value_{ 0 };    
    static const int MAX_AUDIO_BUF;
	  static const int AUDIO_FREQUENCY_INTERVAL;		
	};
}
#endif // DIRECT_SOUND_PLAYER_H_
