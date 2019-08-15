#ifndef S16_AUDIO_CALCULATOR_H_
#define S16_AUDIO_CALCULATOR_H_
#include <vector>
#include "base_audio_calculator.h"

namespace handler {
  class S16AudioCalculator : public BaseAudioCalculator {
  public:
	  using PerChannelSamples = std::vector<int16_t>;
	  using PerChannelSamplesPtr = std::shared_ptr<PerChannelSamples>;
	  using ChannelSamplesCollection = std::vector<PerChannelSamplesPtr>;
	  using ChannelSamplesCollectionPtr = std::unique_ptr<ChannelSamplesCollection>;
    S16AudioCalculator() = default;
    virtual ~S16AudioCalculator() = default;
  protected:
    void Start() override;
    void Stop() override;
    void CalculateLoudness(AudioSamplePtr audio_mix_sample) override;
  private:
    double CalculateAverageValue(PerChannelSamplesPtr item) const;
    double CalculateMiddleValue(PerChannelSamplesPtr item) const;
    void IterateSample(AudioSamplePtr audio_mix_sample, unsigned channel_index, PerChannelSamplesPtr item) const;
	  ChannelSamplesCollectionPtr channel_samples_collection_;
  };
}
#endif // S16_AUDIO_CALCULATOR_H_
