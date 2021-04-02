#ifndef DEVICE_MEDIA_SOURCE_H_
#define DEVICE_MEDIA_SOURCE_H_
#include "base_input_media_source.h"

namespace input {
   class INPUT_MEDIA_API_HEADER DeviceMediaSourceEvent : public BaseInputMediaSourceEvent {
   public:
	   DeviceMediaSourceEvent();
	   virtual  ~DeviceMediaSourceEvent();
   };

  class INPUT_MEDIA_API_HEADER DeviceMediaSource : public BaseInputMediaSource {
  public:
    DeviceMediaSource();
    virtual ~DeviceMediaSource();
  };
}
#endif // DEVICE_MEDIA_SOURCE_H_
