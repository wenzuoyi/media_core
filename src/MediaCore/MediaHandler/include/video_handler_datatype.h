#ifndef VIDEO_HANDLER_DATATYPE_H_
#define VIDEO_HANDLER_DATATYPE_H_
#include <vector>
#include <memory>
#include "video_output_media_source_datatype.h"
namespace handler {
  enum class VideoHandlerType {
    kMosaic = 0,
    kFlip = 1,
    kMirror = 2,
    kRotate = 3
  };
	using VideoFramePtr = output::VideoFramePtr;
  struct MosaicParam {
	  int per_block_pixel{ 5 };
	  int x;
	  int y;
	  int width;
	  int height;   
  };
  using MosaicParamPtr = std::shared_ptr<MosaicParam>;
  struct ConvertorBasicParam {
	  int source_width;
	  int source_height;
	  output::ColorSpace source_format;
	  int target_width;
	  int target_height;
	  output::ColorSpace target_format;
  };
  using ConvertorBasicParamPtr = std::shared_ptr<ConvertorBasicParam>;
}
#endif // VIDEO_HANDLER_DATATYPE_H_
