#ifndef BASE_INPUT_MEDIA_SOURCE_H_
#define BASE_INPUT_MEDIA_SOURCE_H_
#include "global_input_media_source.h"
#include "base_input_media_source_datatype.h"
namespace input {
  class INPUT_MEDIA_API_HEADER BaseInputMediaSourceEvent {
  public:
    BaseInputMediaSourceEvent();
    virtual ~BaseInputMediaSourceEvent();
    virtual void OnDemuxException(int error_code, const std::string& error_message) = 0;
    virtual void OnDemuxAudioPackage(InputMediaType type, AudioPackagePtr package) = 0;
    virtual void OnDemuxVideoPackage(InputMediaType type, AudioPackagePtr package) = 0;
    virtual void OnVideoBaseInfoChanged(InputMediaType type, VideoBaseInfoPtr previous_format, VideoBaseInfoPtr current_format) = 0;
    virtual void OnAudioBaseInfoChanged(InputMediaType type, AudioBaseInfoPtr previous_format, AudioBaseInfoPtr current_format) = 0;
  };
  class INPUT_MEDIA_API_HEADER BaseInputMediaSource {
  public:
	  BaseInputMediaSource();
	  virtual ~BaseInputMediaSource();
	  virtual bool Open(const std::string& uri) = 0;
	  virtual bool Play() = 0;
	  virtual bool Stop() = 0;
	  virtual bool Pause() = 0;
	  virtual bool Resume() = 0;
	  virtual bool Seek(int64_t time_stamp) = 0;
	  virtual bool NextFrame() = 0;
	  virtual bool Speed(double speed) = 0;
  };
}
#endif // BASE_INPUT_MEDIA_SOURCE_H_
