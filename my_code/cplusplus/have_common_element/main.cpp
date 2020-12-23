#include <set>
#include <iostream>
#include <string>

#include "have_common_element.h"

using namespace std;

int main(int argc, char *argv[])
{
	set<int> a{1, 4, 5, 6};
	set<int> b{4, 5, 6, 7, 8};

	cout << have_common_element_hash<set<int>>(a, b) << endl;

	{
		set<string> c{"hello", "world1"};
		set<string> d{"fuck", "the", "world"};

		cout << have_common_element<set<string>, set<string>>(c, d) << endl;
	}
	{
		set<string> c{"hello", "world"};
		set<string> d{"fuck", "the", "world"};

		cout << have_common_element<set<string>, set<string>>(c, d) << endl;
	}
	{
		set<string> c{"hello","", "world1"};
		set<string> d{"fuck", "","the", "world"};

		cout << have_common_element<set<string>, set<string>>(c, d) << endl;
	}
	return 0;
}

