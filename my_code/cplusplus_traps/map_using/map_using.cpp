#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

enum REGION
{
	REGION_NONE	= 0,
	REGION_US_EAST  = 1,
	REGION_US_WEST  = 2,
	REGION_EU	= 3
};

int main(int argc, char* argv[])
{
	string hostname = "laxads001";
	REGION region;

	std::map<std::string, REGION> prefixs = {
		{"sdnyads", REGION_US_EAST},
		{"nycads", REGION_US_EAST},
		{"ashads", REGION_US_EAST},
		{"svlads", REGION_US_WEST},
		{"laxads", REGION_US_WEST},
		{"lonads", REGION_EU},
		{"amsads", REGION_EU}
	};
	for (auto prefix : prefixs)
	{
		if(hostname.find(prefix.first) == 0)
		{
			region = prefix.second;
			break;
		}
	}

	std::map<int, string, std::less<int>, std::allocator<string>> buyer_platform_url_ = {
		{REGION_EU, "http://eu.com"},
		{REGION_US_WEST, "http://us_west.com"},
		{REGION_US_EAST, "http://us_east.com"}
	};

	// std::map with 3 elements =    {[1] = "http://us_east.com";, [2] = "http://us_west.com";, [3] = "http://eu.com”;}
	// Beware of traps!!! If we cant't get the value by usisng operator[] to query map
	// then, then the queried value will be insert to the map.
	if (!buyer_platform_url_[0].empty())
	{
		cout << "none:" << buyer_platform_url_.begin()->second.c_str() << endl;
	}
	// std::map with 4 elements =    {[0] = "", [1] = "http://us_east.com";, [2] = "http://us_west.com";, [3] = "http://eu.com";}
	if (!buyer_platform_url_[REGION_NONE].empty())
	{
		cout << "none:" << buyer_platform_url_.begin()->second.c_str() << endl;
	}
	//else
	//{
	//	cout << "right" << buyer_platform_url_[region].c_str()<< endl;
	//}
	//
	//if (!buyer_platform_url_[region].empty())
	//{
	//	cout << "right" << buyer_platform_url_[region].c_str()<< endl;
	//}
	else
	{
		cout << "none:" << buyer_platform_url_.begin()->second.c_str() << endl;
	}
	return 0;
}
