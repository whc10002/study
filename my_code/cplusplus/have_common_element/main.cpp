#include <set>
#include <iostream>

#include "have_common_element.h"

using namespace std;

int main(int argc, char *argv[])
{
	set<int> a{1, 4, 5, 6};
	set<int> b{4, 5, 6, 7, 8};

	cout << have_common_element<set<int>>(a, b) << endl;
	return 0;
}

