#ifndef SHARED_MUTEX_IMPL_H_
#define SHARED_MUTEX_IMPL_H_
#include "./include/shared_mutex.h"
#include <condition_variable>
#include <mutex>
namespace utils {
  class SharedMutexImpl : public SharedMutex , public std::mutex {
  public:
    SharedMutexImpl();
    virtual ~SharedMutexImpl();
  protected:
    void TryWriteLock() override;
    void WriteUnlock() override;
    void TryReadLock() override;
    void ReadUnlock() override;
  private:
	  std::condition_variable condition_;
    bool write_flag_{false};
    int read_count_{0};
  };
}
#endif // SHARED_MUTEX_IMPL_H_
