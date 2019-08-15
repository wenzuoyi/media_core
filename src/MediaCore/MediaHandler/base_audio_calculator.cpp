#include "base_audio_calculator.h"
namespace handler {
  
	const int BaseAudioCalculator::MAX_SAMPLE_NUM = 32;

	BaseAudioCalculator::BaseAudioCalculator() : audio_sample_circle_buffer_(MAX_SAMPLE_NUM) {
	}

  void handler::BaseAudioCalculator::Start() {
    exit_ = false;
    async_caculate_task_ = std::async(std::launch::async, [this]() {
      while (!exit_) {
        auto item = audio_sample_circle_buffer_.PopBack();
        CalculateLoudness(item);
      }
    });
  }

  void BaseAudioCalculator::Stop() {
	  exit_ = true;
	  async_caculate_task_.wait();
	  audio_sample_circle_buffer_.Clear();
  }

  void BaseAudioCalculator::SetAudioCalculatorEvent(AudioCalculatorEvent* sink) {
    if (sink_ != sink) {
      sink_ = sink;
    }
  }

  void BaseAudioCalculator::SetAudioSampleParam(int channel) {
    if (channel != channel_) {
      channel_ = channel;
    }
  }

  bool BaseAudioCalculator::InputAudioSample(AudioSamplePtr audio_sample) {
	  audio_sample_circle_buffer_.PushFront(audio_sample);
	  return true;
  }
}
