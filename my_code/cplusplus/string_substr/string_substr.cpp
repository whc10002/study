#include <iostream>
#include <string>

using namespace std;

string check_and_correct_invalid_url(const string & url)
{
	string result = url;
	size_t first_slash_pos,first_qmark_pos;
	first_qmark_pos = url.find_first_of('?');
	first_slash_pos = url.find_first_of('/',8);
	if (first_qmark_pos == string::npos)
	{	
		cout << "return_1" << endl;
        	return result;
	}
	if (first_slash_pos != string::npos && first_slash_pos < first_qmark_pos)
	{
		cout << "return_2" << endl;
        	return result;
	}
	cout << "modify" << endl;
	result = url.substr(0,first_qmark_pos) + "/" + url.substr(first_qmark_pos,url.length() - first_qmark_pos);
	return result;
}

int main(int argc, char *argv[])
{
	string url = "http://www.baidu.com?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	url = "http://www.baidu.com/?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
	
	url = "http://www.baidu.com/findle?a=2";
	cout << check_and_correct_invalid_url(url) << endl;
}
