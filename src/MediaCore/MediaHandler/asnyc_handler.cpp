#include "async_handler.h"
namespace handler {
	const unsigned AsyncHandler::MAX_SAMPLE_NUM = 25;

  AsyncHandler::AsyncHandler() : audio_sample_circle_buffer_(MAX_SAMPLE_NUM) {
  }

  void AsyncHandler::Start(AsyncExecuteCallback async_execute_callback) {
	  exit_ = false;
	  async_work_task_ = std::async(std::launch::async, [this, async_execute_callback = std::move(async_execute_callback)]() {
		  while (!exit_) {
			  auto item = audio_sample_circle_buffer_.PopBack();
        if (item != nullptr) {
          async_execute_callback(item);
        }
		  }
	  });
  }

  void AsyncHandler::Stop()  {
	  exit_ = true;
    InputBinaryPackage(nullptr);
	  async_work_task_.wait();
	  audio_sample_circle_buffer_.Clear();
  }

  void AsyncHandler::InputBinaryPackage(MediaBinaryPackagePtr binary_package) {
      audio_sample_circle_buffer_.PushFront(binary_package);
  }
}