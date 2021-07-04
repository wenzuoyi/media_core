#ifndef BASE_VIDEO_HANDLER_H
#define BASE_VIDEO_HANDLER_H
#include "global_media_handler.h"
#include "video_handler_datatype.h"
namespace handler {
	class MEDIA_HANDLER_API_HEADER BaseVideoHandlerEvent {
	public:
		BaseVideoHandlerEvent() = default;
		virtual ~BaseVideoHandlerEvent() = default;
		virtual void OnTransmitVideoFrame(VideoHandlerType video_handler_type, VideoFramePtr video_frame) = 0;
		virtual void OnHandlerException(VideoHandlerType video_handler_type, const std::string& message, int code) = 0;
	};
	class MEDIA_HANDLER_API_HEADER BaseVideoHandler {
	public:
		BaseVideoHandler() = default;
		virtual ~BaseVideoHandler() = default;		
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void InputVideoFrame(VideoFramePtr video_frame) = 0;
	};
}
#endif // BASE_VIDEO_HANDLER_H
