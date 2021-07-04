#include "video_decoder.h"
#include "ffmpeg_video_decoder.h"
namespace handler {
	VideoDecoder::VideoDecoder() = default;

	VideoDecoder::~VideoDecoder() = default;

  VideoDecoderPtr VideoDecoder::CreateInstance() {
	  return std::make_shared<FFmpegVideoDecoder>();
  }
}
