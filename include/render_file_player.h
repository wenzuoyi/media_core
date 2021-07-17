#ifndef RENDER_FILE_PLAYER_H_
#define RENDER_FILE_PLAYER_H_
#include "file_player.h"
#include "render_file_player_dataype.h"
namespace core {
	class RenderFilePlayer;
	using RenderFilePlayerPtr = std::shared_ptr<RenderFilePlayer>;
  class MEDIA_CORE_API_HEADER RenderFilePlayerEvent : virtual public  FilePlayerEvent {
  public:
	  RenderFilePlayerEvent();
	  virtual  ~RenderFilePlayerEvent();
  };
	class MEDIA_CORE_API_HEADER RenderFilePlayer : virtual public  FilePlayer {
	public:
		RenderFilePlayer();
		virtual ~RenderFilePlayer();
		virtual void SetEvent(RenderFilePlayerEvent* event) = 0;
		virtual void SetFormat(RenderFormat format) = 0;
		virtual void SetResolution(const std::string& resolution) = 0;
		static RenderFilePlayerPtr CreateInstance();
	};
}
#endif // RENDER_FILE_PLAYER_H_
