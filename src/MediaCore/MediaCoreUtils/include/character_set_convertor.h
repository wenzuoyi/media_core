#ifndef CHARACTER_SET_CONVERTOR_H_
#define CHARACTER_SET_CONVERTOR_H_
#include <string>
#include "global_media_utils_source.h"
namespace utils {
  class MEDIA_CORE_UTILS_API_HEADER CharacterSetConvertor {
  public:
	  virtual std::string UnicodeToUTF8(const std::wstring& unicode_string) = 0;
	  virtual std::wstring UTF8ToUnicode(const std::string& utf8_string) = 0;
	  virtual std::string UnicodeToGBK(const std::wstring& unicode_string) = 0;
	  virtual std::wstring GBKToUnicode(const std::string& gbk_string) = 0;
	  virtual std::string UTF8ToGBK(const std::string& uf8_string) = 0;
	  virtual std::string GBKToUTF8(const std::string& gbk_string) = 0;
    static CharacterSetConvertor* Instance();
  protected:
    CharacterSetConvertor() = default;
    virtual ~CharacterSetConvertor() = default;
  };
}
#endif // CHARACTER_SET_CONVERTOR_H_
