#include <iostream>
#include <string>
#include <string.h>
#include <map>

using namespace std;

string check_and_correct_invalid_url(const string & url)
{
	string result = url;
	size_t first_slash_pos,first_qmark_pos;
	first_qmark_pos = url.find('?');
	if (first_qmark_pos == string::npos)
	{	
		cout << "return_1" << endl;
        	return result;
	}
	size_t start_pos = url.find("://");
	if (start_pos == string::npos)
	{
		start_pos = 1;
	}
	else
	{
		start_pos += strlen(("://"));
	}
	first_slash_pos = url.find('/',start_pos);
	if (first_slash_pos != string::npos && first_slash_pos < first_qmark_pos)
	{
		cout << "return_2" << endl;
        	return result;
	}
	cout << first_qmark_pos << ":" << first_slash_pos << "\t"<< "modify" << endl;
	result = url.substr(0,first_qmark_pos) + "/" + url.substr(first_qmark_pos,url.length() - first_qmark_pos);
	return result;
}

std::pair<std::string, std::string> lookup_group_label(const std::string& type)
{
	static std::map<std::string, std::pair<std::string, std::string>> group_label_map {
		{"a", std::make_pair("group", "label")},
		{"b", {"title", "tag"}}
	};
}

int main(int argc, char *argv[])
{
	string url = "http://www.baidu.com?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	url = "http://www.baidu.com/?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	url = "http://www.baidu.com/findle?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	url = "www.baidu.com?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	url = "www.baidu.com/?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	typedef std::pair<std::string,std::string> pair;

	std::map<pair,int> map =
	{
		{ std::make_pair("C++", "C++14"), 2014 },
		{ std::make_pair("C++", "C++17"), 2017 },
		{ std::make_pair("Java", "Java 7"), 2011 },
		{ std::make_pair("Java", "Java 8"), 2014 },
		{ std::make_pair("C", "C11"), 2011 }
	};

	for (const auto &entry: map)
	{
		auto key_pair = entry.first;
		std::cout << "{" << key_pair.first << "," << key_pair.second << "}, "
				  << entry.second << '\n';
	}
	return 0;
}
