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
	int fd = open("backingstore.dat", O_RDWR);

	void *mapping;
	ftruncate(fd, g_mmap_size);
	mapping = mmap(g_mmap_start, g_mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
	if(mapping == MAP_FAILED)
	{
		printf("MMap failed.\n");
		return 1;
	}

	mmapstring key("key");
	mmapstring value("value");
	
	mmapstring key1("key1");
	mmapstring value1("value1");
	if(fd < 1)
	{
		printf("Open failed.\n");
		return 1;
	}

	auto map = new(mapping)mmapmap();
	(*map)[key] = value;
	(*map)[key1] = value1;
	printf("Sizeof map: %ld.\n", (long)map->size());
	printf("Value of 'key': %s\n", (*map)[key].c_str());
	printf("Value of 'key1': %s\n", (*map)[key1].c_str());
	return 0;
}
