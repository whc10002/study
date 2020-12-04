#ifndef __TIMEOUT_CACHE__
#define __TIMEOUT_CACHE__

#include <chrono>
#include <unordered_map>
#include "RWLock.h"

template<class Key, class Value>
class Timeout_Cache
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef std::unordered_map<key_type, node_type> map_type;

	static const size_t default_capacity_ = 100 * 1024;
	static const size_t default_timeout_ = 3600;
public:
	Timeout_Cache(size_t capacity = default_capacity_)
		: capacity_(capacity)
	{
	}

	~Timeout_Cache()
	{
	}

	bool get(const key_type &key, value_type &value)
	{
		RWGuard guard(rw_lock_, RWGuard::READ_LOCK);
		auto itr = cache_data_.find(key);
		if (itr == cache_data_.end())
			return false;
		if (is_expired(itr->second))
			return false;
		value = itr->second;
		return true;
	}

	bool set(const key_type &key, const value_type &value, size_t ttl = default_timeout_, bool overwrite = false)
	{
		RWGuard guard(rw_lock_, RWGuard::WRITE_LOCK);
	}
private:
	bool is_expired(const node_type &node)
	{
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) >= node.expired_time_;
	}
private:
	struct Cache_Node 
	{
		value_type value;
		time_t expired_time_;
	}node_type;

	size_t capacity_;
	map_type cache_data_;
	mutable RWLock rw_lock_;
};

#endif // __TIMEOUT_CACHE__
