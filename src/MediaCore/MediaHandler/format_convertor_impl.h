#ifndef FORMAT_CONVERTOR_IMPL_H_
#define FORMAT_CONVERTOR_IMPL_H_
#include "./include/format_convertor.h"
namespace handler {
  class FormatConvertorImpl : public FormatConvertor {
  public:
    FormatConvertorImpl();
    virtual ~FormatConvertorImpl();
  protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(FormatConvertor* event) override;
    void SetBasicFormatInfo(ConvertorBasicParamPtr param) override;
  private:
  };
}
#endif // FORMAT_CONVERTOR_IMPL_H_
