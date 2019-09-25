#include "include/video_output_media_source.h"
#include "direct3d_render.h"
namespace output {
  VideoOutputMediaSourcePtr VideoOutputMediaSource::CreateInstance(RenderMode render_mode) {
    VideoOutputMediaSourcePtr instance{nullptr};
    if (render_mode == RenderMode::kD3D) {
      instance = std::make_shared<Direct3DRender>();
    }
    return instance;
  }
}
