#ifndef MIRROR_HANDLER_H_
#define MIRROR_HANDLER_H_
#include "global_media_handler.h"
#include "base_video_handler.h"
namespace handler {
	class MirrorHandler;
	using MirrorHandlerPtr = std::shared_ptr<MirrorHandler>;
  class MEDIA_HANDLER_API_HEADER MirrorHandlerEvent : virtual public BaseVideoHandlerEvent {
  public:
    MirrorHandlerEvent();
    virtual ~MirrorHandlerEvent();
  };
  class MEDIA_HANDLER_API_HEADER MirrorHandler : public  BaseVideoHandler {
  public:
    MirrorHandler();
    virtual ~MirrorHandler();
    static MirrorHandlerPtr CreateInstance();
    virtual void SetEvent(MirrorHandlerEvent* event) = 0;
	  virtual void EnableMirror(bool enable) = 0;
	  virtual bool IsEnableMirror() const = 0;
  };
}
#endif // MIRROR_HANDLER_H_
