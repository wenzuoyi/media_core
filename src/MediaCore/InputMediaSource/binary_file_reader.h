#ifndef BINARY_FILE_READER_H_
#define BINARY_FILE_READER_H_
#include <future>
#include <fstream>
#include <string>
namespace input {
  class BinaryFileReaderEvent {
  public:
	  BinaryFileReaderEvent();
	  virtual ~BinaryFileReaderEvent();
	  virtual void OnRequestFrameSize(uint64_t* frame_size) = 0;
	  virtual void OnPostBinaryData(const  std::vector<char>& buffer, uint64_t offset) = 0;
	  virtual void OnEOF() = 0;
	  virtual  void OnBOF() = 0;
  };

  class BinaryFileReader {
  public:
    BinaryFileReader();
    virtual ~BinaryFileReader();
    void SetEvent(BinaryFileReaderEvent* event);
    bool Open(const std::string& file_path);
    void Close();
	  bool Start();
	  void Stop();
	  void EnableLoopPlayback(bool enable);
	  void EnableReversePlayback(bool enable);
	  void Speed(double speed);
  private:
	  bool PreReadFrame(uint64_t* read_bytes, uint64_t* frame_size);
	  void PostReadFrame(uint64_t* read_bytes, const std::vector<char>& frame_data);
	  BinaryFileReaderEvent* event_{ nullptr };
	  std::ifstream ifs_;
	  uint64_t file_size_{ 0ULL };
	  bool exit_{ false };
	  std::future<void> read_file_task_;
	  bool enable_loop_playback_{ false };
	  bool enable_reverse_playback_{ false };
	  int interval_{ 40 };
  };
}
#endif // BINARY_FILE_READER_H_
