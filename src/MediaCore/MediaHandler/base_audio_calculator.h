#ifndef BASE_AUDIO_CALCULATOR_H_
#define BASE_AUDIO_CALCULATOR_H_
#include <future>
#include "include/audio_calculator.h"
#include "../MediaCoreUtils/include/bound_circle_buffer.hpp"
namespace handler {
  class BaseAudioCalculator : public AudioCalculator {
  public:
    BaseAudioCalculator();
    virtual ~BaseAudioCalculator() = default;
  protected:
	  void SetAudioCalculatorEvent(AudioCalculatorEvent* sink) override;
    void SetAudioSampleParam(int channel) override;
    void Start() override;
    void Stop() override;
    bool InputAudioSample(AudioSamplePtr audio_sample) override;
	  virtual void CalculateLoudness(AudioSamplePtr audio_sample) = 0;
	  utils::BoundedBuffer<AudioSamplePtr> audio_sample_circle_buffer_;
	  int channel_{ 2 };
	  AudioCalculatorEvent* sink_{ nullptr };
  private:
	  bool exit_{ false };
	  std::future<void> async_caculate_task_;
	  static const int MAX_SAMPLE_NUM;
  }; 
}
#endif // BASE_AUDIO_CALCULATOR_H_
