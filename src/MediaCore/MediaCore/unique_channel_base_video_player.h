#ifndef UNIQUE_CHANNEL_BASE_VIDEO_PLAYER_H_
#define UNIQUE_CHANNEL_BASE_VIDEO_PLAYER_H_
#include "mosaic_handler.h"
#include "mirror_handler.h"
#include "rotation_handler.h"
#include "snapshot_handler.h"
#include "video_output_media_source.h"
#include "include/base_player_datatype.h"
#include "filter.h"

namespace core {
	class UniqueChannelBaseVideoPlayerEvent {
	public:
		UniqueChannelBaseVideoPlayerEvent();
		virtual ~UniqueChannelBaseVideoPlayerEvent();
		virtual void OnVideoException(const std::string& message, int code, BaseVideoPlayerFilterPtr filter) = 0;
		virtual void OnVideoPainting(HDC hdc, BaseVideoPlayerFilterPtr filter) = 0;
		virtual void OnVideoFrame(VideoFramePtr video_frame, BaseVideoPlayerFilterPtr filter) = 0;
	};

  class UniqueChannelBaseVideoPlayer : public output::VideoOutputMediaSourceEvent,
                                                                 public handler::MosaicHandlerEvent,
                                                                 public handler::MirrorHandlerEvent,
                                                                 public handler::RotationHandlerEvent,
                                                                 public handler::SnapshotHandlerEvent {
  public:
	  UniqueChannelBaseVideoPlayer();
    virtual ~UniqueChannelBaseVideoPlayer();
    virtual void SetEvent(UniqueChannelBaseVideoPlayerEvent* event, BaseVideoPlayerFilterPtr filter = nullptr);
    virtual bool Init(BasicPlayerParamPtr basic_player_info, VideoBaseInfoPtr video_base_info);
    virtual void Fini();
    virtual bool Snapshot(const std::string& url);
    virtual bool IsZoom() const;
    virtual void Zoom(RegionPtr region);
    virtual bool IsSettingMosaic() const;
    virtual void Mosaic(RegionPtr region);
    virtual bool IsMirror() const;
    virtual void Mirror(bool enable);
    virtual bool IsEnableRotation() const;
    virtual void EnableRotation(bool enable);
    virtual void Rotate(RotationOptions options);
    virtual RotationOptions GetRotateType() const;
    virtual void ImageScale(AspectRatio ratio);
    virtual void OSD(OSDParamListPtr param);
    virtual bool IsValidRegion(const POINT& point) const;
    void OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type, VideoFramePtr video_frame) override;
    void OnHandlerException(handler::VideoHandlerType video_handler_type, const std::string& message, int code) override;
    void OnVideoOutputException(unsigned error_code) override;
    void OnVideoCustomPainting(HDC hdc) override;
    void OnVideoTransmitFrame(VideoFramePtr video_frame) override;
  protected:
	  UniqueChannelBaseVideoPlayerEvent* event_{ nullptr };
	  handler::MosaicHandlerPtr mosaic_handler_{ nullptr };
	  handler::MirrorHandlerPtr mirror_handler_{ nullptr };
	  handler::RotationHandlerPtr rotation_handler_{ nullptr };
	  handler::SnapshotHandlerPtr snapshot_handler_{ nullptr };
	  output::VideoOutputMediaSourcePtr video_output_{ nullptr };
	  BaseVideoPlayerFilterPtr filter_{ nullptr };
  };
  using UniqueChannelBaseVideoPlayerPtr = std::shared_ptr<UniqueChannelBaseVideoPlayer>;
}
#endif // UNIQUE_CHANNEL_BASE_VIDEO_PLAYER_H_
