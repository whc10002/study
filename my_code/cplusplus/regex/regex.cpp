#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <map>

int test_regex_match()
{
	std::string pattern{ "\\d{3}-\\d{8}|\\d{4}-\\d{7}" }; // fixed telephone
	std::regex re(pattern);
 
	std::vector<std::string> str{ "010-12345678", "0319-9876543", "021-123456789"};
 
	/* std::regex_match:
		判断一个正则表达式(参数re)是否匹配整个字符序列str,它主要用于验证文本
		注意，这个正则表达式必须匹配被分析串的全部，否则返回false;如果整个序列被成功匹配，返回true
	*/
 
	for (auto tmp : str) {
		bool ret = std::regex_match(tmp, re);
		if (ret) fprintf(stderr, "%s, can match\n", tmp.c_str());
		else fprintf(stderr, "%s, can not match\n", tmp.c_str());
	}
 
	return 0;
}
 
int test_regex_search()
{
	std::string pattern{ "http|hppts://\\w*$" }; // url
	std::regex re(pattern);
 
	std::vector<std::string> str{ "http://blog.csdn.net/fengbingchun", "https://github.com/fengbingchun",
		"abcd://124.456", "abcd https://github.com/fengbingchun 123" };
 
	/* std::regex_search:
		类似于regex_match,但它不要求整个字符序列完全匹配
		可以用regex_search来查找输入中的一个子序列，该子序列匹配正则表达式re
	*/
 
	for (auto tmp : str) {
		bool ret = std::regex_search(tmp, re);
		if (ret) fprintf(stderr, "%s, can search\n", tmp.c_str());
		else fprintf(stderr, "%s, can not search\n", tmp.c_str());
	}
 
	return 0;
}
 
int test_regex_search2()
{
	std::string pattern{ "[a-zA-z]+://[^\\s]*" }; // url
	std::regex re(pattern);
 
	std::string str{ "my csdn blog addr is: http://blog.csdn.net/fengbingchun , my github addr is: https://github.com/fengbingchun " };
	std::smatch results;
	while (std::regex_search(str, results, re)) {
		for (auto x : results)
			std::cout << x << " ";
		std::cout << std::endl;
		str = results.suffix().str();
	}
 
	return 0;
}
 
int test_regex_replace()
{
	std::string pattern{ "\\d{18}|\\d{17}X" }; // id card
	std::regex re(pattern);
 
	std::vector<std::string> str{ "123456789012345678", "abcd123456789012345678efgh",
		"abcdefbg", "12345678901234567X" };
	std::string fmt{ "********" };
 
	/* std::regex_replace:
		在整个字符序列中查找正则表达式re的所有匹配
		这个算法每次成功匹配后，就根据参数fmt对匹配字符串进行替换
	*/
 
	for (auto tmp : str) {
		std::string ret = std::regex_replace(tmp, re, fmt);
		fprintf(stderr, "src: %s, dst: %s\n", tmp.c_str(), ret.c_str());
	}
 
	return 0;
}
 
int test_regex_replace2()
{
	// reference: http://www.cplusplus.com/reference/regex/regex_replace/
	std::string s("there is a subsequence in the string\n");
	std::regex e("\\b(sub)([^ ]*)");   // matches words beginning by "sub"
 
	// using string/c-string (3) version:
	std::cout << std::regex_replace(s, e, "sub-$2");
 
	// using range/c-string (6) version:
	std::string result;
	std::regex_replace(std::back_inserter(result), s.begin(), s.end(), e, "$2");
	std::cout << result;
 
	// with flags:
	std::cout << std::regex_replace(s, e, "$1 and $2", std::regex_constants::format_no_copy);
	std::cout << std::endl;
 
	return 0;
}

int replace_str(const std::string src, std::string &result)
{
	result = src.c_str();
        static const std::string prefix = "${";
        static const std::string suffix = "}";
        static const std::string gdpr_consent_prefix = "${GDPR_CONSENT_";
        size_t pre_pos = src.find(prefix);
        if (pre_pos == std::string::npos)
                return 0;

        const std::map<std::string, std::string> macros = {
                {"${US_PRIVACY}", "#{US_PRIVACY}"},
                {"${GDPR}", "#{GDPR}"},
                {"${GDPR_CONSENT}", "#{GDPR_CONSENT}"}
        };
        while (pre_pos != std::string::npos)
        {
                size_t found = result.find(suffix, pre_pos);
                if (found == std::string::npos)
                        break;
                std::string pre_part = result.substr(0, pre_pos);
                std::string name = result.substr(pre_pos, found);
                std::string left_part = result.substr(found + suffix.length());

                if (name.empty())
                        break;
                const auto it = macros.find(name);
                if (it != macros.end())
                        name = it->second;
                else
                {
                        size_t pos = name.find(gdpr_consent_prefix);
                        if (pos != std::string::npos)
                        {
                                std::string num_part = name.substr(pos + gdpr_consent_prefix.length(), name.length()-1);
                                if (num_part.empty())
                                        break;
                                name = "#{GDPR_CONSENT_285}";
                        }
                }
                result = pre_part + name + left_part;
                pre_pos = result.find(prefix, found);
        }
	return 0;
}

int main(int argc, char* argv[])
{
	std::regex e("\\$\\{GDPR_CONSENT_\\d+\\}");
	std::string name = "http://www.baidu.com/para?nihao=2&gdpr=${GDPR_CONSENT_123}&hi=${GDPR_CONSENT}&test=test";
	// std::cout << std::regex_replace(name.c_str(), e, "${GDPR_CONSENT}") << std::endl;
	std::cout << name << std::endl;
		
	std::string result;
	replace_str(name, result);
	std::cout << result << std::endl;

	//test_regex_match();
	//test_regex_search();
	//test_regex_search2();
	//test_regex_replace();
	//test_regex_replace2();

	return 0;
}
