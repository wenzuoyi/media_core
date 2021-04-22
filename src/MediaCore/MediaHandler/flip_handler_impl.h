#ifndef FLIP_HANDLER_IMPL_H_
#define FLIP_HANDLER_IMPL_H_
#include "./include/flip_handler.h"
#include "shared_mutex.h"
#include <mutex>
namespace handler {
  class FlipHandlerImpl : public FlipHandler {
  public:
    FlipHandlerImpl();
    virtual ~FlipHandlerImpl();
  protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(FlipHandlerEvent* event) override;
    void Flip(uint16_t orientation) override;
    uint16_t State() const override;
  private:
	  bool IsHorizontalFlip() const;
	  bool IsVerticalFlip() const;
    static void HorizontalFlip(VideoFramePtr video_frame);
    static void VerticalFlip(VideoFramePtr video_frame);
	  uint16_t orientation_{ 0 };
	  FlipHandlerEvent* event_{ nullptr };
	  utils::SharedMutexPtr mutex_;
  };
}
#endif // FLIP_HANDLER_IMPL_H_
