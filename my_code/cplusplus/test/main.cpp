#include "recursor.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
bool
value_from_string(const std::string& str, T& value)
{
	if (str.empty())
		return false;

	value = static_cast<T>(str);
	return true;
}

int main(int argc, char *argv[])
{
	recursor r;
	cout << r.sum(4) << endl;

	vector<double> arr;
	arr.push_back(2);
	arr.push_back(1.009);
	arr.push_back(3.0009);
	arr.push_back(3.0);
	arr.push_back(0.0);

	int num = arr.size();
	void *p = static_cast<void*>(arr.data());
	
	double *a = static_cast<double*>(p);
	for (int i = 0; i < num; i++)
	{
		cout << a[i] << endl;
	}
	cout << r.sum(4) << endl;

	std::string value;
	value_from_string<std::string>((const char*)nullptr, value);
	return 0;
}
