#include "binary_file_reader.h"
#include <chrono>
using namespace std::chrono_literals;
namespace input {
	BinaryFileReaderEvent::BinaryFileReaderEvent() = default;

	BinaryFileReaderEvent::~BinaryFileReaderEvent() = default;

	BinaryFileReader::BinaryFileReader() = default;

  BinaryFileReader::~BinaryFileReader() = default;

  void BinaryFileReader::SetEvent(BinaryFileReaderEvent* event) {
    if (event != event_) {
      event_ = event;
    }
  }

  bool BinaryFileReader::Open(const std::string& file_path) {
	  ifs_.open(file_path.c_str(), std::ios::binary);
	  ifs_.seekg(0, std::ios_base::end);
	  file_size_ = ifs_.tellg();
	  ifs_.seekg(0, std::ios_base::beg);
	  return !ifs_.fail();
  }

  void BinaryFileReader::Close() {
	  ifs_.close();
  }

  bool BinaryFileReader::Start() {
    if (ifs_.fail()) {
      return true;
    }
    exit_ = false;
    read_file_task_ = std::async(std::launch::async, [this]() {
      auto read_bytes{0ULL};
      while (!exit_) {
		    uint64_t frame_size{ 0 };
			  if (!PreReadFrame(&read_bytes, &frame_size)) {
				  break;
			  }
        std::vector<char> buffer(frame_size, 0);
        ifs_.read(&buffer[0], buffer.size());
        if (ifs_.fail()) {
			    break;;
        }
        if (static_cast<unsigned>(ifs_.gcount()) != buffer.size()) {
          buffer.resize(ifs_.gcount());
        }
        read_bytes += buffer.size();
        std::chrono::milliseconds interval(interval_);
        std::this_thread::sleep_for(interval);
		    PostReadFrame(&read_bytes, buffer);
      }
    });
    return true;
  }

  void BinaryFileReader::Stop() {
	  exit_ = true;
	  read_file_task_.wait();
  }

  void BinaryFileReader::EnableLoopPlayback(bool enable) {
    if (enable != enable_loop_playback_) {
      enable_loop_playback_ = enable;
    }
  }

  void BinaryFileReader::EnableReversePlayback(bool enable) {
    if (enable != enable_reverse_playback_) {
		  enable_reverse_playback_ = enable;
    }
  }

  void BinaryFileReader::Speed(double speed) {
	  const auto interval = 40.0 / speed;
	  interval_ = static_cast<int>(interval);
  }

  bool BinaryFileReader::PreReadFrame(uint64_t* read_bytes, uint64_t* frame_size) {
	  if (*read_bytes == 0) {
		  if (event_ != nullptr) {
			  event_->OnBOF();
		  }
      if (enable_reverse_playback_) {
        if (!enable_loop_playback_) {
          return false;
        }
        *read_bytes = file_size_;
        ifs_.seekg(0, std::ios_base::end);
      }
	  }
    if (event_ != nullptr) {
      event_->OnRequestFrameSize(frame_size);
    }
    if (!enable_reverse_playback_) {
      return true;
    }
	  int64_t offset = -1 * (*frame_size);
    if (*read_bytes < *frame_size) {
      offset = -1 * (*read_bytes);
    }
    ifs_.seekg(offset);
    *read_bytes += offset;
    return true;
  }

  void BinaryFileReader::PostReadFrame(uint64_t* read_bytes, const std::vector<char>& frame_data) {
    if (event_ != nullptr) {
      event_->OnPostBinaryData(frame_data, *read_bytes);
    }
    if (*read_bytes >= file_size_) {
      if (event_ != nullptr) {
        event_->OnEOF();
      }
      if (enable_reverse_playback_) {
        const auto frame_size = frame_data.size();
        ifs_.seekg(-1 * frame_size);
        read_bytes -= frame_size;
      } else if (enable_loop_playback_) {
        *read_bytes = 0;
        ifs_.seekg(*read_bytes, std::ios_base::beg);
      } else {
        exit_ = true;
      }
    }
  }
}
