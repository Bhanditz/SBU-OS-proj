#include <sys/defs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sbush.h>


void *opendir(const char *name) {
	int* fd = malloc(sizeof(int));
	if((*fd = open(name, O_RDONLY|O_DIRECTORY)) < 0) {
		return NULL;
	}
	return fd;
}

struct dirent *readdir(void *dir) {
	struct dirent* dirp;
	memset(buf, 0, BUF_SIZE);
	if((nread = getdents(*(int *)dir, (struct dirent *)buf, BUF_SIZE)) <= 0) {
		return NULL;
	}
	dirp = (struct dirent *) buf;
	return dirp;
}

int closedir(void *dir) {
	int n;
	n = close(*(int *)dir);
	free(dir);
	return n;
}