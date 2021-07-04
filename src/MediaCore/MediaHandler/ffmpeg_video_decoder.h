#ifndef FFMPEG_VIDEO_DECODER_H_
#define FFMPEG_VIDEO_DECODER_H_
#include "video_decoder.h"
#include <functional>
extern "C" {
  #include <libavcodec/avcodec.h>
}
namespace handler {
  class FFmpegVideoDecoder : public VideoDecoder {
  public:
    FFmpegVideoDecoder();
    virtual ~FFmpegVideoDecoder();
  protected:
    bool Init(VideoPackageType video_package_type) override;
    void Fini() override;
    VideoFrameListPtr Decode(VideoPackagePtr package) override;
  private:
	  int Decode(AVPacket* packet, std::function<void(AVFrame*)>&& callback) const;
	  VideoFrameListPtr Flush();
	  static AVCodecID ConvertToAVCodecID(VideoPackageType video_package_type);
	  static VideoFramePtr ConvertAVFrameToVideoFrame(AVFrame* frame);
	  const AVCodec* codec_{nullptr};
	  AVCodecParserContext* parser_{ nullptr };
	  AVCodecContext* context_{ nullptr };
  };
}
#endif // FFMPEG_VIDEO_DECODER_H_
