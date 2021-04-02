#include "render_file_reader_impl.h"
#include <regex>
#include <algorithm>
namespace input {
  RenderFileReaderImpl::RenderFileReaderImpl() {
  }

  RenderFileReaderImpl::~RenderFileReaderImpl() {
  }

  bool RenderFileReaderImpl::Open(const std::string& uri) {
	  return true;
  }

  bool RenderFileReaderImpl::Play() {
	  return true;
  }

  bool RenderFileReaderImpl::Stop() {
	  return true;
  }

  bool RenderFileReaderImpl::Pause() {
	  return true;
  }

  bool RenderFileReaderImpl::Resume() {
	  return true;
  }

  bool RenderFileReaderImpl::Seek(int64_t time_stamp) {
	  return true;
  }

  bool RenderFileReaderImpl::NextFrame() {
	  return true;
  }

  bool RenderFileReaderImpl::Speed(double speed) {
	  return true;
  }

  bool RenderFileReaderImpl::RPlay() {
	  return true;
  }

  bool RenderFileReaderImpl::PreviousFrame() {
	  return true;
  }

  void RenderFileReaderImpl::SetFormat(RenderFormat format) {
    if (format != format_) {
      format_ = format;
    }
  }

  void RenderFileReaderImpl::SetResolution(const std::string& resolution) {
    std::regex regex{"([0-9]+)[xX]([0-9]+)"};
    std::smatch matches;
    if (std::regex_match(resolution, matches, regex)) {
      if (matches.size() == 2) {
        with_ = std::stoi(matches[0].str());
        height_ = std::stoi(matches[1].str());
      }
    }
  }
}
