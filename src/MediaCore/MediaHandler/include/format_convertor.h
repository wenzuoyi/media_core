#ifndef FORMAT_CONVERTOR_H_
#define FORMAT_CONVERTOR_H_
#include "global_media_handler.h"
#include "base_video_handler.h"

namespace handler {
	class FormatConvertor;
	using FormatConvertorPtr = std::shared_ptr<FormatConvertor>;

  class MEDIA_HANDLER_API_HEADER FormatConvertorEvent : public BaseVideoHandlerEvent {
  public:
    FormatConvertorEvent();
    virtual ~FormatConvertorEvent();
  };

  class MEDIA_HANDLER_API_HEADER FormatConvertor : public BaseVideoHandler {
  public:
    FormatConvertor();
    virtual ~FormatConvertor();
    virtual void SetEvent(FormatConvertorEvent* event) = 0;
    virtual void SetBasicFormatInfo(ConvertorBasicParamPtr param) = 0;
	  static FormatConvertorPtr CreateInstance();
  };
}
#endif // FORMAT_CONVERTOR_H_
