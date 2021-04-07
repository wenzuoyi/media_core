#ifndef RENDER_FILE_PLAYER_H_
#define RENDER_FILE_PLAYER_H_
#include "file_player.h"

namespace core {
  class MEDIA_CORE_API_HEADER RenderFilePlayerEvent : public  FilePlayerEvent {
  public:
	  RenderFilePlayerEvent();
	  virtual  ~RenderFilePlayerEvent();
  };

	class MEDIA_CORE_API_HEADER RenderFilePlayer : public  FilePlayer {
	public:
		RenderFilePlayer();
		virtual ~RenderFilePlayer();

	};
}
#endif // RENDER_FILE_PLAYER_H_
