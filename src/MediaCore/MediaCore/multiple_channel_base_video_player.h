#ifndef MULTIPLE_CHANNEL_BASE_VIDEO_PLAYER_H_
#define MULTIPLE_CHANNEL_BASE_VIDEO_PLAYER_H_
#include <map>
#include <functional>
#include "unique_channel_base_video_player.h"

namespace core {
	using VideoPlayerParam = std::pair<BasicPlayerParamPtr, VideoBaseInfoPtr>;
	using VideoPlayerParamPtr = std::shared_ptr<VideoPlayerParam>;
	using MultipleChannelVideoPlayerParam = std::map<unsigned, VideoPlayerParamPtr>;
	using MultipleChannelVideoPlayerParamPtr = std::shared_ptr<MultipleChannelVideoPlayerParam>;

	class MultipleChannelBaseVideoPlayerEvent {
	public:
		MultipleChannelBaseVideoPlayerEvent();
		virtual ~MultipleChannelBaseVideoPlayerEvent();
		virtual void OnVideoException(int channel_id, const std::string& message, int code) = 0;
		virtual void OnVideoPainting(int channel_id, HDC hdc) = 0;
		virtual void OnVideoFrame(int channel_id, VideoFramePtr video_frame) = 0;
	};

  class MultipleChannelBaseVideoPlayer : public UniqueChannelBaseVideoPlayerEvent {
  public:
	  MultipleChannelBaseVideoPlayer();
	  virtual ~MultipleChannelBaseVideoPlayer();
	  virtual void SetEvent(MultipleChannelBaseVideoPlayerEvent* event);
	  virtual bool Init(MultipleChannelVideoPlayerParamPtr param) = 0;
		virtual void Fini();
	  virtual bool Snapshot(unsigned channel_id, const std::string& url);
	  virtual bool IsZoom(unsigned channel_id) const;
	  virtual void Zoom(unsigned channel_id, RegionPtr region);
	  virtual bool IsSettingMosaic(unsigned channel_id) const;
	  virtual void Mosaic(unsigned channel_id, RegionPtr region);
	  virtual bool IsMirror(unsigned channel_id) const;
	  virtual void Mirror(unsigned channel_id, bool enable);
	  virtual bool IsEnableRotation(unsigned channel_id) const;
	  virtual void EnableRotation(unsigned channel_id, bool enable);
	  virtual void Rotate(unsigned channel_id, RotationOptions options);
	  virtual RotationOptions GetRotateType(unsigned channel_id) const;
	  virtual void ImageScale(unsigned channel_id,AspectRatio ratio);
	  virtual void OSD(unsigned channel_id, OSDParamListPtr param);
	  virtual bool IsValidRegion(unsigned channel_id, const POINT& point) const;
  protected:
	  void OnVideoException(const std::string& message, int code, BaseVideoPlayerFilterPtr filter) override;
	  void OnVideoPainting(HDC hdc, BaseVideoPlayerFilterPtr filter) override;
	  void OnVideoFrame(VideoFramePtr video_frame, BaseVideoPlayerFilterPtr filter) override;
	  bool Execute(unsigned channel_id, std::function<bool(UniqueChannelBaseVideoPlayerPtr)>&& method) const;
	  void ExecuteEx(unsigned channel_id, std::function<void(UniqueChannelBaseVideoPlayerPtr)>&& method) const;
	  mutable std::map<unsigned, UniqueChannelBaseVideoPlayerPtr> multiple_channel_base_video_player_map_;
  private:
	  MultipleChannelBaseVideoPlayerEvent* event_{ nullptr };
  };
}
#endif // MULTIPLE_CHANNEL_BASE_VIDEO_PLAYER_H_
