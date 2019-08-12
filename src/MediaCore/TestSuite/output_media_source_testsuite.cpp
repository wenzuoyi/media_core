#include <thread>
#include "output_media_source_testsuite.h"
namespace output {
	std::wstring OutputMediaSourceTestSuite::CONSOLE_TITLE_NAME = L"Simplest Audio Play DirectSound";

  void OutputMediaSourceTestSuite::Init() {
    audio_output_media_source_ = AudioOutputMediaSource::CreateInstance();
    if (audio_output_media_source_ != nullptr) {
      audio_output_media_source_->SetAudioOutputMediaSourceEvent(this);
      audio_output_media_source_->Init();
    }
  }

  void OutputMediaSourceTestSuite::Fini() {
    if (audio_output_media_source_ != nullptr) {
      audio_output_media_source_->Fini();
      audio_output_media_source_ = nullptr;
    }
  }

  void OutputMediaSourceTestSuite::TestPlayAudioFile() {
	  FILE* audio_file{ nullptr };
	  SetConsoleTitle(CONSOLE_TITLE_NAME.c_str());
	  if ((audio_file = fopen(R"(D:\audio\sample.pcm)", "rb")) == NULL) {
		  return;
	  }
	  auto config = std::make_shared<AudioOutputParam>();
	  config->bits_per_sample = 16;
	  config->sample_rate = 44100;
	  config->channels = 2;
	  config->player_wnd = FindWindow(nullptr, CONSOLE_TITLE_NAME.c_str());
	  audio_output_media_source_->SetAudioOutputMediaParam(config);
	  audio_output_media_source_->Play();
	  auto buffer_size = config->sample_rate / 25 * config->bits_per_sample / 8 * config->channels;
	  auto buffer = new char[buffer_size];
	  auto play_count = 3U;
    while (play_count > 0) {
      if (fread(buffer, 1, buffer_size, audio_file) != buffer_size) {
        fseek(audio_file, 0, SEEK_SET);
        fread(buffer, 1, buffer_size, audio_file);
        play_count--;
      }
      auto sample = std::make_shared<AudioSample>(buffer, buffer_size + buffer);
      audio_output_media_source_->InputAudioSample(sample);
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
	  audio_output_media_source_->Stop();
    if (buffer != nullptr) {
      delete[] buffer;
      buffer = nullptr;
    }
    if (audio_file != nullptr) {
      fclose(audio_file);
      audio_file = nullptr;
    }
  }
  
  void OutputMediaSourceTestSuite::OnAudioOutputMediaExceptionEvent(unsigned error_code) {
    
  }

  void OutputMediaSourceTestSuite::OnTransmitDataEvent(AudioSamplePtr audio_sample) {
    
  }

  void OutputMediaSourceTestSuite::OnSampleFrequency(unsigned long frequency) {
    
  }


}
