#include "./include/render_file_player.h"
#include "render_file_player_impl.h"
namespace core {
  RenderFilePlayerEvent::RenderFilePlayerEvent() = default;

  RenderFilePlayerEvent::~RenderFilePlayerEvent() = default;

  RenderFilePlayer::RenderFilePlayer() = default;

	RenderFilePlayer::~RenderFilePlayer() = default;

  RenderFilePlayerPtr RenderFilePlayer::CreateInstance() {
	  return std::make_shared<RenderFilePlayerImpl>();
  }
}
