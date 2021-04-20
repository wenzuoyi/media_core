#include "./include/render_file_reader.h"
#include "render_file_reader_impl.h"
namespace input {
	RenderFileReaderEvent::RenderFileReaderEvent() = default;

	RenderFileReaderEvent::~RenderFileReaderEvent() = default;

  RenderFileReader::RenderFileReader() = default;

	RenderFileReader::~RenderFileReader() = default;

  RenderFileReaderPtr RenderFileReader::CreateInstance() {
	  return std::make_shared<RenderFileReaderImpl>();
  }
}
