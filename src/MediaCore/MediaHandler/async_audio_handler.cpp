#include "async_audio_handler.h"
namespace handler {
  
	const int AsyncAudioHandler::MAX_SAMPLE_NUM = 25;

	AsyncAudioHandler::AsyncAudioHandler() : audio_sample_circle_buffer_(MAX_SAMPLE_NUM) {
	}

  void handler::AsyncAudioHandler::Start() {
    exit_ = false;
    async_work_task_ = std::async(std::launch::async, [this]() {
      while (!exit_) {
        auto item = audio_sample_circle_buffer_.PopBack();
        CalculateAudioSample(item);
        if (sink_ != nullptr) {
          sink_->OnTransmitAudioSample(AudioHandlerType::kS16AudioColumnHandler, item);
        }
      }
    });
  }

  void AsyncAudioHandler::Stop() {
	  exit_ = true;
	  async_work_task_.wait();
	  audio_sample_circle_buffer_.Clear();
  }

  void AsyncAudioHandler::SetAudioHandlerEvent(AudioHandlerEvent* sink) {
    if (sink_ != sink) {
      sink_ = sink;
    }
  }

  void AsyncAudioHandler::SetAudioSampleParam(int channel) {
    if (channel != channel_) {
      channel_ = channel;
    }
  }

  bool AsyncAudioHandler::InputAudioSample(AudioSamplePtr audio_sample) {
	  audio_sample_circle_buffer_.PushFront(audio_sample);
	  return true;
  }
}
