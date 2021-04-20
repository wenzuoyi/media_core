#ifndef NETWORK_MEDIA_SOURCE_H_
#define NETWORK_MEDIA_SOURCE_H_
#include "base_input_media_source.h"
namespace input {
  class INPUT_MEDIA_API_HEADER NetworkMediaSourceEvent : public  BaseInputMediaSourceEvent {
  public:
	  NetworkMediaSourceEvent();
	  virtual ~NetworkMediaSourceEvent();
	  virtual void OnBufferMediaData(InputMediaType type, uint64_t buffer_size) = 0;
	  virtual void OnClientDropFrame(InputMediaType type, uint64_t time_stamp) = 0;    
  };

  class INPUT_MEDIA_API_HEADER NetworkMediaSource : public BaseInputMediaSource {
  public:
    NetworkMediaSource();
    virtual ~NetworkMediaSource();
  };
}
#endif // NETWORK_MEDIA_SOURCE_H_
