#include "./include/format_convertor.h"

namespace handler {
  FormatConvertorEvent::FormatConvertorEvent() = default;

  FormatConvertorEvent::~FormatConvertorEvent() = default;

  FormatConvertor::FormatConvertor() = default;

  FormatConvertor::~FormatConvertor() = default;

  FormatConvertorPtr FormatConvertor::CreateInstance() {
	  return nullptr;
  }
}
