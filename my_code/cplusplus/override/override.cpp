#include <iostream>
#include <string>
#include <vector>

using namespace std;

class B
{
	public:
		virtual void f(int)
		{
			std::cout << "B::f()" << std::endl;
		}
		
		virtual void function(int)
		{
			std:cout << "D::function()" << std::endl;
		}
};

class D: public B
{
	public:
		// override.cpp:: error: 'f' marked 'override' but does not override any member functions
		//                 virtual void f(short) override
		//                                         ^
		// because the signature differ, one takes a short, one takes an int, so it's overload not an override
		/*virtual void f(short) override
		{
			std::cout << "D::f()" << std::endl;
		}*/

		virtual void f(int) override
		{
			std::cout << "D::f()" << std::endl;
		}

		virtual void function(int) override final
		{
			std:cout << "D::function()" << std::endl;
		}
};

class F : public D
{
	public:
		// override.cpp:: error: declaration of 'function' overrides a 'final' function
		//                 virtual void function(int)
		/*virtual void function(int)
		{

			std:cout << "F::function()" << std::endl;
		}*/
};

int main(int argc, char **agrv)
{
	B b;
	b.f(1);

	D d;
	B *p = dynamic_cast<B*>(&d);
	p->f(2);
	return 1;
}
