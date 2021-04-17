#ifndef RENDER_FILE_READER_IMPL_H_
#define RENDER_FILE_READER_IMPL_H_
#include "./include/render_file_reader.h"
#include "binary_file_reader.h"
#include "event.h"
namespace input {
  class RenderFileReaderImpl : public RenderFileReader, public BinaryFileReaderEvent {
  public:
    RenderFileReaderImpl();
    virtual ~RenderFileReaderImpl();
  protected:
    void OnRequestFrameSize(uint64_t* frame_size) override;
    void OnPostBinaryData(const std::vector<char>& buffer, uint64_t offset) override;
    void OnEOF() override;
    void OnBOF() override;
    void SetEvent(RenderFileReaderEvent* event) override;
    bool Open(const std::string& uri) override;
    void Close() override;
    bool Play() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    bool Seek(int64_t time_stamp) override;
    bool NextFrame() override;
    void Speed(double speed) override;
    void RPlay(bool enable) override;
    void PreviousFrame() override;
    void SetFormat(RenderFormat format) override;
    void SetResolution(const std::string& resolution) override;
  private:
	  inline void GetFrameSize();
	  RenderFormat format_{ RenderFormat::kYUV420 };
	  int with_{ 0 };
	  int height_{ 0 };
	  BinaryFileReader binary_file_reader_;
	  RenderFileReaderEvent* event_{ nullptr };
	  utils::EventPtr player_pause_control_;
	  utils::EventPtr player_singleframe_control_;
	  uint64_t frame_size_{ 0 };
	  std::atomic_bool single_frame_flag_{ false };
  };
}
#endif // RENDER_FILE_READER_IMPL_H_
