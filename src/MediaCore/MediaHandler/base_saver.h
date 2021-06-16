#ifndef BASE_SAVER_H_
#define BASE_SAVER_H_
#include <string>
#include "./include/video_handler_datatype.h"
namespace handler {
	class BaseSaver;
	using BaseSaverPtr = std::shared_ptr<BaseSaver>;
  class BaseSaver {
  public:
	  BaseSaver() = default;
	  virtual ~BaseSaver() = default;
    virtual bool Open(const std::string& file_path) = 0;
    virtual void InputVideoFrame(VideoFramePtr frame) = 0;
    virtual void Close() = 0;
  };
}
#endif // BASE_SAVER_H_
