#ifndef ROTATION_HANDLER_H_
#define ROTATION_HANDLER_H_
#include "global_media_handler.h"
#include "base_video_handler.h"
namespace handler {
	class RotationHandler;
	using RotationHandlerPtr = std::shared_ptr<RotationHandler>;
  class MEDIA_HANDLER_API_HEADER RotationHandlerEvent: public BaseVideoHandlerEvent {
  public:
	  RotationHandlerEvent();
	  virtual ~RotationHandlerEvent();
  };

	class MEDIA_HANDLER_API_HEADER RotationHandler : public BaseVideoHandler {
	public:
		RotationHandler();
		virtual ~RotationHandler();
		virtual void SetEvent(RotationHandlerEvent* event) = 0;
		virtual void EnableRotation(bool enable) = 0;
		virtual bool GetEnableRotation() const = 0;
		virtual void Rotate(RotationDegreeType rotation_degree) = 0;
		virtual RotationDegreeType GetRotateType() = 0;
		static RotationHandlerPtr CreateInstance();
	};
}

#endif // ROTATION_HANDLER_H_
