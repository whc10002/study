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
	RWLock(const RWLock&&) = delete;
	RWLock& operator=(const RWLock&&) = delete;

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
