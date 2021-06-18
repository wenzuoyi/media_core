#include "shared_mutex_impl.h"
#include <chrono>
using namespace std::chrono_literals;

namespace utils {
	SharedMutexImpl::SharedMutexImpl() = default;

  SharedMutexImpl::~SharedMutexImpl() = default;

  void SharedMutexImpl::TryWriteLock() {
    std::unique_lock<std::mutex> lock(*this);
    condition_.wait(lock, [this]() {
      return read_count_ == 0 && !write_flag_;
    });
    write_flag_ = true;
  }

  void SharedMutexImpl::WriteUnlock() {
	  std::unique_lock<std::mutex> lock(*this);
	  write_flag_ = false;
	  condition_.notify_all();
  }

  void SharedMutexImpl::TryReadLock() {
    std::unique_lock<std::mutex> lock(*this);
    condition_.wait(lock, [this]() {
      return !write_flag_;
    });
    ++read_count_;
  }

  void SharedMutexImpl::ReadUnlock() {
    std::unique_lock<std::mutex> lock(*this);
    --read_count_;
    condition_.notify_all();
    if (read_count_ == 0) {
      condition_.notify_all();
    }
  }
}
