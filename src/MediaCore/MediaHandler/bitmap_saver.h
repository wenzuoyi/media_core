#ifndef BITMAP_SAVER_H_
#define BITMAP_SAVER_H_
#include <fstream>
#include <Windows.h>
#include "base_saver.h"
namespace handler {
  class BitmapSaver : public BaseSaver {
  public:
    BitmapSaver();
    virtual ~BitmapSaver();
  protected:
    bool Open(const std::string& file_path) override;
    void InputVideoFrame(VideoFramePtr frame) override;
    void Close() override;
  private:
	  static VideoFramePtr ConvertYUV420ToRGB24(VideoFramePtr frame);
	  BITMAPFILEHEADER file_header_;
	  BITMAPINFOHEADER info_header_;
	  std::ofstream ofs_;
	  std::locale locale_;
  };
}
#endif // BITMAP_SAVER_H_
