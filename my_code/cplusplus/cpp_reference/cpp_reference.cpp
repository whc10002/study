#include <iostream>

using namespace std;
 
int main()  
{  
	// A simple integer variable
	int i = 10;
	// A Reference to the variable i
	int &j = i;
	// Incrementing j will increment both i and j.
	j++;
	// check by printing values of i and j
	// should print 11 11  
	cout<<  i  <<  j  <<endl;
	// Now try to print the address of both variables i and j  
	cout<<  &i  <<  &j  <<endl;  
	// surprisingly both print the same address and make us feel that they are  
	// alias to the same memory location.  
	// In example below we will see what is the reality 
	//
	
	int i2 = 10;
	int *const j2 = &i2;
	(*j)++;
	cout<<  i  <<  *j  <<endl;
	 
	return 0;  
}
