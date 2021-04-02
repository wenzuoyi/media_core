#ifndef RENDER_FILE_READER_IMPL_H_
#define RENDER_FILE_READER_IMPL_H_
#include "./include/render_file_reader.h"
namespace input {
  class RenderFileReaderImpl : public RenderFileReader {
  public:
    RenderFileReaderImpl();
    virtual ~RenderFileReaderImpl();
  protected:
    bool Open(const std::string& uri) override;
    bool Play() override;
    bool Stop() override;
    bool Pause() override;
    bool Resume() override;
    bool Seek(int64_t time_stamp) override;
    bool NextFrame() override;
    bool Speed(double speed) override;
    bool RPlay() override;
    bool PreviousFrame() override;
    void SetFormat(RenderFormat format) override;
    void SetResolution(const std::string& resolution) override;
  private:
	  RenderFormat format_{ RenderFormat::kYUV420 };
	  int with_{ 0 };
	  int height_{ 0 };
  };
}
#endif // RENDER_FILE_READER_IMPL_H_
