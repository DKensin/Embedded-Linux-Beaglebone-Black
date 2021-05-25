#include <stdio.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
  
void  main(void)
{ 
    	/* the size (in bytes) of shared memory object */
    	const int SIZE = 4096;
  
	/* name of the shared memory object */
    	const char* name = "OS";
  
    	/* shared memory file descriptor */
    	int shm_fd = -1;
  
    	/* pointer to shared memory obect */
    	void *ptr = NULL;
  
    	/* create the shared memory object */
    	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  
    	/* configure the size of the shared memory object */
    	ftruncate(shm_fd, SIZE);
  
    	/* memory map the shared memory object */
    	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  
    	/* write to the shared memory object */
    	sprintf(ptr, "Hello World.\n");

    	munmap(ptr, SIZE);
    	close(shm_fd);
}
