#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
	printf("%d\n", sizeof(envp)/sizeof(envp[0]));
	/*
	int i;
	for(i=0; i<sizeof(envp); i++) {
		printf("%s\n", envp[i]);
	}
	*/
	return 0;
}
