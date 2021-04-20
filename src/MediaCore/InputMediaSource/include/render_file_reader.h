#ifndef RENDER_FILE_READER_H_
#define RENDER_FILE_READER_H_
#include "file_media_source.h"
#include "render_file_reader_datatype.h"
namespace input {
	class RenderFileReader;
	using RenderFileReaderPtr = std::shared_ptr<RenderFileReader>;

  class INPUT_MEDIA_API_HEADER RenderFileReaderEvent : public FileMediaSourceEvent {
  public:
	  RenderFileReaderEvent();
	  virtual ~RenderFileReaderEvent();
  };
	class INPUT_MEDIA_API_HEADER RenderFileReader :	public FileMediaSource {
	public:
		RenderFileReader();
		virtual ~RenderFileReader();
		virtual void SetEvent(RenderFileReaderEvent* event) = 0;
		virtual void SetFormat(RenderFormat format) = 0;
		virtual void SetResolution(const std::string& resolution) = 0;
		static RenderFileReaderPtr CreateInstance();
	};
}
#endif // RENDER_FILE_READER_H_
