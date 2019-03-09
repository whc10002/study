#include <iostream>
#include <thread>
using namespace std;
void LaunchRocket()
{
	cout << "Launching Rocket" << endl;
}
int main()
{
	thread t1(LaunchRocket);
	//thread *t1 = new thread(LaunchRocket);
	//t1.join(); // somehow we forgot to join this to main thread - will cause a crash.
	return 0;
}
