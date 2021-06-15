#include "include/snapshot_handler.h"
#include "snapshot_handler_impl.h"
namespace handler {
	SnapshotHandlerEvent::SnapshotHandlerEvent() = default;

	SnapshotHandlerEvent::~SnapshotHandlerEvent() = default;

	SnapshotHandler::SnapshotHandler() = default;

	SnapshotHandler::~SnapshotHandler() = default;

	SnapshotHandlerPtr SnapshotHandler::CreateInstance() {
	 return std::make_shared<SnapshotHandlerImpl>();
  }
}
