#include "./include/async_workflow.h"
#include <thread>
#include <Poco/ThreadPool.h>
namespace utils {
	AsyncWorkflow::AsyncWorkflow() = default;

	AsyncWorkflow::~AsyncWorkflow() = default;

  void AsyncWorkflow::AsyncStart() {
    reference_count_ = 0;
    exit_ = false;
    RequestAsyncTask();
  }

  void AsyncWorkflow::AsyncStop() {
    exit_ = true;
    while (reference_count_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
  }

  void AsyncWorkflow::Terminate() {
	  exit_ = true;
  }

  void AsyncWorkflow::run() {
    if (exit_) {
      --reference_count_;
      return;
    }
    AsyncExecute();
    RequestAsyncTask();
    --reference_count_;
  }

  void AsyncWorkflow::RequestAsyncTask() {
    ++reference_count_;
    Poco::ThreadPool::defaultPool().start(*this);
  }
}
