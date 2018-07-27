#include <iostream>
#include <string>
#include <map>
 
 
 
int main(int argc, char* argv[])
{
	std::multimap<std::string, int> myMultiMap;
 
	// 插入
	myMultiMap.insert(std::pair<std::string, int>("Jack", 1));
	myMultiMap.insert(std::pair<std::string, int>("Jack", 2));
	myMultiMap.insert(std::pair<std::string, int>("Bob", 1));
	myMultiMap.insert(std::pair<std::string, int>("Navy", 3));
	myMultiMap.insert(std::pair<std::string, int>("Demo", 4));
	myMultiMap.insert(std::pair<std::string, int>("Bob", 5));
 
	// 遍历
	std::cout << "================All member"<< std::endl;
	for (std::multimap<std::string, int>::iterator iter = myMultiMap.begin(); iter != myMultiMap.end(); ++iter)
	{
		std::cout << (*iter).first << ":" << (*iter).second << std::endl;
	}
 
	
	// 统计key为"Jack"的数目
	std::string strFind = "Jack";
	unsigned int uCount = myMultiMap.count(strFind);
	if (uCount == 0)
	{
		std::cout << "================Count " << strFind << ":0"<< std::endl;
	}
	else
	{
		std::cout << "================Count " << strFind << ":" << uCount << std::endl;
		std::multimap<std::string, int>::iterator iter = myMultiMap.find(strFind);
		if (iter != myMultiMap.end())
		{
			for (unsigned int i = 0; i < uCount; ++i)
			{
				std::cout << (*iter).first << ":" << (*iter).second << std::endl;
				iter++;
			}
		}
	}


	std::cout << "================use equal_range"<< std::endl;
	typedef std::multimap<std::string, int>::iterator MultiMapIterator;
	std::pair<MultiMapIterator, MultiMapIterator> iterPair = myMultiMap.equal_range("Jack");
	for (MultiMapIterator it = iterPair.first; it != iterPair.second; ++it)
	{
		std::cout << (*it).first << ":" << (*it).second << std::endl;
	}

	// 删除所有key为"Bob"的键值对
	myMultiMap.erase("Bob");
	std::cout << "================After erase Bob"<< std::endl;
	for (std::multimap<std::string, int>::iterator iter = myMultiMap.begin(); iter != myMultiMap.end(); ++iter)
	{
		std::cout << (*iter).first << ":" << (*iter).second << std::endl;
	}


	// 删除重复的key
	MultiMapIterator iter = myMultiMap.find("Jack");
	myMultiMap.erase(iter);
	std::cout << "================Use unique key, erase \"Jack\" " << std::endl;
	for (std::multimap<std::string, int>::iterator iter = myMultiMap.begin(); iter != myMultiMap.end(); ++iter)
	{
		std::cout << (*iter).first << ":" << (*iter).second << std::endl;
	}

	return 0;
}
