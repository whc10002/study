#pragma once
#include <mutex>

template <typename T>
class dual_buffer
{
private:
	T* ctx_[2] = {nullptr};
	int active_index_ = 0;
	std::mutex mutex_;

public:
	dual_buffer()
	{}

	void load(T* ctx)
	{
		if (ctx == nullptr)
			return;
		{
			std::lock_guard<std::mutex> guard(this->mutex_);
			int next_index = (!this->active_index_);
			T* old_ctx = this->ctx_[next_index];
			if (old_ctx)
				delete static_cast<T*>(old_ctx);
			this->ctx_[next_index] = ctx;
			this->active_index_ = next_index;
		}
	}

	const T* get()
	{
		std::lock_guard<std::mutex> guard(this->mutex_);
		return ctx_[active_index_];
	}

	~dual_buffer()
	{
		for (int i = 0; i < 2; i++)
		{
			if (this->ctx_[i])
				delete static_cast<T*>(this->ctx_[i]);
		}
	}
};
