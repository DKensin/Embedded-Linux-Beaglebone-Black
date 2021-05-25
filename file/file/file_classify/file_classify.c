#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	printf("File type: \t");

	switch (sb.st_mode & S_IFMT) {
	case S_IFBLK:
		printf("block device\n");
		break;
	case S_IFCHR:
		printf("character device\n");
		break;
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFIFO:
		printf("FIFO/pipe\n");
		break;
	case S_IFLNK:
		printf("symlink\n");
		break;
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown?\n");
		break;
	}

	printf("I-node number: \t%ld\n", sb.st_ino);
	printf("Mode: \t%d\n", sb.st_mode);
	printf("Link count: \t%ld\n", sb.st_nlink);
	printf("Ownership: \tUID = %d, GID = %d\n", sb.st_uid, sb.st_gid);
	printf("Preferred I/O block size: \t%ld\n", sb.st_blksize);
	printf("File size: \t%ld\n", sb.st_size);
	printf("Blocks allocated: \t%ld\n", sb.st_blocks);

	printf("Last status change: \t%s\n", ctime(&sb.st_ctime));
	printf("Last file access: \t%s\n", ctime(&sb.st_atime));
	printf("Last file modification: \t%s\n", ctime(&sb.st_mtime));

	return 0;
}
