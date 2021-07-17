#include "./include/video_render_file_player.h"
#include "video_render_file_player_impl.h"
namespace core {
  VideoRenderFilePlayerEvent::VideoRenderFilePlayerEvent() = default;

  VideoRenderFilePlayerEvent::~VideoRenderFilePlayerEvent() = default;

  VideoRenderFilePlayer::VideoRenderFilePlayer() = default;

	VideoRenderFilePlayer::~VideoRenderFilePlayer() = default;

  RenderFilePlayerPtr VideoRenderFilePlayer::CreateInstance() {
	  return std::make_shared<VideoRenderFilePlayerImpl>();
  }
}
