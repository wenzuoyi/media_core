#ifndef FORMAT_CONVERTOR_IMPL_H_
#define FORMAT_CONVERTOR_IMPL_H_
#include "./include/format_convertor.h"
#include "async_runnable.hpp"
enum AVPixelFormat;
struct AVFrame;
struct SwsContext;

namespace handler {
  class FormatConvertorImpl : public FormatConvertor, public utils::AsyncRunnable<VideoFramePtr>{
  public:
    FormatConvertorImpl();
    virtual ~FormatConvertorImpl();
  protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(FormatConvertorEvent* event) override;
    void SetBasicFormatInfo(ConvertorBasicParamPtr param) override;
    void AsyncRun(std::shared_ptr<output::VideoFrame> video_frame) override;
  private:
	  static AVPixelFormat ColorSpace2PixelFormat(output::ColorSpace color_space);
    SwsContext* image_convert_context_{nullptr};
    AVFrame* frame_{nullptr};
    FormatConvertorEvent* event_{nullptr};
  };
}
#endif // FORMAT_CONVERTOR_IMPL_H_
