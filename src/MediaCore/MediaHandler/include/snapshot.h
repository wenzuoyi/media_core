#include "global_media_handler.h"
#include "base_video_handler.h"
namespace handler {
	class MEDIA_HANDLER_API_HEADER SnapshotEvent : public BaseVideoHandlerEvent {
	public:
		SnapshotEvent();
		virtual ~SnapshotEvent();
	};

	class MEDIA_HANDLER_API_HEADER Snapshot : public BaseVideoHandler {
	public:
		Snapshot();
		virtual ~Snapshot();
		virtual void SetEvent(SnapshotEvent* event) = 0;
		virtual bool Save(const std::string& path) = 0;
	};
}


