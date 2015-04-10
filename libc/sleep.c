#include <stdlib.h>
#include <sys/syscall.h>

unsigned int sleep(unsigned int seconds) {
    struct timespec *a = malloc(sizeof(struct timespec));
    struct timespec *b = malloc(sizeof(struct timespec));

    a->tv_sec = seconds;
    
    return nanosleep(a,b);
}
