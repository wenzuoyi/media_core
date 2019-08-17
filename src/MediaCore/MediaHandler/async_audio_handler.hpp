#ifndef ASYNC_AUDIO_HANDLER_H_
#define ASYNC_AUDIO_HANDLER_H_
#include <future>
#include "../MediaCoreUtils/include/bound_circle_buffer.hpp"

namespace handler {
  #define MAX_SAMPLE_NUM 25

  template <class T>
  class AsyncAudioHandler : public T {
  public:
    AsyncAudioHandler() : audio_sample_circle_buffer_(MAX_SAMPLE_NUM) {
    }

    virtual ~AsyncAudioHandler() = default;
  protected:
	  void SetBaseAudioHandlerEvent(BaseAudioHandlerEvent* sink) override {
      if (sink_ != sink) {
        sink_ = sink;
      }
    }

    void Start() override {
      exit_ = false;
      async_work_task_ = std::async(std::launch::async, [this]() {
        while (!exit_) {
          auto item = audio_sample_circle_buffer_.PopBack();
          CalculateAudioSample(item);
          if (sink_ != nullptr) {
            sink_->OnTransmitAudioSample(AudioHandlerType::kAudioColumn, item);
          }
        }
      });
    }

    void Stop() override {
      exit_ = true;
      async_work_task_.wait();
      audio_sample_circle_buffer_.Clear();
    }

    bool InputAudioSample(AudioSamplePtr audio_sample) override {
      audio_sample_circle_buffer_.PushFront(audio_sample);
      return true;
    }

    virtual void CalculateAudioSample(AudioSamplePtr audio_sample) = 0;
    utils::BoundedBuffer<AudioSamplePtr> audio_sample_circle_buffer_;
    BaseAudioHandlerEvent* sink_{nullptr};
  private:
    bool exit_{false};
    std::future<void> async_work_task_;
  };
}
#endif // ASYNC_AUDIO_HANDLER_H_
