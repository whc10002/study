#include <iostream>
#include <memory>

using namespace std;
static unique_ptr<int> p1 = std::make_unique<int>(10);

// move -> copy construction -> failed
unique_ptr<int> foo()
{
	auto p = make_unique<int>(10);
	return p;                   // 1 ok p is local parameter(rvalue) can run it with move
	//return move( p );         // 2 also ok
	//return p1;                // NOK. p1 is lvalue can not performance move on it.
}

int main()
{
	unique_ptr<int> p = foo();
	
	cout << *p << endl;
	return 0;
}
