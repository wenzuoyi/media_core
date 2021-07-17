
#ifndef VIDEO_RENDER_FILE_PLAYER_IMPL_H_
#define VIDEO_RENDER_FILE_PLAYER_IMPL_H_
#include "include/video_render_file_player.h"
#include "unique_channel_uncompress_video_player.h"
#include "render_file_reader.h"
namespace core {
  class VideoRenderFilePlayerImpl : public VideoRenderFilePlayer ,
                                                         public input::RenderFileReaderEvent,
                                                         public UniqueChannelBaseVideoPlayerEvent {
  public:
    VideoRenderFilePlayerImpl();
    virtual ~VideoRenderFilePlayerImpl();
  protected:
    bool Init(BasicPlayerParamPtr param) override;
    void Fini() override;
    bool Open(const std::string& uri) override;
    void Close() override;
    bool Play() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    bool Seek(int64_t time_stamp) override;
    bool NextFrame() override;
    void Speed(double speed) override;
    void Mute(bool enable) override;
    void SetVolume(int volume) override;
    int GetVolume() override;
    bool Snapshot(const std::string& url) override;
    bool IsZoom() const override;
    void Zoom(RegionPtr region) override;
    bool IsSettingMosaic() const override;
    void Mosaic(RegionPtr region) override;
    bool IsMirror() const override;
    void Mirror(bool enable) override;
    void EnableRotation(bool enable) override;
    bool IsEnableRotation() const override;
    void Rotate(RotationOptions options) override;
    RotationOptions GetRotateType() const override;
    bool SwitchStream(StreamType type) override;
    void ImageScale(AspectRatio ratio) override;
    void OSD(OSDParamListPtr param) override;
    void RPlay(bool enable) override;
    void PreviousFrame() override;
    void EnableLoopPlayback(bool enable) override;
    void SetEvent(VideoRenderFilePlayerEvent* event) override;
    void SetFormat(RenderFormat format) override;
    void SetResolution(const std::string& resolution) override;
    int Width() const override;
    int Height() const override;
    bool IsValidRegion(const POINT& point) const override;
    PlayerStatus Status() const override;
    void OnDemuxException(int error_code, const std::string& error_message) override;
    void OnDemuxAudioPackage(input::InputMediaType type, AudioPackagePtr package) override;
    void OnDemuxVideoPackage(input::InputMediaType type, VideoPackagePtr package) override;
    void OnVideoBaseInfoChanged(input::InputMediaType type, VideoBaseInfoPtr previous_format,      VideoBaseInfoPtr current_format) override;
    void OnAudioBaseInfoChanged(input::InputMediaType type, input::AudioBaseInfoPtr previous_format,  input::AudioBaseInfoPtr current_format) override;
    void OnEOF(input::InputMediaType type) override;
    void OnBOF(input::InputMediaType type) override;
    void OnVideoException(const std::string& message, int code, BaseVideoPlayerFilterPtr filter) override;
    void OnVideoPainting(HDC hdc, BaseVideoPlayerFilterPtr filter) override;
    void OnVideoFrame(VideoFramePtr video_frame, BaseVideoPlayerFilterPtr filter) override;
  private:
	  static VideoFramePtr CreateYUVObject(VideoPackagePtr package);
    input::RenderFileReaderPtr render_file_reader_{nullptr};
    VideoRenderFilePlayerEvent* event_{nullptr};
    VideoBaseInfoPtr video_base_info_{nullptr};    
    BasicPlayerParamPtr basic_player_param_{nullptr};
    UniqueChannelUnCompressVideoPlayerPtr unique_channel_uncompress_video_player_;
	  PlayerStatus player_status_{ PlayerStatus::kClose };
  };
}
#endif // VIDEO_RENDER_FILE_PLAYER_IMPL_H_
