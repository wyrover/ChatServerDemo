#ifndef CHAT_SERVER_ROBIN_ID_H
#define CHAT_SERVER_ROBIN_ID_H

#include <cstdint>
#include <cassert>

class robin_id {
public:
  /**
   * Generate id in [min, max)
   */
  robin_id(uint64_t min, uint64_t max) : min_(min), max_(max), next_id_(min) {
    assert(min < max);
  }

  uint64_t next_id() {
    uint64_t id = next_id_;
    if (++next_id_ == max_) {
      next_id_ = min_;
    }
    return id;
  }

  /**
   * @return the count of different id in the range
   */
  uint64_t count() const {
    return max_ - min_;
  }

private:
  const uint64_t min_, max_;
  uint64_t next_id_;
};

#endif /* CHAT_SERVER_ROBIN_ID_H */
