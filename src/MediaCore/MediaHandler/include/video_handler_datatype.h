#ifndef VIDEO_HANDLER_DATATYPE_H_
#define VIDEO_HANDLER_DATATYPE_H_
#include <vector>
#include <memory>
#include "video_output_media_source_datatype.h"
namespace handler {
  enum class VideoHandlerType {
    kMosaic = 0
  };
	using VideoFramePtr = output::VideoFramePtr;
  struct MosaicParam {
	  int per_block_pixel{ 8 };
	  int x;
	  int y;
	  int width;
	  int height;   
  };
  using MosaicParamPtr = std::shared_ptr<MosaicParam>;
}
#endif // VIDEO_HANDLER_DATATYPE_H_