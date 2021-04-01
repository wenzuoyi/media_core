#include <iostream>
#include <future>
#include <mutex>
#include "output_media_source_testsuite.h"
#include "../MediaCoreUtils/include/bound_circle_buffer.hpp"
int main(int argc, const char* argv) {
	output::OutputMediaSourceTestSuite output_media_source_test_suite;
	output_media_source_test_suite.Init();
	output_media_source_test_suite.TestPlayAudioFile();
	int ctrl;
	std::cin >> ctrl;
	output_media_source_test_suite.Fini();
	//int data;
	//utils::BoundedBuffer<int> bounded_buffer(5);
	//std::mutex mutex;
 // for(auto i = 0; i < 5; i++) {
	//  bounded_buffer.PushFront(i);
 // }
 // for (auto i = 0 ; i < 16; i++) {
	//  auto value = 0;
	//  bounded_buffer.PopBack(&value);
	//  std::cout << "index:" << i << ";value:" << value << std::endl;
 // }
  //auto product_task = std::async(std::launch::async, [&bounded_buffer, &mutex]() {
  //  auto count{0};
  //  while (true) {
  //    std::vector<int> temp;
  //    for (auto i = 0; i < 4; i++) {
  //      temp.push_back(count++);
  //    }
  //    {
  //      std::lock_guard<std::mutex> lock_guard(mutex);
  //      for (const auto& item : temp) {
  //        std::cout << "product:" << item << std::endl;
  //      }
  //    }
  //    bounded_buffer.AppendCollection(std::move(temp));
  //    std::this_thread::sleep_for(std::chrono::milliseconds(40));
  //  }
  //});
  //auto consume_task = std::async(std::launch::async, [&bounded_buffer, &mutex]() {
	 // while (true) {
	 //   auto temp = bounded_buffer.RemoveTopItems(4);
  //    {
  //      std::lock_guard<std::mutex> lock_guard(mutex);
  //      for (const auto& item : temp) {
  //        std::cout << "consume:" << item << std::endl;
  //      }
  //    }
		//  std::this_thread::sleep_for(std::chrono::milliseconds(40));		  
	 // }
  //});
  //product_task.wait();
  //consume_task.wait();
	return 0;
}