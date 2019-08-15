#ifndef AUDIO_CALCULATOR_H_
#define AUDIO_CALCULATOR_H_
#include "audio_calculator_datatype.h"
#include "global_media_handler.h"
namespace handler {
	class AudioCalculator;
	using AudioCalculatorPtr = std::shared_ptr<AudioCalculator>;
  enum class SampleType {
    kS8 = 8,
    kS16 = 16
  };
  class API_HEADER AudioCalculatorEvent {
  public:
	  AudioCalculatorEvent() = default;
	  virtual ~AudioCalculatorEvent() = default;
	  virtual void OnSampleVolumeRatio(std::vector<double>&& ratio_array) = 0;
  };
	class API_HEADER AudioCalculator {
	public:
		AudioCalculator() = default;
		virtual ~AudioCalculator() = default;
		static AudioCalculatorPtr CreateInstance(SampleType sample_type);
		virtual void SetAudioSampleParam(int channel) = 0;
		virtual void SetAudioCalculatorEvent(AudioCalculatorEvent* sink) = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual bool InputAudioSample(AudioSamplePtr audio_sample) = 0;
	};
}
#endif // AUDIO_CALCULATOR_H_
