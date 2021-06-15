#include "snapshot_impl.h"
#include <Poco/Foundation.h>
#include <Poco/Path.h>
#include <Poco/File.h>
namespace handler {
	SnapshotImpl::SnapshotImpl() = default;

	SnapshotImpl::~SnapshotImpl() = default;

  void SnapshotImpl::Start() {
  }

  void SnapshotImpl::Stop() {
  }

  void SnapshotImpl::InputVideoFrame(VideoFramePtr video_frame) {
    if (event_ != nullptr) {
      event_->OnTransmitVideoFrame(VideoHandlerType::kSnapshot, video_frame);
    }
  }

  void SnapshotImpl::SetEvent(SnapshotEvent* event) {
    if (event == nullptr && event_ != event) {
      event_ = event;
    }
  }
  
  bool SnapshotImpl::Save(const std::string& path) {
    Poco::Path save_path(path);
    if (!save_path.isFile()) {
      return false;
    }
    const auto& folder_path = save_path.parent();
    Poco::File file(folder_path);
    if (!file.exists()) {
      file.createDirectories();
    }
    const auto& extension_file_name = save_path.getExtension();
    return true;
  }
}
