#ifndef VIDEO_DECODER_H_
#define VIDEO_DECODER_H_
#include "./include/video_handler_datatype.h"

namespace handler {
  class VideoDecoder;
  using VideoDecoderPtr = std::shared_ptr<VideoDecoder>;

  class VideoDecoder {
  public:
    VideoDecoder();
    virtual ~VideoDecoder();
    virtual bool Init(VideoPackageType video_package_type) = 0;
    virtual void Fini() = 0;
    virtual VideoFrameListPtr Decode(VideoPackagePtr package) = 0;
    static VideoDecoderPtr CreateInstance();
  };
}
#endif // VIDEO_DECODER_H_
