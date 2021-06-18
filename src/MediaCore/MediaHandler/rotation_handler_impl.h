#ifndef ROTATION_HANDLER_IMPL_H_
#define ROTATION_HANDLER_IMPL_H_
#include <libyuv/rotate.h>
#include "include/rotation_handler.h"
#include "async_runnable.hpp"
namespace handler {
  class RotationHandlerImpl : public RotationHandler , public AsyncRunnable<VideoFramePtr>{
  public:
    RotationHandlerImpl();
    virtual ~RotationHandlerImpl();
  protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(RotationHandlerEvent* event) override;
    void EnableRotation(bool enable) override;
    void Rotate(RotationDegreeType rotation_degree) override;
    bool GetEnableRotation() const override;
    RotationDegreeType GetRotateType() override;
    void AsyncRun(VideoFramePtr video_frame) override;
  private:
	  VideoFramePtr RotateVideoFrame(VideoFramePtr video_frame) const;
    VideoFramePtr RotateViaLibYUV(VideoFramePtr source, bool flag, libyuv::RotationMode mode) const;	  
    static VideoFramePtr Clone(VideoFramePtr source);
    bool enable_{false};
    RotationDegreeType rotation_degree_type_{RotationDegreeType::kDegreeUnknown };
	  RotationHandlerEvent* event_{ nullptr };
  };
}
#endif // ROTATION_HANDLER_IMPL_H_
