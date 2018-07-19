#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "Mmap_Allocate.h"

using namespace std;

int main(int argc, char*argv[])
{
	int fd = open("backingstore.dat", O_RDONLY);

	void *mapping;
	ftruncate(fd, g_mmap_size);
	mapping = mmap(g_mmap_start, g_mmap_size, PROT_READ, MAP_SHARED | MAP_FIXED, fd, 0);
	if(mapping == MAP_FAILED)
	{
		printf("MMap failed.\n");
		return 1;
	}

	std::string key("key");	
	auto *map = reinterpret_cast<std::map<std::string,std::string> *>(mapping);
 	printf("Sizeof map: %ld.\n", (long)map->size());
 	printf("Value of 'key': %s\n", (*map)[key].c_str());
 	printf("Value of 'key1': %s\n", (*map)["key1"].c_str());
	return 0;
}
