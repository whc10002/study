#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <iostream>
#include "Mmap_Allocate.h"

using namespace std;

int main(int argc, char*argv[])
{
	int fd = open("backingstore.dat", O_RDWR);

	void *mapping;
	mapping = mmap(mmap_start, 10*1024*1024,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
	if(mapping == MAP_FAILED)
	{
		printf("MMap failed.\n");
		return 1;
	}

	mmapstring key("key");
	mmapstring value("value");
	
	if(fd < 1)
	{
		printf("Open failed.\n");
		return 1;
	}

	auto map = new(mapping)mmapmap();
	(*map)[key] = value;
	printf("Sizeof map: %ld.\n", (long)map->size());
	printf("Value of 'key': %s\n", (*map)[key].c_str());
	return 0;
}
