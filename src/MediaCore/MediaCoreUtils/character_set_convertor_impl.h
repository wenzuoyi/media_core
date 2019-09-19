#ifndef CHARACTER_SET_CONVERTOR_IMPL_H_
#define CHARACTER_SET_CONVERTOR_IMPL_H_
#include "include/character_set_convertor.h"
namespace utils {
  class CharacterSetConvertorImpl : public CharacterSetConvertor {
  public:
    CharacterSetConvertorImpl() = default;
    virtual ~CharacterSetConvertorImpl() = default;
  protected:
    std::string UnicodeToUTF8(const std::wstring& unicode_string) override;
    std::wstring UTF8ToUnicode(const std::string& utf8_string) override;
    std::string UnicodeToGBK(const std::wstring& unicode_string) override;
    std::wstring GBKToUnicode(const std::string& gbk_string) override;
    std::string UTF8ToGBK(const std::string& uf8_string) override;
    std::string GBKToUTF8(const std::string& gbk_string) override;
  };
}
#endif // CHARACTER_SET_CONVERTOR_IMPL_H_
