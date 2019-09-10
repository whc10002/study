#include <tuple>
#include <map>
#include <vector>
#include <complex>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	std::map<int, std::string> m;

	m.emplace(std::piecewise_construct,
			std::forward_as_tuple(10),
			std::forward_as_tuple(20, 'a'));

	for (const auto& kv : m)
	{
		cout << "key:" << kv.first << "\tvalue:" << kv.second << endl;
	}

	map<string, complex<double>> scp;
	// scp.emplace("hello", 1, 2);
	// 	will compile fail, can't disguise which is value and which is key
	
	// pair<string, complex<double>> scp(make_tuple("hello"), make_tuple(1, 2));
	// 	will compile fail because the first make_tuple was treated as key,
	// 	the second make_tuple was treated as value which was mismatch type.

	// piecewise_construct: global variable of piecewise_construct_t	
	scp.emplace(piecewise_construct,
			forward_as_tuple("hello"),
			forward_as_tuple(1, 2));
	for (const auto& kv : scp)
	{
		cout << "key:" << kv.first << "\tvalue:" << kv.second << endl;
	}
	return 0;
}
