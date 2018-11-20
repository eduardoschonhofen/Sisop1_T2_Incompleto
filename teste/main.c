#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include <stdio.h>

int main()
{
	DIR2 root = opendir2("/");
  puts("AAA");
	DIRENT2 direntry;
	int i;
	for(i = 0; i < 3; i++)
		readdir2(root, &direntry);
	printf("%s --- %d", direntry.name, direntry.fileType);
	return 0;
}
