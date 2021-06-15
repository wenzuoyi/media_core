#ifndef ROTATION_HANDLER_IMPL_H_
#define ROTATION_HANDLER_IMPL_H_
#include <libyuv/rotate.h>

#include "include/rotation_handler.h"

namespace handler {
  class RotationHandlerImpl : public RotationHandler {
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
    void Rotate(int degree) override;
  private:
	  VideoFramePtr RotateVideoFrame(VideoFramePtr video_frame) const;
	  void ConvertEnumTypeToInteger(RotationDegreeType rotation_degree_type, int* degree);
	  void ConvertIntegerToEnumType(int degree, RotationDegreeType* rotation_degree_type);
    VideoFramePtr RotateViaLibYUV(VideoFramePtr source, bool flag, libyuv::RotationMode mode) const;	  
    static VideoFramePtr Clone(VideoFramePtr source);
	  VideoFramePtr RotateViaIntegerValue(VideoFramePtr source) const;
    bool enable_{false};
    RotationDegreeType rotation_degree_type_{RotationDegreeType::kDegreeUnknown };
    int rotation_degree_{0};
    bool use_integer_{false};
	  RotationHandlerEvent* event_{ nullptr };
	  double sin_alpha_{ 0.0f };
	  double cos_alpha_{ 0.0f };
	  static const double PI;
  };
}
#endif // ROTATION_HANDLER_IMPL_H_
