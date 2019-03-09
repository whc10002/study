// equal_range example
// equal_range example
#include <iostream>     // std::cout
#include <algorithm>    // std::equal_range, std::sort
#include <vector>       // std::vector
#include <map>          // std::vector

bool mygreater (int i,int j) { return (i>j); }

int main () {
	int myints[] = {10,20,30,30,20,10,10,20};
	std::vector<int> v(myints,myints+8);                         // 10 20 30 30 20 10 10 20
	std::pair<std::vector<int>::iterator,std::vector<int>::iterator> bounds;

	// using default comparison:
	std::sort (v.begin(), v.end());                              // 10 10 10 20 20 20 30 30
	bounds=std::equal_range (v.begin(), v.end(), 20);            //          ^        ^

	// using "mygreater" as comp:
	std::sort (v.begin(), v.end(), mygreater);                   // 30 30 20 20 20 10 10 10
	bounds=std::equal_range (v.begin(), v.end(), 20, mygreater); //       ^        ^

	std::cout << "bounds at positions " << (bounds.first - v.begin());
	std::cout << " and " << (bounds.second - v.begin()) << '\n';

	bounds = std::equal_range(v.begin(), v.end(), 40, mygreater);
	std::cout << "bounds at positions " << (bounds.first - bounds.second) << '\n';

	std::multimap<char,int> mymm;

	mymm.insert(std::pair<char,int>('a',10));
	mymm.insert(std::pair<char,int>('b',20));
	mymm.insert(std::pair<char,int>('b',30));
	mymm.insert(std::pair<char,int>('b',40));
	mymm.insert(std::pair<char,int>('c',50));
	mymm.insert(std::pair<char,int>('c',60));
	mymm.insert(std::pair<char,int>('d',60));

	std::cout << "mymm contains:\n";
	for (char ch='e'; ch<='e'; ch++)
	{
		std::pair <std::multimap<char,int>::iterator, std::multimap<char,int>::iterator> ret;
		ret = mymm.equal_range(ch);
		if (ret.first == ret.second)
		{
			std::cout << "can't find" << '\n';
		}
		std::cout << ch << " =>";
		for (std::multimap<char,int>::iterator it=ret.first; it!=ret.second; ++it)
			std::cout << ' ' << it->second;
		std::cout << '\n';
	}	
	return 0;
}
