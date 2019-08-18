#ifndef ASYNC_HANDLER_H_
#define ASYNC_HANDLER_H_
#include <future>
#include <functional>
#include "../MediaCoreUtils/include/bound_circle_buffer.hpp"

namespace handler {
	using MediaBinaryPackage = std::vector<unsigned char>;
	using MediaBinaryPackagePtr = std::shared_ptr<MediaBinaryPackage>;
	using AsyncExecuteCallback = std::function<void(MediaBinaryPackagePtr)>;
  class AsyncHandler {
  public:
	  AsyncHandler();
    virtual ~AsyncHandler() = default;
	  virtual void Start(AsyncExecuteCallback async_execute_callback);
	  virtual void Stop();
	  virtual void InputBinaryPackage(MediaBinaryPackagePtr binary_package);
  private:
    bool exit_{false};
    std::future<void> async_work_task_;
	  AsyncExecuteCallback async_execute_callback_;
    utils::BoundedBuffer<MediaBinaryPackagePtr> audio_sample_circle_buffer_;
    static const unsigned MAX_SAMPLE_NUM;
  };
}
#endif // ASYNC_HANDLER_H_
