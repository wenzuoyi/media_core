#ifndef BOUND_CIRCLE_BUFFER_HPP_
#define BOUND_CIRCLE_BUFFER_HPP_
#include <vector>
#include <mutex>
#include <condition_variable>
#include <boost/circular_buffer.hpp>
#include <boost/call_traits.hpp>

namespace utils {
  template <class T>
  class BoundedBuffer : public std::mutex {
  public:
	  using ContainerType = boost::circular_buffer<T>;
	  using SizeType = typename ContainerType::size_type;
	  using ValueType = typename ContainerType::value_type;
	  using ParamType = typename boost::call_traits<ValueType>::param_type;
	  using ParamTypeCollection = std::vector<ValueType>;
      
    explicit BoundedBuffer(SizeType capacity) : unread_(0), container_(capacity) {
    }

    BoundedBuffer(const BoundedBuffer&) = delete;
    BoundedBuffer& operator =(const BoundedBuffer&) = delete;

    void PushFront(ParamType item) {
      std::unique_lock<std::mutex> lock(*this);
      not_full_.wait(lock, [this]() {
        return unread_ < container_.capacity();
      });
      container_.push_front(item);      
      ++unread_;
      lock.unlock();
      not_empty_.notify_one();
    }

    void AppendCollection(ParamTypeCollection&& collection) {
      std::unique_lock<std::mutex> lock(*this);
      not_full_.wait(lock, [collection, this]() {
        return unread_ + collection.size() < container_.capacity();
      });
      container_.insert(container_.end(), collection.begin(), collection.end());
      unread_ += collection.size();
      lock.unlock();
      not_empty_.notify_one();
    }

    ValueType PopBack() {
      std::unique_lock<std::mutex> lock(*this);
      not_empty_.wait(lock, [this]() {
        return unread_ > 0;
      });
      auto item = container_.back();
      container_.pop_back();
      --unread_;
      lock.unlock();
      not_full_.notify_one();
      return item;
    }

    ParamTypeCollection RemoveTopItems(SizeType size) {
      std::unique_lock<std::mutex> lock(*this);
      not_empty_.wait(lock, [size, this]() {
        return unread_ >= size;
      });
      auto removed_collection = ParamTypeCollection(container_.begin(), container_.begin() + size);
      container_.erase(container_.begin(), container_.begin() + size);
      unread_ -= size;
      lock.unlock();
      not_full_.notify_one();
      return removed_collection;
    }

    void Clear() {
      std::unique_lock<std::mutex> lock(*this);
      container_.clear();
      unread_ = 0;
    }
  private:
    SizeType unread_;
    ContainerType container_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
  };
}
#endif // BOUND_CIRCLE_BUFFER_HPP_
