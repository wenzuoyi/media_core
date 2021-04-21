#ifndef RENDER_FILE_PLAYER_IMPL_H_
#define RENDER_FILE_PLAYER_IMPL_H_
#include "include/render_file_player.h"
#include "abstract_player_object.h"
#include "render_file_reader.h"

namespace core {
  class RenderFilePlayerImpl : public AbstractPlayerObject, public RenderFilePlayer , public input::RenderFileReaderEvent {
  public:
    RenderFilePlayerImpl();
    virtual ~RenderFilePlayerImpl();
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
    uint16_t FlipState() const override;
    void Flip(uint16_t orientation) override;
    void Mirror() override;
    void Rotate(RotationOptions options) override;
    bool SwitchStream(StreamType type) override;
    void ImageScale(AspectRatio ratio) override;
    void OSD(OSDParamListPtr param) override;
    void RPlay(bool enable) override;
    void PreviousFrame() override;
    void EnableLoopPlayback(bool enable) override;
    void SetEvent(RenderFilePlayerEvent* event) override;
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
	  void OnVideoCustomPainting(HDC hdc) override;
  private:
	  static VideoFramePtr CreateYUVObject(VideoPackagePtr package);
    input::RenderFileReaderPtr render_file_reader_{nullptr};
    RenderFilePlayerEvent* event_{nullptr};
    VideoBaseInfoPtr video_base_info_{nullptr};    
    BasicPlayerParamPtr basic_player_param_{nullptr};
	  PlayerStatus player_status_{ PlayerStatus::kClose };
  };
}
#endif // RENDER_FILE_PLAYER_IMPL_H_
