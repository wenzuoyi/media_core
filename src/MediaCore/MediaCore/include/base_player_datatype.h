#ifndef BASE_PLAYER_DATATYPE_H_
#define BASE_PLAYER_DATATYPE_H_
#include <memory>
#include <vector>
#include <Windows.h>
#include "base_input_media_source_datatype.h"
#include "video_output_media_source_datatype.h"
#include "audio_output_media_source_datatype.h"
namespace core {
	using Region = RECT;
  using RegionPtr = std::shared_ptr<Region>;
  using AudioOutputParam = output::AudioOutputParam;
  using AudioOutputParamPtr = output::AudioOutputParamPtr;
  using PlayingMode = output::PlayingMode;
  using RenderMode = output::RenderMode;
  struct BasicPlayerParam {
	  PlayingMode audio_render_mode{ PlayingMode::kDirectSound };
	  RenderMode video_render_mode{ RenderMode::kD3D };
	  HWND hwnd;
  };
  using BasicPlayerParamPtr = std::shared_ptr<BasicPlayerParam>;
  enum class RotationOptions {
	  kDegree90 = 0,
	  kDegree180 = 1,
	  kDegree270 = 2
  };
  enum  class StreamType {
    kMain = 0,
    kSub,
  };
  using AudioBaseInfo = input::AudioBaseInfo;
  using AudioBaseInfoPtr = input::AudioBaseInfoPtr;
  using VideoBaseInfo = input::VideoBaseInfo;
  using VideoBaseInfoPtr = input::VideoBaseInfoPtr;
  using AudioPackage = input::AudioPackage;
  using AudioPackagePtr = input::AudioPackagePtr;
  using VideoPackage = input::VideoPackage;
  using VideoPackagePtr = input::VideoPackagePtr;
  using VideoFramePtr = output::VideoFramePtr;
  using VideoFrame = output::VideoFrame;
  using AudioSample= output::AudioSample;
  using AudioSamplePtr = output::AudioSamplePtr;
  using OSDParamList = output::OSDParamList;
  using OSDParamListPtr = output::OSDParamListPtr;
  using AspectRatio = output::AspectRatio;
}
#endif // BASE_PLAYER_DATATYPE_H_
