#ifndef BASE_PLAYER_H_
#define BASE_PLAYER_H_
#include <string>
#include "global_media_core.h"
#include "base_player_datatype.h"
#include "media_core_erorcode.h"
namespace core {
  class MEDIA_CORE_API_HEADER BasePlayerEvent {
  public:
    BasePlayerEvent();
    virtual ~BasePlayerEvent();
	  virtual void OnPlayerException(int error_code, const std::string& error_message) = 0;
	  virtual void OnVideoFrameInfo(VideoBaseInfoPtr frame_info) = 0;
	  virtual void OnAudioFrameInfo(AudioBaseInfoPtr frame_info) = 0;
    virtual void OnPreDecodingPackage(VideoPackagePtr package) = 0;
	  virtual void OnPreDecodingPackage(AudioPackagePtr package ) = 0;
	  virtual void OnPreRenderingFrame(VideoFramePtr video_frame)= 0;
    virtual void OnPreRenderingFrame(AudioSamplePtr audio_sample) = 0;
	  virtual void OnPostRenderingFrame(VideoFramePtr video_frame) = 0;
	  virtual void OnPostRenderingFrame(AudioSamplePtr audio_sample) = 0;
	  virtual void OnExtraDisplay(HDC hdc) = 0;
  };

  class MEDIA_CORE_API_HEADER BasePlayer {
  public:
    BasePlayer();
    virtual ~BasePlayer();
    virtual bool Init(BasicPlayerParamPtr param) = 0;
    virtual void Fini() = 0;
    virtual bool Open(const std::string& uri) = 0;
    virtual void Close() = 0;
    virtual bool Play() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual bool Seek(int64_t time_stamp) = 0;
    virtual bool NextFrame() = 0;
    virtual void Speed(double speed) = 0;
    virtual void Mute(bool enable) = 0;
    virtual void SetVolume(int volume) = 0;
    virtual int GetVolume() = 0;
    virtual bool Snapshot(const std::string& url) = 0;
    virtual void Zoom(RegionPtr region) = 0;
    virtual void Mosaic(RegionPtr region) = 0;
    virtual void Flip() = 0;
    virtual void Mirror() = 0;
    virtual void Rotate(RotationOptions options) = 0;
	  virtual bool SwitchStream(StreamType type) = 0;
	  virtual void ImageScale(AspectRatio ratio) = 0;
	  virtual void OSD(OSDParamListPtr param) = 0;
  };
}
#endif // BASE_PLAYER_H_
