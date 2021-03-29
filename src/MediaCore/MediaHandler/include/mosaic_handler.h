#ifndef MOSAIC_HANDLER_H_
#define MOSAIC_HANDLER_H_
#include "global_media_handler.h"
#include "base_video_handler.h"
namespace handler {
	class MosaicHandler;
	using MosaicHandlerPtr = std::shared_ptr<MosaicHandler>;
  class MEDIA_HANDLER_API_HEADER MosaicHandlerEvent : public BaseVideoHandlerEvent {
  public:
	  MosaicHandlerEvent();
	  virtual  ~MosaicHandlerEvent();
  };
  	class MEDIA_HANDLER_API_HEADER MosaicHandler : public BaseVideoHandler {
	public:
		MosaicHandler();
		virtual ~MosaicHandler();
		static MosaicHandlerPtr CreateInstance();
		virtual void SetEvent(MosaicHandlerEvent* event) = 0;
		virtual void EnableMosaic(bool enable) = 0;
		virtual bool SetParam(MosaicParamPtr param) = 0;
		virtual bool Clear() = 0;
	};
}
#endif // MOSAIC_HANDLER_H_
