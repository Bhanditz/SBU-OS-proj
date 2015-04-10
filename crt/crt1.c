#include <stdlib.h>
#include <stdio.h>
#include <sys/defs.h>

void _start(void) {
	void *rsp;
	int *argc;
	char **argv;
	char **envp;

	int res;

	__asm__ __volatile__(
		"movq %%rsp, %0"
		:"=a" (rsp)
		::"memory");

	argc = (int *)rsp + 2;
	argv = (char **)(argc) + 1;
	envp = argv + *argc + 1;

	res = main(*argc, argv, envp);

	exit(res);

}
