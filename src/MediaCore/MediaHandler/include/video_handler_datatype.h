#ifndef VIDEO_HANDLER_DATATYPE_H_
#define VIDEO_HANDLER_DATATYPE_H_
#include <memory>
#include <vector>
#include "video_output_media_source_datatype.h"
#include "audio_output_media_source_datatype.h"
#include "base_input_media_source_datatype.h"
namespace handler {
  enum class VideoHandlerType {
    kMosaic = 0,
    kMirror = 1,
    kRotate = 2,
    kFormatConvert =3,
    kSnapshot = 4,
    kVideoDecoder = 5
  };
  enum class RotationDegreeType {
    kDegree0 = 0,
    kDegree90 = 1,
    kDegree180 = 2,
    kDegree270 = 3,
    kDegreeUnknown = 4
  };
  using Buffer = output::Buffer;
  using BufferPtr = output::BufferPtr;
  using VideoPackageType = input::VideoPackageType;
  using VideoPackagePtr = input::VideoPackagePtr;
  using AudioPackagePtr = input::AudioPackagePtr;
	using VideoFrame = output::VideoFrame;
	using VideoFramePtr = output::VideoFramePtr;
	using VideoFrameList = output::VideoFrameList;
	using VideoFrameListPtr = output::VideoFrameListPtr;
	using AudioFramePtr = output::AudioSamplePtr;
  
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
