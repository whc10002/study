#ifndef __HAVE_COMMON_ELEMENT_H_
#define __HAVE_COMMON_ELEMENT_H_

#include<set>
#include<algorithm>
#include<iostream>
#include<unordered_map>

template <class Container>
struct common_inserter : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	common_inserter& operator*() { return *this; }
	common_inserter& operator++() { return *this; }
	common_inserter& operator = (const typename Container::value_type &value)
	{
		throw true;
	}
	common_inserter(const Container &res) {}
};

template <typename T>
struct Functional_Inserter : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	Functional_Inserter& operator*() { return *this; }
	Functional_Inserter& operator++() { return *this; }
	Functional_Inserter& operator = (const T& item)
	{
		insert_func_(item);
		return *this;
	}

	template<typename F> Functional_Inserter(const F& f) : insert_func_(f) {}
	std::function<void(const T&)> insert_func_;
};

template<typename Set1, typename Set2>
bool set_is_intersected(const Set1& s1, const Set2& s2)
{
	typedef typename Set1::value_type Value_Type;
	try {
		std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
				Functional_Inserter<Value_Type>(
					[&](const Value_Type&) { throw true; }
				));
	} catch (bool res) {
		return res;
	}
	return false;
}

template <class Container>
bool have_common_element_my(const Container &a, const Container &b)
{
	typedef typename Container::value_type Value_Type;
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

template <class Container>
bool have_common_element_hash(const Container &a, const Container &b)
{
	if (a.empty() || b.empty()) return false;
	const Container &tmp = a.size() >= b.size() ? b : a;
	const Container &it = a.size() >= b.size() ? a : b;

	typedef typename Container::value_type Value_Type;
	std::unordered_map<Value_Type, bool> hash_set;
	for (const Value_Type &i : tmp)
		hash_set[i] = true;
	for (const Value_Type &i : it)
	{
		if (hash_set.find(i) != hash_set.end())
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

template <class Set1, class Set2>
bool have_common_element(Set1 &s1, Set2 &s2)
{
	typename Set1::iterator first1 = s1.begin();
	typename Set1::iterator first2 = s2.begin();
        while (first1 != s1.end() && first2 != s2.end())
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
