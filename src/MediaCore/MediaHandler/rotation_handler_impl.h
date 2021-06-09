#ifndef ROTATION_HANDLER_IMPL_H_
#define ROTATION_HANDLER_IMPL_H_
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
	  void ConvertEnumTypeToInteger(RotationDegreeType rotation_degree_type, int* degree);
	  void ConvertIntegerToEnumType(int degree, RotationDegreeType* rotation_degree_type);
    static void Rotate90(VideoFramePtr source);
    static void Rotate180(VideoFramePtr source);
    static void Rotate270(VideoFramePtr source);
	  static VideoFramePtr Clone(VideoFramePtr source);
    void RotateViaIntegerValue(VideoFramePtr source) const;
    bool enable_{false};
    RotationDegreeType rotation_degree_type_{RotationDegreeType::kDegree0};
    int rotation_degree_{0};
    bool use_integer_{false};
	  RotationHandlerEvent* event_{ nullptr };
	  double sin_alpha_{ 0.0f };
	  double cos_alpha_{ 0.0f };
	  static const double PI;
  };
}
#endif // ROTATION_HANDLER_IMPL_H_
