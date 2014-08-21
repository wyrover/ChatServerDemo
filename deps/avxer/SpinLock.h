#ifndef CRAZY_NET_SPIN_LOCK_H
#define CRAZY_NET_SPIN_LOCK_H

#include "avxerlib.h"
#include <atomic>
#include <boost/utility.hpp>

NS_AVXER_BEGIN

class SpinLock : private boost::noncopyable {

public:
	SpinLock() {
		lock_state_.clear();
	}

	void lock() {
		while (lock_state_.test_and_set());
	}

	void unlock() {
		lock_state_.clear();
	}

	bool try_lock() {
		return !lock_state_.test_and_set();
	}

private:
	std::atomic_flag lock_state_;
};

NS_AVXER_END

#endif /* CRAZY_NET_SPIN_LOCK_H */
