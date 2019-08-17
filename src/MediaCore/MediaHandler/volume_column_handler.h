#ifndef VOLUME_COLUMN_HANDLER_H_
#define VOLUME_COLUMN_HANDLER_H_
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include "async_audio_handler.h"

namespace handler {
  template <class T>
  class VolumeColumnHandler final : public AsyncAudioHandler {
  public:
	  using PerChannelSamples = std::vector<T>;
	  using PerChannelSamplesPtr = std::shared_ptr<PerChannelSamples>;
	  using ChannelSamplesCollection = std::vector<PerChannelSamplesPtr>;
	  using ChannelSamplesCollectionPtr = std::unique_ptr<ChannelSamplesCollection>;
    VolumeColumnHandler() = default;
    virtual ~VolumeColumnHandler() = default;
  protected:
    void Start() override {
      channel_samples_collection_ = std::make_unique<ChannelSamplesCollection>();
      for (auto i = 0; i < channel_; ++i) {
        channel_samples_collection_->push_back(std::make_shared<PerChannelSamples>());
      }
      AsyncAudioHandler::Start();
    }

    void Stop() override {
      AsyncAudioHandler::Stop();
      channel_samples_collection_->clear();
      channel_samples_collection_ = nullptr;
    }

    void CalculateAudioSample(AudioSamplePtr audio_mix_sample) override {
      std::vector<double> sample_loudness_ratio_collection;
      for (auto i = 0U; i < static_cast<unsigned>(channel_); ++i) {
        auto sample_item_collection = (*channel_samples_collection_)[i];
        sample_item_collection->clear();
        IterateSample(audio_mix_sample, i, sample_item_collection);
        sample_loudness_ratio_collection.push_back(CalculateAverageValue(sample_item_collection));
      }
      if (sink_ != nullptr) {
        sink_->OnSampleVolumeRatio(AudioHandlerType::kS16AudioColumnHandler, std::move(sample_loudness_ratio_collection));
      }
    }
  private:
    double CalculateAverageValue(PerChannelSamplesPtr item) const {
      auto average_value = std::accumulate(item->begin(), item->end(), 0) / static_cast<int>(item->size());
      auto diff = (std::numeric_limits<T>::max)() - (std::numeric_limits<T>::min)();
      auto min = (std::numeric_limits<T>::min)();
      return 100.0 * (average_value -  min / diff);
    }

    double CalculateMiddleValue(PerChannelSamplesPtr item) const {
      auto middle_value = (*std::max(item->begin(), item->end()) + *std::min(item->begin(), item->end())) / 2;
      return 100.0 * middle_value / (std::numeric_limits<T>::max)();
    }

    void IterateSample(AudioSamplePtr audio_mix_sample, unsigned channel_index, PerChannelSamplesPtr item) const {
      auto buffer = &((*audio_mix_sample)[0]);
      auto buffer_size = audio_mix_sample->size();
      auto iter = channel_index * sizeof(T);
      auto step = channel_ * sizeof(T);
      while (iter < buffer_size) {
        auto sample = *reinterpret_cast<T*>(buffer + iter);
        item->push_back(sample);
        iter += step;
      }
    }

	  ChannelSamplesCollectionPtr channel_samples_collection_;
  };
}
#endif // VOLUME_COLUMN_HANDLER_H_
