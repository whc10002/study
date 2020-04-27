#include <iostream>
#include <sstream>
#include <string>

using namespace std;

bool is_prime_number(int n)
{
	if (n < 2)
		return false;
	if (n == 2)
		return true;
	for (int i = 2; i < n; i++)
		if (n % i == 0)
			return false;
	return true;
}

void get_prime_str(int max_len, stringstream& ss)
{
	ss.clear();
	if (max_len <= 0)
		return;
	for (int i = 2; ss.str().length() <= max_len; i++)
	{
		if (is_prime_number(i))
			ss << i;
	}
}

int main(int argc, char* argv[])
{
	stringstream ss("");
	get_prime_str(6, ss);
	cout << ss.str().substr(0, 5) << endl;
		
	get_prime_str(1006, ss);
	cout << ss.str().substr(1000, 5) << endl;
}
