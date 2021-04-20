#ifndef FILE_MEDIA_SOURCE_H_
#define FILE_MEDIA_SOURCE_H_
#include "base_input_media_source.h"
namespace input {
  class INPUT_MEDIA_API_HEADER FileMediaSourceEvent : public BaseInputMediaSourceEvent {
  public:
	  FileMediaSourceEvent();
	  virtual ~FileMediaSourceEvent();
	  virtual void OnEOF(InputMediaType type) = 0 ;
	  virtual void OnBOF(InputMediaType type) = 0;
  };

  class INPUT_MEDIA_API_HEADER FileMediaSource : public BaseInputMediaSource {
  public:
    FileMediaSource();
    virtual ~FileMediaSource();
    virtual void RPlay(bool enable) = 0;
    virtual void PreviousFrame() = 0;
    virtual void EnableLoopPlayback(bool enable) = 0;
  };
}
#endif // FILE_MEDIA_SOURCE_H_
