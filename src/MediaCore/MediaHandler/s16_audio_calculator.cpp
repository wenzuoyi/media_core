#include <limits.h>
#include <algorithm>
#include <numeric>
#include "s16_audio_calculator.h"

namespace handler {
  #define ISUNSIGNED(a) (a>=0 && ~a>=0)

  void S16AudioCalculator::Start() {
    channel_samples_collection_ = std::make_unique<ChannelSamplesCollection>();
    for (auto i = 0; i < channel_; ++i) {
      channel_samples_collection_->push_back(std::make_shared<PerChannelSamples>());
    }
    BaseAudioCalculator::Start();
  }

  void S16AudioCalculator::Stop() {
	  BaseAudioCalculator::Stop();
	  channel_samples_collection_->clear();
	  channel_samples_collection_ = nullptr;
  }

  void S16AudioCalculator::IterateSample(AudioSamplePtr audio_mix_sample, unsigned channel_index, PerChannelSamplesPtr item) const {
	  auto buffer = &((*audio_mix_sample)[0]);
	  auto buffer_size = audio_mix_sample->size();
    auto iter = channel_index * sizeof(int16_t);
    auto step = channel_ * sizeof(int16_t);
    while (iter < buffer_size) {
      auto sample = *reinterpret_cast<int16_t*>(buffer + iter);
      item->push_back(sample);
      iter += step;
    }
  }

  void S16AudioCalculator::CalculateLoudness(AudioSamplePtr audio_mix_sample) {
    std::vector<double> sample_loudness_ratio_collection;
    for (auto i = 0U; i < static_cast<unsigned>(channel_); ++i) {
      auto sample_item_collection = (*channel_samples_collection_)[i];
      sample_item_collection->clear();
      IterateSample(audio_mix_sample, i, sample_item_collection);
      sample_loudness_ratio_collection.push_back(CalculateAverageValue(sample_item_collection));
    }
    if (sink_ != nullptr) {
      sink_->OnSampleVolumeRatio(std::move(sample_loudness_ratio_collection));
    }
  }

  double S16AudioCalculator::CalculateAverageValue(PerChannelSamplesPtr item) const {
    auto average_value = std::accumulate(item->begin(), item->end(), 0) / static_cast<int>(item->size());
    return 100.0 * (average_value - INT16_MIN) / (INT16_MAX - INT16_MIN);
  }

  double S16AudioCalculator::CalculateMiddleValue(PerChannelSamplesPtr item) const {
    auto middle_value = (*std::max(item->begin(), item->end()) + *std::min(item->begin(), item->end())) / 2;
    return 100.0 * middle_value / INT16_MAX;
  }

}
