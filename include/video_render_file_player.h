#ifndef VIDEO_RENDER_FILE_PLAYER_H_
#define VIDEO_RENDER_FILE_PLAYER_H_
#include "file_player.h"
#include "video_render_file_player_dataype.h"
namespace core {
	class VideoRenderFilePlayer;
	using RenderFilePlayerPtr = std::shared_ptr<VideoRenderFilePlayer>;
  class MEDIA_CORE_API_HEADER VideoRenderFilePlayerEvent : virtual public  FilePlayerEvent {
  public:
	  VideoRenderFilePlayerEvent();
	  virtual  ~VideoRenderFilePlayerEvent();
  };
	class MEDIA_CORE_API_HEADER VideoRenderFilePlayer : virtual public  FilePlayer {
	public:
		VideoRenderFilePlayer();
		virtual ~VideoRenderFilePlayer();
		virtual void SetEvent(VideoRenderFilePlayerEvent* event) = 0;
		virtual void SetFormat(RenderFormat format) = 0;
		virtual void SetResolution(const std::string& resolution) = 0;
		static RenderFilePlayerPtr CreateInstance();
	};
}
#endif // VIDEO_RENDER_FILE_PLAYER_H_
