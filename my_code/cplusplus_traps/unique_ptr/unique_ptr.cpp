#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <utility>
#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>

using namespace std;

struct A {
	unique_ptr<size_t> max_count_;
};

class Test
{
	public:
		unique_ptr<A> a_;
};

int main(int argc, char* argv[])
{
	unique_ptr<Test> test;
	test.reset(new Test);
	if (test->a_)
		cout << "fuck" << endl;
	return 0;
}
