#ifndef MIRROR_HANDLER_IMPL_H_
#define MIRROR_HANDLER_IMPL_H_
#include "./include/mirror_handler.h"
#include "shared_mutex.h"
#include "async_runnable.hpp"
namespace handler {
  class MirrorHandlerImpl : public MirrorHandler , public AsyncRunnable<VideoFramePtr>{
  public:
    MirrorHandlerImpl();
    virtual ~MirrorHandlerImpl();
  protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(MirrorHandlerEvent* event) override;
    void EnableMirror(bool enable) override;
    bool IsEnableMirror() const override;
    void AsyncRun(std::shared_ptr<output::VideoFrame> video_frame) override;
  private:
	  static VideoFramePtr HandleVideoFrame(VideoFramePtr source);
	  bool enable_{ false };
	  MirrorHandlerEvent* event_{ nullptr };
	  utils::SharedMutexPtr mutex_{ nullptr };
  };
}
#endif // MIRROR_HANDLER_IMPL_H_
