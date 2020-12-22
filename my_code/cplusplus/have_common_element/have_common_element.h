#ifndef __HAVE_COMMON_ELEMENT_H_
#define __HAVE_COMMON_ELEMENT_H_

#include<set>
#include<algorithm>
#include<iostream>

template <class Container>
struct common_inserter : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	common_inserter& operator*() { return *this; }
	common_inserter& operator++() { return *this; }
	common_inserter& operator = (const typename Container::value_type& value)
	{
		throw true;
	}
	common_inserter(const Container &res) {}
};

template <class Container>
bool have_common_element(const Container &a, const Container &b)
{
	try{
		Container res;
		std::set_intersection(a.begin(), a.end(),
                	 b.begin(), b.end(),
                	 common_inserter<Container>(res));
	} catch(...) {
		return true;
	}
	return false;
}

template <class I1, class I2>
bool have_common_element(I1 first1, I1 last1, I2 first2, I2 last2)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else
			return true;
	}
	return false;
}

#endif //__HAVE_COMMON_ELEMENT_H_
