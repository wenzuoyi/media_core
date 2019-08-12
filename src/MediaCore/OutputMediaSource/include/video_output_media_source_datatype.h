#ifndef VIDEO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
#define VIDEO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
#include <vector>
#include <memory>
#include <Windows.h>

namespace output {
  enum class DisplayRatio {
    kOriginStreamRatio = 0,
    kDisplayWindowRatio = 1,
  };

  enum class RenderMode {
    kD3D = 0,
    kSdl = 1,
  };

  enum class ColorSpace {
    kYUV420P = 0,
    kBGRA = 1,
  };

  using VideoFrame = std::vector<unsigned char>;
  using VideoFramePtr = std::shared_ptr<VideoFrame>;

  struct VideoOutputParam {
    HWND render_wnd{nullptr};
    unsigned width{0};
    unsigned height{0};
    ColorSpace color_space{ColorSpace::kYUV420P};
  };
  using VideoOutputParamPtr = std::shared_ptr<VideoOutputParam>;
}
#endif // VIDEO_OUTPUT_MEDIA_SOURCE_DATATYPE_H_
