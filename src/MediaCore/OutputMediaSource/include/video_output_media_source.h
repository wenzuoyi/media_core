#ifndef VIDEO_OUTPUT_MEDIA_SOURCE_H_
#define VIDEO_OUTPUT_MEDIA_SOURCE_H_
#include <memory>
#include "global_output_media_source.h"
#include "video_output_media_source_datatype.h"

namespace output {
  class VideoOutputMediaSource;
  using VideoOutputMediaSourcePtr = std::shared_ptr<VideoOutputMediaSource>;

  class OUTPUT_MEDIA_API_HEADER VideoOutputMediaSourceEvent {
  public:
    VideoOutputMediaSourceEvent() = default;
    virtual ~VideoOutputMediaSourceEvent() = default;
    virtual void OnVideoOutputException(unsigned error_code) = 0;
    virtual void OnVideoCustomPainting(HDC hdc) = 0;
    virtual void OnVideoTransmitFrame(VideoFramePtr video_frame) = 0;
  };

  class OUTPUT_MEDIA_API_HEADER VideoOutputMediaSource {
  public:
    VideoOutputMediaSource() = default;
    virtual ~VideoOutputMediaSource() = default;
    static VideoOutputMediaSourcePtr CreateInstance(RenderMode render_mode);
    virtual void Init() = 0;
    virtual void Fini() = 0;
    virtual void SetEvent(VideoOutputMediaSourceEvent* sink) = 0;
    virtual void SetVideoOutputMediaParam(VideoOutputParamPtr video_output_param) = 0;
    virtual VideoOutputParamPtr GetVideoOutputMediaParam() = 0;
    virtual bool Renderable(VideoFramePtr video_frame) const = 0;
    virtual bool Play() = 0;
    virtual void Stop() = 0;
	  virtual bool IsROIEnable() const = 0;
    virtual void EnableROI(bool enable) = 0;
    virtual void UpdateROI(const RECT& roi) = 0;
    virtual void SetOSD(OSDParamListPtr osd_param_list) = 0;
    virtual void SetDisplayRatio(AspectRatio display_ratio) = 0;
    virtual void InputVideoFrame(VideoFramePtr video_frame) = 0;
    virtual void ResizeWindow() = 0;
	  virtual bool IsValidRendingArea(const POINT& point) const = 0;	  
  };
}
#endif // VIDEO_OUTPUT_MEDIA_SOURCE_H_
