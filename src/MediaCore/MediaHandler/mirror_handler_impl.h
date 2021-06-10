#ifndef MIRROR_HANDLER_IMPL_H_
#define MIRROR_HANDLER_IMPL_H_
#include "./include/mirror_handler.h"
#include "shared_mutex.h"
namespace handler {
  class MirrorHandlerImpl : public MirrorHandler {
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
  private:
	  static void HandleVideoFrame(VideoFramePtr video_frame);
	  bool enable_{ false };
	  MirrorHandlerEvent* event_{ nullptr };
	  utils::SharedMutexPtr mutex_{ nullptr };
  };
}
#endif // MIRROR_HANDLER_IMPL_H_