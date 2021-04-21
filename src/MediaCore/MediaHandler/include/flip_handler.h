#ifndef FLIP_HANDLER_H_
#define FLIP_HANDLER_H_
#include "global_media_handler.h"
#include "base_video_handler.h"
#include <memory>
namespace handler {
	class FlipHandler;
	using FlipHandlerPtr = std::shared_ptr<FlipHandler>;

  class MEDIA_HANDLER_API_HEADER FlipHandlerEvent : public BaseVideoHandlerEvent {
  public:
	  FlipHandlerEvent();
	  virtual ~FlipHandlerEvent();
  };

  class MEDIA_HANDLER_API_HEADER FlipHandler : public BaseVideoHandler {
  public:
    FlipHandler();
    virtual ~FlipHandler();
    static FlipHandlerPtr CreateInstance();
    virtual void SetEvent(FlipHandlerEvent* event) = 0;
	  virtual void Flip(uint16_t orientation) = 0;
	  virtual uint16_t State() const = 0;
  };
}
#endif // FLIP_HANDLER_H_
