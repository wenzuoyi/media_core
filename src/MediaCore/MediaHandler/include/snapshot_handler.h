#ifndef SNAPSHOT_HANDLER_H_
#define SNAPSHOT_HANDLER_H_
#include "global_media_handler.h"
#include "base_video_handler.h"
#include <memory>
namespace handler {
	class SnapshotHandler;
	using SnapshotHandlerPtr = std::shared_ptr<SnapshotHandler>;
	class MEDIA_HANDLER_API_HEADER SnapshotHandlerEvent : public BaseVideoHandlerEvent {
	public:
		SnapshotHandlerEvent();
		virtual ~SnapshotHandlerEvent();
	};

	class MEDIA_HANDLER_API_HEADER SnapshotHandler : public BaseVideoHandler {
	public:
		SnapshotHandler();
		virtual ~SnapshotHandler();
		virtual void SetEvent(SnapshotHandlerEvent* event) = 0;
		virtual bool Save(const std::string& path) = 0;
		static SnapshotHandlerPtr CreateInstance();
	};
}
#endif // SNAPSHOT_HANDLER_H_
