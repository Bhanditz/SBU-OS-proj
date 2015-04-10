#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sbush.h>




int main(int argc, char* argv[], char* envp[]) {
	void* ptr;
	struct dirent* d;
	int pos;

	ptr = opendir("/bin");
	printf("ptr value: %d\n", *(int *)ptr);

	while(1) {
		readdir(ptr);
		if(nread <= 0) {
			break;
		}
		printf("---------nread = %d--------\n", nread);
		for(pos=0; pos<nread;) {
			d = (struct dirent *) (buf + pos);
			printf("filename: %s\n", d->d_name);
            pos += d->d_reclen;
		}
	}
	

	printf("close value: %d\n", closedir(ptr));

	return 0;
}
