#ifndef ASYNC_AUDIO_HANDLER_H_
#define ASYNC_AUDIO_HANDLER_H_
#include <future>
#include "include/audio_handler.h"
#include "../MediaCoreUtils/include/bound_circle_buffer.hpp"

namespace handler {
  class AsyncAudioHandler : public AudioHandler {
  public:
    AsyncAudioHandler();
    virtual ~AsyncAudioHandler() = default;
  protected:
	  void SetAudioHandlerEvent(AudioHandlerEvent* sink) override;
    void SetAudioSampleParam(int channel) override;
    void Start() override;
    void Stop() override;
    bool InputAudioSample(AudioSamplePtr audio_sample) override;
	  virtual void CalculateAudioSample(AudioSamplePtr audio_sample) = 0;
	  utils::BoundedBuffer<AudioSamplePtr> audio_sample_circle_buffer_;
	  int channel_{ 2 };
	  AudioHandlerEvent* sink_{ nullptr };
  private:
	  bool exit_{ false };
	  std::future<void> async_work_task_;
	  static const int MAX_SAMPLE_NUM;
  }; 
}
#endif // ASYNC_AUDIO_HANDLER_H_
