#ifndef JPEG_SAVER_H_
#define JPEG_SAVER_H_
#include "bitmap_saver.h"
#include <GdiPlus.h>

namespace handler {
  class JPEGSaver : public BitmapSaver {
  public:
    JPEGSaver();
    virtual ~JPEGSaver();
  protected:
    bool Open(const std::string& file_path) override;
    void Close() override;
  private:
    static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	  bool ConvertBitmapToJPEG(const std::wstring& bmp_file_path, const std::wstring& jpeg_file_path, int qa) const;
	 std::string bmp_file_path_;
	 std::string jpeg_file_path_;
	 ULONG_PTR token_;
	 std::wstring mime_;
	 Gdiplus::GdiplusStartupInput startup_input_;
  };
}
#endif // JPEG_SAVER_H_
