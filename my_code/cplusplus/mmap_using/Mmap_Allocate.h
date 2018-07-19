#include <string>
#include <map>
#include <cstddef>

void *g_mmap_start = (void*)(0x100A01000000);
long g_mmap_size  = (10*1024*1024);
size_t offset = 1024;

template <typename _Tp>
class MmapAlloc
{
public:
	typedef size_t     size_type;
	typedef ptrdiff_t  difference_type;
	typedef _Tp*       pointer;
	typedef const _Tp* const_pointer;
	typedef _Tp&       reference;
	typedef const _Tp& const_reference;
	typedef _Tp        value_type;

	template<typename _Tp1>
	struct rebind
        { typedef MmapAlloc<_Tp1> other; };
	
	MmapAlloc() throw() { }

	MmapAlloc(const MmapAlloc& __a) throw() { }

	template<typename _Tp1>
 	MmapAlloc(const MmapAlloc<_Tp1>&) throw() { }

	~MmapAlloc() throw() { }
	
	pointer	address(reference __x) const { return &__x; }
	const_pointer address(const_reference __x) const { return &__x; }
 	
	pointer allocate(size_t num, const void *hint = 0)
	{
 		long returnvalue = (long)g_mmap_start + offset;
 		size_t increment = num * sizeof(_Tp) + 8;
		increment -= increment % 8;
 		offset += increment;
 		return (pointer)returnvalue;
 	}

	void deallocate(pointer __p, size_type __n)
 	{
		//if ((void *)0x90200000UL < static_cast<void *>(__p) && static_cast<void *>(__p) < (void *)0xD03000000UL)
		//{
		//	return;
		//}
		::operator delete(__p);
	}

	size_type max_size() const throw()
	{ return size_t(-1) / sizeof(_Tp); }

#if __cplusplus >= 201103L
	template<typename _Up, typename... _Args>
	void
	construct(_Up* __p, _Args&&... __args)
	{ ::new((void *)__p) _Up(std::forward<_Args>(__args)...); }

	template<typename _Up>
	void
	destroy(_Up* __p) { __p->~_Up(); }
#else
	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 402. wrong new expression in [some_] allocator::construct
	void
	construct(pointer __p, const _Tp& __val)
	{ ::new((void *)__p) _Tp(__val); }

	void
	destroy(pointer __p) { __p->~_Tp(); }
#endif
};

template<typename _T1, typename _T2>
inline bool operator==(const MmapAlloc<_T1>&, const MmapAlloc<_T2>&)
{ return true; }

template<typename _T1, typename _T2>
inline bool
operator!=(const MmapAlloc<_T1>&, const MmapAlloc<_T2>&)
{ return false; }

typedef std::basic_string<char, std::char_traits<char>,MmapAlloc<char>> mmapstring;
typedef std::map<mmapstring, mmapstring, std::less<mmapstring>,MmapAlloc<mmapstring> > mmapmap;
