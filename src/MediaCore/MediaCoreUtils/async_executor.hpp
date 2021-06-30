#ifndef ASYNC_EXECUTOR_HPP_
#define ASYNC_EXECUTOR_HPP_
#include <atomic>
#include <thread>
#include <Poco/ThreadPool.h>
#include <Poco/Runnable.h>
namespace utils {
		template <class T>
		class AsyncExecutor : public Poco::Runnable {
		public:
			AsyncExecutor() {
			}

			virtual ~AsyncExecutor() {			  
			}
		protected:
      virtual void Start() {
        reference_count_ = 0;
        exit_ = false;
        RequestAsyncTask();
      }

      virtual void Stop() {
        exit_ = true;
        while (reference_count_ > 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
      }

      void run() override final {
        if (exit_) {
          --reference_count_;
          return;
        }
        AsyncExecute();
        RequestAsyncTask();
        --reference_count_;
      }

	  virtual void AsyncExecute() = 0;
		private:
      void RequestAsyncTask() {
        ++reference_count_;
        Poco::ThreadPool::defaultPool().start(*this);
      }
			std::atomic_int reference_count_{ 0 };
			std::atomic_bool exit_{ false };
		};
}
#endif // ASYNC_EXECUTOR_HPP_
