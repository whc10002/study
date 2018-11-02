#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

#define MAX_LOOP (1000000)

class Base
{
	public:
		int initialize(int a) const
		{
			static const string OPENRTB_APP_NAME("appname");
			return a;
		}
};

int main(int argc, char** argv)
{
	Base *a = new Base();
	int i = 0;
	for (; i < MAX_LOOP; i++)
	{
		a->initialize(1);
	}

	while (true)
	{
		sleep(1);
	}
	return 0;
}
