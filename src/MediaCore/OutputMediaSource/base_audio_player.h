#ifndef BASE_AUDIO_PLAYER_H_
#define BASE_AUDIO_PLAYER_H_
#include "include/audio_output_media_source.h"
#include <mutex>
#include <vector>
#include <future>
namespace output {
	class BaseAudioPlayer : public AudioOutputMediaSource {
	public:
		BaseAudioPlayer() = default;
		virtual ~BaseAudioPlayer() = default;
	protected:
    void SetEvent(AudioOutputMediaSourceEvent* sink) override;
    bool SetParam(AudioOutputParamPtr audio_output_param) override;
    void Mute() override;
    bool IsMute() const override;
    bool InputAudioSample(AudioSamplePtr audio_sample) override;
    AudioOutputMediaSourceEvent* sink_{nullptr};
	  std::mutex mutex_;
	  std::vector<uint8_t> audio_playing_buffer_;
	private:
	  unsigned max_cache_buffer_size_ {0};
	  bool is_mute_{ false };
    int previous_volume_value_{0};
	  std::future<void> frequency_collect_task_;
	  bool exit_{ false };
	  static const int AUDIO_FREQUENCY_INTERVAL;
  };
}
#endif // BASE_AUDIO_PLAYER_H_
