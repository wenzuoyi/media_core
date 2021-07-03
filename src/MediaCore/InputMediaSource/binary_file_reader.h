#ifndef BINARY_FILE_READER_H_
#define BINARY_FILE_READER_H_
#include <future>
#include <fstream>
#include <string>
#include "async_workflow.h"
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

  class BinaryFileReader : public utils::AsyncWorkflow {
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
  protected:
    void AsyncExecute() override;
  private:
	  bool PreReadFrame(uint64_t* frame_size);
	  void PostReadFrame(const std::vector<char>& frame_data);
	  BinaryFileReaderEvent* event_{ nullptr };
	  std::ifstream ifs_;
	  uint64_t file_size_{ 0ULL };
	  bool enable_loop_playback_{ false };
	  bool enable_reverse_playback_{ false };
	  int interval_{ 40 };
	  uint64_t read_bytes_{ 0ULL };
  };
}
#endif // BINARY_FILE_READER_H_
