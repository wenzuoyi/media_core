#ifndef ASYNC_WORKFLOW_H_
#define ASYNC_WORKFLOW_H_
#include <atomic>
#include <Poco/Runnable.h>
#include "global_media_utils_source.h"
namespace utils {
  class MEDIA_CORE_UTILS_API_HEADER AsyncWorkflow : public Poco::Runnable {
  public:
    AsyncWorkflow();
    virtual ~AsyncWorkflow();
  protected:
	  void AsyncStart();
	  virtual void AsyncStop();
	  virtual void Terminate();
	  virtual void AsyncExecute() = 0;
  private:
    void run() override final;
    void RequestAsyncTask();
    std::atomic_int reference_count_{0};
    std::atomic_bool exit_{false};
  };
}
#endif // ASYNC_WORKFLOW_H_
