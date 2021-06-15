#include "bitmap_saver.h"
#include "libyuv/convert_argb.h"
namespace handler {
  BitmapSaver::BitmapSaver() {
	  file_header_.bfType = 0x4d42;
	  file_header_.bfReserved1 = 0;
	  file_header_.bfReserved2 = 0;
	  file_header_.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	  info_header_.biClrImportant = 0;
	  info_header_.biClrUsed = 0;
	  info_header_.biCompression = 0;
	  info_header_.biPlanes = 1;
	  info_header_.biSize = sizeof(BITMAPINFOHEADER);
	  info_header_.biXPelsPerMeter = 3779;
	  info_header_.biYPelsPerMeter = 3779;
  }

  BitmapSaver::~BitmapSaver() = default;

  bool BitmapSaver::Open(const std::string& file_path) {
	  locale_ = std::locale::global(std::locale(""));
    ofs_.open(file_path, std::ios::out | std::ios::binary);
    if (ofs_.fail()) {
      return false;
    }
    return true;
  }

  void BitmapSaver::InputVideoFrame(VideoFramePtr frame) {
    if (frame == nullptr) {
      return;
    }
    file_header_.bfSize = frame->width * frame->height * 3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    info_header_.biBitCount = 24;
    info_header_.biHeight = frame->height * -1;
    info_header_.biWidth = frame->width;
    info_header_.biSizeImage = frame->width * frame->height * 3;
    if (ofs_.is_open()) {
      ofs_.write(reinterpret_cast<char*>(&file_header_), sizeof(BITMAPFILEHEADER));
      ofs_.write(reinterpret_cast<char*>(&info_header_), sizeof(BITMAPINFOHEADER));
      auto rgb_frame = ConvertYUV420ToRGB24(frame);
      auto* const rgb = &(*(rgb_frame->data[0]))[0];
      auto rgb_size = rgb_frame->line_size[0] * rgb_frame->height;
      ofs_.write(reinterpret_cast<char*>(rgb), rgb_size);
      ofs_.flush();
    }
  }

  void BitmapSaver::Close() {
	  ofs_.close();
	  std::locale::global(std::locale(locale_));
  }

  VideoFramePtr BitmapSaver::ConvertYUV420ToRGB24(VideoFramePtr frame) {
	  auto target = std::make_shared<output::VideoFrame>();
	  target->width = frame->width;
	  target->height = frame->height;
	  target->line_size[0] = target->width *  3;
	  target->data[0] = std::make_shared<output::Buffer>(target->line_size[0] * frame->height, 0);
	  target->line_size[1] = 0;
	  target->line_size[2] = 0;
	  auto* const target_y = &(*(target->data[0]))[0];
	  auto target_y_linesize = target->line_size[0];	
	  auto* const source_y = &(*(frame->data[0]))[0];
	  auto source_y_linesize = frame->line_size[0];
	  auto* const source_u = &(*(frame->data[1]))[0];
	  auto source_u_linesize = frame->line_size[1];
	  auto* const source_v = &(*(frame->data[2]))[0];
	  auto source_v_linesize = frame->line_size[2];
	  libyuv::I420ToRGB24(source_y, source_y_linesize, source_u, source_u_linesize, source_v, source_v_linesize, target_y,
		  target_y_linesize, frame->width, frame->height);
	  return target;
  }
}
