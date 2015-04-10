#include <sys/defs.h>
#include <sys/syscall.h>
#include <syscall.h>
#include <stdlib.h>

void exit(int status) {
    syscall_1(SYS_exit, status);
}

//-------------------MEMORY----------------------------------
int brk(void *end_data_segment) {
	return syscall_1(SYS_brk, (uint64_t)end_data_segment);
}

void *mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
	return (void*)syscall_6(SYS_mmap, (uint64_t)addr, length, prot, flags, fd, offset);
}

int munmap(void *start,size_t length) {
	return syscall_2(SYS_munmap, (uint64_t)start, length);
}

//-------------------PROCESS---------------------------------
pid_t fork(void) {
	return syscall_0(SYS_fork);
}

pid_t getpid(void) {
	return syscall_0(SYS_getpid);
}

pid_t getppid(void) {
	return syscall_0(SYS_getppid);
}

int execve(const char *filename, char *const argv[], char *const envp[]) {
	return syscall_3(SYS_execve, (uint64_t)filename, (uint64_t)argv, (uint64_t)envp);
}

pid_t waitpid(pid_t pid, int *status, int options) {
	return syscall_3(SYS_wait4, pid, (uint64_t)status, options);
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
    return syscall_2(SYS_nanosleep,(uint64_t)req,(uint64_t)rem);
}

unsigned int alarm(unsigned int seconds) {
	return syscall_1(SYS_alarm, seconds);
}

//-------------------PATH-----------------------------------
char *getcwd(char *buf, size_t size) {
	return (char *)syscall_2(SYS_getcwd, (uint64_t)buf, size);
}

int chdir(const char *path) {
	return syscall_1(SYS_chdir, (uint64_t)path);
}

//-------------------FILES----------------------------------
int open(const char *pathname, int flags) {
	return syscall_2(SYS_open, (uint64_t)pathname, flags);
}

ssize_t read(int fd, void *buf, size_t count) {
	return syscall_3(SYS_read, fd, (uint64_t)buf, count);
}

ssize_t write(int fd, const void *buf, size_t count) { 
	return syscall_3(SYS_write, fd, (uint64_t)buf, count);
}

off_t lseek(int fildes, off_t offset, int whence) {
	return syscall_3(SYS_lseek, fildes, offset, whence);
}

int close(int fd) {
	return syscall_1(SYS_close, fd);
}

int pipe(int filedes[2]) {
	return syscall_1(SYS_pipe, (uint64_t)filedes);
}

int dup(int oldfd) {
	return syscall_1(SYS_dup, oldfd);
}

int dup2(int oldfd, int newfd) {
	return syscall_2(SYS_dup2, oldfd, newfd);
}

//-------------------DIRECTORIES----------------------------
int getdents(uint32_t fd, struct dirent *dirp, uint32_t count) {
	return syscall_3(SYS_getdents, fd, (uint64_t)dirp, count);
}
