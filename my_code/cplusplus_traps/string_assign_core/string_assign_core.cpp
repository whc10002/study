#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include<string>

// std::string operator= will invoke std::string funciton:assign
// what happend when assign was called:
// 1. delete the old data region
// 2. new a new region for the new data

std::string g_str;
 
void* test(void*)
{
	
	while(1)
	{
		std::string str  ="123456";
		g_str = str;
	}
}
 
int main()
{
 
	pthread_t t1;
	pthread_create(&t1, NULL, test, NULL); 
	usleep(50);
 
	
	while(1)
	{
		std::string str  ="123456";
		g_str = str;
	}
	return 0;
}
