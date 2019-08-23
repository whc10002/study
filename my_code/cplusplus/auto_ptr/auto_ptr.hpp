#ifndef __AUTO_PTR_HPP__
#define __AUTO_PTR_HPP__
namespace ark
{
template <class T>
class auto_ptr {
	private:
		T* pr_;

	public:
		explicit auto_ptr(T* p) throw()
			: pr_(p)
		{
		}

		~auto_ptr() throw()
		{
			if (pr_)
				delete pr_;
		}

		auto_ptr(auto_ptr& rhs) throw()
			: pr_(rhs.release())
		{
		}

		template <class Y>
		auto_ptr(auto_ptr<Y>& rhs) noexcept
			: pr_(rhs.release())
		{
		}

		auto_ptr& operator= (auto_ptr& rhs) noexcept
		{
			reset(rhs.release());
			return *this;
		}

		template <class Y>
		auto_ptr<Y>& operator= (auto_ptr<Y> rhs) noexcept
		{
			reset(rhs.release());
			return *this;
		}

		T& operator* () const noexcept
		{
			return *pr_;
		}

		T* operator-> () const noexcept
		{
			return pr_;
		}

		// helper function 
		T* get() const noexcept
		{
			return pr_;
		}

		// Release ownership
		T* release() noexcept
		{
			T* tmp = pr_;
			pr_ = nullptr;
			return tmp;
		}

		// Reset
		auto_ptr& reset(T* pr = nullptr) noexcept
		{
			if (pr_ != pr)
			{
				if (pr_)
					delete pr_;
				pr_ = pr;
			}
			return *this;
		}
};	
}

#endif //__AUTO_PTR_HPP__
