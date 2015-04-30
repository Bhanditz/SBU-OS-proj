#include <sys/defs.h>

#ifndef _TARFS_H
#define _TARFS_H

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

typedef struct posix_header_ustar posix_header_ustar;

struct posix_header_ustar {
	char name[100]; 	//	File name
	char mode[8];		//	File mode
	char uid[8];		//	Owner's numeric user ID
	char gid[8];		//	Group's numeric user ID
	char size[12];		//	File size in bytes (octal base)
	char mtime[12];		//	Last modification time in numeric Unix time format (octal)
	char checksum[8];	//	Checksum for header record
	char typeflag[1];	//	Link indicator (file type)
	char linkname[100];	//	Name of linked file
	char magic[6];		//	UStar indicator "ustar"
	char version[2];	//	UStar version "00"
	char uname[32];		//	Owner user name
	char gname[32];		//	Owner group name
	char devmajor[8];	//	Device major number
	char devminor[8];	//	Device minor number
	char prefix[155];	//	Filename prefix
	char pad[12];
};

void* retrieval(char *file_name);

#endif
