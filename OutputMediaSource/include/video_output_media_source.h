#ifndef VIDEO_OUTPUT_MEDIA_SOURCE_H_
#define VIDEO_OUTPUT_MEDIA_SOURCE_H_
#include <memory>
#include "global_output_media_source.h"
#include "video_output_media_source_datatype.h"
namespace output {
	class VideoOutputMediaSource;
	using VideoOutputMediaSourcePtr = std::shared_ptr<VideoOutputMediaSource>;

  class API_HEADER VideoOutputMediaSourceEvent {
  public:
	  VideoOutputMediaSourceEvent() = default;
	  virtual ~VideoOutputMediaSourceEvent() = default;
	  virtual void OnVideoOutputMediaExceptionEvent(unsigned error_code) = 0;
	  virtual void OnOSDDisplayEvent(HDC hdc) = 0;
	  virtual void OnTransmitDataEvent(VideoFramePtr video_frame) = 0;
  };

  class API_HEADER VideoOutputMediaSource {
  public:
    VideoOutputMediaSource() = default;
    virtual ~VideoOutputMediaSource() = default;
    static VideoOutputMediaSourcePtr CreateInstance(RenderMode render_mode);
	  virtual void SetEvent(VideoOutputMediaSourceEvent* sink) = 0;
    virtual void SetVideoOutputMediaParam(VideoOutputParamPtr video_output_param) = 0;
    virtual void EnableOSD(bool enable) = 0;
    virtual void SetDisplayRatio(DisplayRatio display_ratio) = 0;
    virtual void Flip(bool enable) = 0;
	  virtual void Mirror(bool mirror) = 0;
	  virtual bool InputVideoFrame(VideoFramePtr video_frame) = 0;
  };
}
#endif // VIDEO_OUTPUT_MEDIA_SOURCE_H_
