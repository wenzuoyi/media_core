//Note:here, if your compiler supports C++14 or higher, you'd better to use mutex provide by system(just like std::sharedmutex), or you can use the class below.
#ifndef SHARED_MUTEX_H_
#define SHARED_MUTEX_H_
#include <memory>
#include "global_media_utils_source.h"
namespace utils {
	class SharedMutex;
	using SharedMutexPtr = std::shared_ptr<SharedMutex>;
  class MEDIA_CORE_UTILS_API_HEADER SharedMutex {
  public:
    SharedMutex();
    virtual ~SharedMutex();
    static SharedMutexPtr CreateInstance();
	  virtual void TryWriteLock() = 0;
	  virtual void WriteUnlock() = 0;
	  virtual void TryReadLock() = 0;
	  virtual void ReadUnlock() = 0;
  };

  class WriteLock final {
  public:
    explicit WriteLock(SharedMutexPtr mutex) : mutex_(mutex) {
      if (mutex_ != nullptr) {
        mutex_->TryWriteLock();
      }
    }

    ~WriteLock() {
      if (mutex_ != nullptr) {
        mutex_->WriteUnlock();
      }
    }

  private:
    SharedMutexPtr mutex_{nullptr};
  };

  class ReadLock final {
  public:
    explicit ReadLock(SharedMutexPtr mutex) : mutex_(mutex) {
      if (mutex_ != nullptr) {
        mutex_->TryReadLock();
      }
    }

    ~ReadLock() {
      if (mutex_ != nullptr) {
        mutex_->ReadUnlock();
      }
    }

  private:
    SharedMutexPtr mutex_{nullptr};
  };
}
#endif // SHARED_MUTEX_H_
