#include <sys/defs.h>

void phys_init(uint64_t phys_base, uint64_t phys_free, uint64_t phys_size);

uint64_t frame_allocator(); 

void frame_freer(uint64_t frame_addr);