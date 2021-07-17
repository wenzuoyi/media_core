#ifndef ABSTRACT_AUDIO_PLAYER_H_
#define ABSTRACT_AUDIO_PLAYER_H_
#include "audio_output_media_source.h"
#include "volume_column_handler.h"
#include "include/base_player_datatype.h"
namespace core {
  class AbstractAudioPlayer : public output::AudioOutputMediaSourceEvent,
                                              public handler::VolumeColumnHandlerEvent {
  public:
    AbstractAudioPlayer();
    virtual ~AbstractAudioPlayer();
  protected:
	  virtual bool Initialize(BasicPlayerParamPtr basic_player_info, AudioBaseInfoPtr audio_base_info) final;
	  virtual void UnInitialize() final;
    void OnAudioOutputException(unsigned error_code) override;
    void OnAudioTransmitSample(AudioSamplePtr audio_sample) override;
    void OnAudioSampleFrequency(unsigned long frequency) override;
    void OnTransmitAudioSample(handler::AudioHandlerType audio_event, AudioSamplePtr audio_sample) override;
    void OnSampleVolumeRatio(std::vector<double>&& ratio_array) override;
	  output::AudioOutputMediaSourcePtr audio_output_media_source_{ nullptr };
	  handler::VolumeColumnHandlerPtr volume_column_handler_{ nullptr };
  };
}
#endif // ABSTRACT_AUDIO_PLAYER_H_
