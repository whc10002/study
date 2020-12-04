#pragma once

#include <mutex>
#include <condition_variable>

#ifdef USE_RW_LOCK_C11
class RWLock
{
public:
	RWLock()
		: status_(0), waiting_readers_(0), waiting_writers_(0)
	{
	}

	RWLock(const RWLock&) = delete;
	RWLock(RWLock&&) = delete;
	RWLock& operator = (const RWLock&) = delete;
	RWLock& operator = (RWLock&&) = delete;
	
	void rdlock()
	{
		std::unique_lock<std::mutex> lck(mutex_);
		waiting_readers_ += 1;
		read_cv_.wait(lck, [&]() { return waiting_writers_ == 0 && status_ >= 0; });
		waiting_readers_ -= 1;
		status_ += 1;
	}
	
	void wrlock()
	{
		std::unique_lock<std::mutex> lck(mutex_);
		waiting_writers_ += 1;
		write_cv_.wait(lck, [&]() { return status_ == 0; });
		waiting_writers_ -= 1;
		status_ = -1;
	}
	
	void unlock()
	{
		std::unique_lock<std::mutex> lck(mutex_);
		if (status_ == -1)
		{
			status_ = 0;
		}
		else
		{
			status_ -= 1;
		}
		if (waiting_writers_ > 0)
		{
			if (status_ == 0)
			{
				write_cv_.notify_one();
			}
		}
		else
		{
			read_cv_.notify_all();
		}
	}

private:
    // -1    : one writer
    // 0     : no reader and no writer
    // n > 0 : n reader
    int32_t status_;
    int32_t waiting_readers_;
    int32_t waiting_writers_;
    std::mutex mutex_;
    std::condition_variable read_cv_;
    std::condition_variable write_cv_;
};
#else
class RWLock
{
private:
	pthread_rwlock_t lock_;
	//pthread_rwlockattr_t attr_;
public:
	RWLock()
	{
		pthread_rwlock_init(&lock_, nullptr);
	}
	virtual ~RWLock()
	{
		pthread_rwlock_destroy(&lock_);
	}
	RWLock(const RWLock&) = delete;
	RWLock& operator=(const RWLock&) = delete;
	RWLock(RWLock&&) = delete;
	RWLock& operator=(RWLock&&) = delete;

	int rdlock()
	{
		return pthread_rwlock_rdlock(&lock_);
	}
	int wrlock()
	{
		return pthread_rwlock_wrlock(&lock_);
	}
	int unlock()
	{
		return pthread_rwlock_unlock(&lock_);
	}
};
#endif

class RWGuard
{
public:
	enum LOCK_TYPE
	{
		READ_LOCK,
		WRITE_LOCK
	};
private:
	RWLock& lock_;
	LOCK_TYPE lock_type_;
public:
	RWGuard(RWLock& lock, LOCK_TYPE lock_type) : lock_(lock), lock_type_(lock_type)
	{
		if (lock_type_ == READ_LOCK)
			lock_.rdlock();
		else
			lock_.wrlock();
	}
	~RWGuard()
	{
		lock_.unlock();
	}
	RWGuard(const RW_Guard&) = delete;
	RWGuard& operator=(const RW_Guard&) = delete;
	RWGuard(const RW_Guard&&) = delete;
	RWGuard& operator=(const RW_Guard&&) = delete;
};
