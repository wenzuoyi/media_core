#include "./include/shared_mutex.h"
#include "shared_mutex_impl.h"
namespace utils {
  SharedMutex::SharedMutex() = default;

  SharedMutex::~SharedMutex() = default;

  SharedMutexPtr SharedMutex::CreateInstance() {
	  return std::make_shared<SharedMutexImpl>();
  }
}
