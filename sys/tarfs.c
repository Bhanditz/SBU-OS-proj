#include <sys/common.h>
#include <sys/sbunix.h>
#include <sys/kstring.h>
#include <sys/paging.h>
#include <sys/proc.h>
#include <sys/elf.h>
#include <sys/tarfs.h>
#include <sys/phys_mem.h>
#include <sys/virt_mem.h>

void* retrieval(char *file_name) {

    posix_header_ustar *header = (posix_header_ustar *)&_binary_tarfs_start;
    posix_header_ustar *tailer = (posix_header_ustar *)&_binary_tarfs_end;
    posix_header_ustar *ptr = header;
    uint64_t file_size = 0; 
       
    while(ptr < tailer) {
        if(!kstrlen(ptr->name)) {
            continue;
        } 
        file_size = k_oct_to_dec(katoi(ptr->size));
        //printf("ptr addr: %p\n", ptr);
        //printf("File Name: %s, File Size: %d, Type: %s\n", ptr->name, file_size, ptr->typeflag);
        
        if (!kstrcmp(file_name, ptr->name)) { 
            //printf("Found it! addr: %p\n", ptr);  
            return (void *)(ptr+1);
        }
        if (file_size) {
            //plus 2 because one for the header size, another for the file size part
            ptr += file_size/sizeof(posix_header_ustar) + 2;
        } else {
            ptr++;
        }
    }
    
    return NULL;
    
}
