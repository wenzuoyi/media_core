#include "character_set_convertor_impl.h"
#include <string>
#include <codecvt>
namespace utils {
  std::string CharacterSetConvertorImpl::UnicodeToUTF8(const std::wstring& unicode_string) {
	  static std::wstring_convert<std::codecvt_utf8<wchar_t> > convertor;    
	  return convertor.to_bytes(unicode_string);
  }

  std::wstring CharacterSetConvertorImpl::UTF8ToUnicode(const std::string& utf8_string) {
	  static std::wstring_convert< std::codecvt_utf8<wchar_t> > convertor;
	  return convertor.from_bytes(utf8_string);
  }

  std::string CharacterSetConvertorImpl::UnicodeToGBK(const std::wstring& unicode_string) {
	  using F = std::codecvt_byname<wchar_t, char, std::mbstate_t>;
	  static std::wstring_convert<F> convertor(new F("chs"));
	  return convertor.to_bytes(unicode_string);
  }

  std::wstring CharacterSetConvertorImpl::GBKToUnicode(const std::string& gbk_string) {
	  using F = std::codecvt_byname<wchar_t, char, std::mbstate_t>;
	  static std::wstring_convert<F> convertor(new F("chs"));
	  return convertor.from_bytes(gbk_string);
  }

  std::string CharacterSetConvertorImpl::UTF8ToGBK(const std::string& uf8_string) {
    auto temp = UTF8ToUnicode(uf8_string);
    return UnicodeToGBK(temp);
  }

  std::string CharacterSetConvertorImpl::GBKToUTF8(const std::string& gbk_string) {
	  auto temp = GBKToUnicode(gbk_string);
	  return UnicodeToUTF8(temp);
  }
}