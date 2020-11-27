#pragma once
#include <mutex>
#include <memory>

template <typename T>
class dual_buffer_shared_ptr
{
private:
	std::shared_ptr<T> ctx_[2];
	int active_index_ = 0;
	std::mutex mutex_;

public:
	dual_buffer_shared_ptr()
	{}

	void load(std::shared_ptr<T> ctx)
	{
		std::lock_guard<std::mutex> guard(this->mutex_);
		int next_index = (!this->active_index_);
		this->ctx_[next_index] = ctx;
		this->active_index_ = next_index;
	}

	const std::shared_ptr<T> get()
	{
		std::lock_guard<std::mutex> guard(this->mutex_);
		return ctx_[active_index_];
	}
};
