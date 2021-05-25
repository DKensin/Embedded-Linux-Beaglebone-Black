#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void main(void)
{
	struct stat data_stat;

	if (0 != stat("data", &data_stat)) {  
		printf("Can't read file property\n");
		return;
	}

	printf("mode = %u\n", data_stat.st_mode);
	printf("size = %ld\n", data_stat.st_size);
	printf("uid = %u, gid = %u\n", data_stat.st_uid, data_stat.st_gid);

}
