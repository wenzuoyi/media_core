#include "snapshot_handler_impl.h"
#include <algorithm>
#include <thread>
#include <Poco/Path.h>
#include <Poco/File.h>
#include "bitmap_saver.h"
#include "jpeg_saver.h"
namespace handler {
	SnapshotHandlerImpl::SnapshotHandlerImpl() = default;

	SnapshotHandlerImpl::~SnapshotHandlerImpl() = default;

  void SnapshotHandlerImpl::Start() {
	  AsyncStart();
  }

  void SnapshotHandlerImpl::Stop() {
	  AsyncStop();
  }

  void SnapshotHandlerImpl::InputVideoFrame(VideoFramePtr video_frame) {
	  Push(video_frame);
  }

  void SnapshotHandlerImpl::SetEvent(SnapshotHandlerEvent* event) {
    if (event != nullptr && event_ != event) {
      event_ = event;
    }
  }
  
  bool SnapshotHandlerImpl::Save(const std::string& path) {
    while (flag_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    Poco::Path save_path(path);
    if (!save_path.isFile()) {
      return false;
    }
    const auto& folder_path = save_path.parent();
    Poco::File file(folder_path);
    if (!file.exists()) {
      file.createDirectories();
    }
    auto extension_file_name = save_path.getExtension();
	  std::transform(extension_file_name.begin(), extension_file_name.end(), extension_file_name.begin(), ::tolower);
    if (extension_file_name == std::string("bmp")) {
		  saver_ = std::make_shared<BitmapSaver>();
    } else if (extension_file_name == std::string("jpg") || extension_file_name == std::string("jpeg")) {
		  saver_ = std::make_shared<JPEGSaver>();
    }
    if (saver_ != nullptr) {
      saver_->Open(path);      
      flag_ = true;
    }
    return true;
  }

  void SnapshotHandlerImpl::AsyncRun(std::shared_ptr<output::VideoFrame> video_frame) {
	  if (flag_ && saver_ != nullptr) {
		  saver_->InputVideoFrame(video_frame);
		  saver_->Close();
		  flag_ = false;
	  }
	  if (event_ != nullptr) {
		  event_->OnTransmitVideoFrame(VideoHandlerType::kSnapshot, video_frame);
	  }
  }
}
