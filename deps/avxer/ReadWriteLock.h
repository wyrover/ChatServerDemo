#ifndef CRAZY_NET_READ_WRITE_LOCK_H
#define CRAZY_NET_READ_WRITE_LOCK_H

#include "avxerlib.h"
#include <atomic>
#include <cassert>
#include <boost/utility.hpp>

NS_AVXER_BEGIN

class ReadWriteLock : private boost::noncopyable {
public:
	ReadWriteLock() {
		lock_state_ = 0;
	}

	bool TryLockRead() {
		int32_t old_value = lock_state_;
		if (old_value != -1) {
			return lock_state_.compare_exchange_strong(old_value, old_value + 1);
		} else {
			return false;
		}
	}

	void LockRead() {
		while (!TryLockRead());
	}

	void UnlockRead() {
		assert(lock_state_ > 0);
		--lock_state_;
	}

	bool TryLockWrite() {
		int32_t expect_value = 0;
		return lock_state_.compare_exchange_strong(expect_value, -1);
	}

	void LockWrite() {
		while (!TryLockWrite());
	}

	void UnlockWrite() {
		assert(-1 == lock_state_);
		lock_state_ = 0;
	}

private:
	// lock_state_ == -1: an thread owns write lock
	// lock_state_ == 0: no threads owns read lock and write lock
	// lock_state_ > 0: lock_state_ threads owns read lock
	std::atomic_int32_t lock_state_;
};


class ReadLockGuard {
public:
	ReadLockGuard(ReadWriteLock &rw_lock) : rw_lock_(rw_lock) {
		rw_lock_.LockRead();
	}

	~ReadLockGuard() {
		rw_lock_.UnlockRead();
	}

private:
	ReadWriteLock &rw_lock_;
};


class WriteLockGuard {
public:
	WriteLockGuard(ReadWriteLock &rw_lock) : rw_lock_(rw_lock) {
		rw_lock_.LockWrite();
	}

	~WriteLockGuard() {
		rw_lock_.UnlockWrite();
	}

private:
	ReadWriteLock &rw_lock_;
};

NS_AVXER_END

#endif /* CRAZY_NET_READ_WRITE_LOCK_H */
