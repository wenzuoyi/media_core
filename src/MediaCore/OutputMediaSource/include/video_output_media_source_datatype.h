#ifndef VIDEO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
#define VIDEO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <Windows.h>

namespace output {
	enum class DisplayRatio {
		kAdapter = 0,
		kRatio43 = 1,
		kRatio169 = 2
  };

  enum class RenderMode {
    kD3D = 0,
    kSdl = 1,
    kGDI = 2,
  };

  enum class ColorSpace {
    kYUV420P = 0,
    kBGRA = 1,
  };
  using Buffer = std::vector<unsigned char>;
  using BufferPtr = std::shared_ptr<Buffer>;
  struct VideoFrame {
	  int width;
	  int height;
	  std::array<BufferPtr, 3> data;
	  std::array<int, 3> line_size;
  };
  using VideoFramePtr = std::shared_ptr<VideoFrame>;

  struct VideoOutputParam {
    HWND render_wnd{nullptr};
    unsigned width{0};
    unsigned height{0};
    ColorSpace color_space{ColorSpace::kYUV420P};
  };
  using VideoOutputParamPtr = std::shared_ptr<VideoOutputParam>;
  
  struct OSDParam {
	  bool enable{ false };
	  int x_pos{ 0 };
	  int y_pos{ 0 };
	  int width{ 0 };
	  int height{ 0 };
	  std::string content;
  };
  using OSDParamList = std::array<OSDParam, 8>;
  using OSDParamListPtr = std::shared_ptr<OSDParamList>;
}
#endif // VIDEO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
