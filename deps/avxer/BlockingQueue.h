#ifndef CRAZY_NET_BLOCKING_QUEUE_H
#define CRAZY_NET_BLOCKING_QUEUE_H

#include "internal/common.h"
#include <list>
#include <thread>
#include <numeric>
#include <chrono>
#include <algorithm>
#include <cassert>
#include <condition_variable>
#include <boost/utility.hpp>

NS_AVXER_BEGIN

template<typename ElemType>
class BlockingQueue : public boost::noncopyable {
public:
  BlockingQueue(uint32_t capacity = (std::numeric_limits<uint32_t>::max)())
      : max_capacity_(capacity) {
  }

  template<typename OutputIterator>
  int DrainTo(OutputIterator iter) {
    std::lock_guard<std::mutex> guard(mutex_);
    return DrainToWithLock(iter, elements_.size());
  }

  template<typename OutputIterator>
  int DrainTo(OutputIterator iter, uint32_t max_elements) {
    assert(max_elements > 0);
    std::lock_guard<std::mutex> guard(mutex_);
    return DrainToWithLock(iter, (std::min)(elements_.size(), max_elements));
  }

  /**
   * Inserts the specified element into this queue if it is possible to do so
   * immediately without violating capacity restrictions, returning true upon
   * success and false if no space is currently available.
   */
  bool Offer(const ElemType &elem) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (elements_.size() < max_capacity_) {
      const bool is_empty = elements_.empty();
      elements_.push_back(elem);
      if (is_empty) {
        not_empty_.notify_one();
      }
      return true;
    } else {
      return false;
    }
  }

  /**
   * Inserts the specified element into this queue, waiting up to the specified
   * wait time if necessary for space to become available.
   */
  bool Offer(const ElemType &elem, long timeout) {
    std::unique_lock<std::mutex> guard(mutex_);
    bool result = not_full_.wait_for(guard, std::chrono::milliseconds(timeout),
      [&]() ->bool {
      return elements_.size() < max_capacity_;
    });
    if (result) {
      const bool is_empty = elements_.empty();
      elements_.push_back(elem);
      if (is_empty) {
        not_empty_.notify_one();
      }
    }
    return result;
  }


  /**
  * Inserts the specified element into this queue, waiting if necessary for
  * space to become available.
  */
  void Put(const ElemType &elem) {
    std::unique_lock<std::mutex> guard(mutex_);
    not_full_.wait(guard, [&]() -> bool {
      return elements_.size() < max_capacity_;
    });
    const bool is_empty = elements_.empty();
    elements_.push_back(elem);
    if (is_empty) {
      not_empty_.notify_one();
    }
  }

  void Put(ElemType &&elem) {
    std::unique_lock<std::mutex> guard(mutex_);
    not_full_.wait(guard, [&]() -> bool {
      return elements_.size() < max_capacity_;
    });
    const bool is_empty = elements_.empty();
    elements_.push_back(std::move(elem));
    if (is_empty) {
      not_empty_.notify_one();
    }
  }


  /**
   * Retrieves and removes the head of this queue, waiting up to the specified
   * wait time if necessary for an element to become available.
   */
  bool Poll(ElemType &elem, long timeout) {
    std::unique_lock<std::mutex> guard(mutex_);
    bool result = not_empty_.wait_for(guard, std::chrono::milliseconds(timeout),
      [&]() -> bool {
      return !elements_.empty();
    });
    if (result) {
      const bool is_full = elements_.size() == max_capacity_;
      elem = elements_.front();
      elements_.pop_front();
      if (is_full) {
        not_full_.notify_one();
      }
    }
    return result;
  }

  /**
  * Retrieves and removes the head of this queue, waiting if necessary until
  * an element becomes available.
  */
  ElemType Take() {
    std::unique_lock<std::mutex> guard(mutex_);
    not_empty_.wait(guard, [&]() -> bool {
      return !elements_.empty();
    });
    const bool is_full = elements_.size() == max_capacity_;
    ElemType elem = elements_.front();
    elements_.pop_front();
    if (is_full) {
      not_full_.notify_one();
    }
    return elem;
  }

  /**
   * Returns the number of additional elements that this queue can ideally (in
   * the absence of memory or resource constraints) accept without blocking,
   * or Integer.MAX_VALUE if there is no intrinsic limit.
   */
  int RemainingCapacity() {
    return -1;
  }

private:
  template<typename OutputIterator>
  int DrainToWithLock(OutputIterator iter, uint32_t count) {
    assert(count <= elements_.size());
    if (!elements_.empty()) {
      const bool is_full = elements_.size() == max_capacity_;
      std::copy_n(elements_.begin(), count, iter);
      auto iter_end = elements_.begin();
      std::advance(iter_end, count);
      elements_.erase(elements_.begin(), iter_end);
      if (is_full) {
        not_full_.notify_one();
      }
      return count;
    } else {
      return 0;
    }
  }

private:
  std::list<ElemType> elements_;
  const uint32_t max_capacity_;
  std::mutex mutex_;
  std::condition_variable not_full_;
  std::condition_variable not_empty_;
};

NS_AVXER_END

#endif /* CRAZY_NET_BLOCKING_QUEUE_H */
