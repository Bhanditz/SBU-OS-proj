#include <sys/defs.h>

#define START_FRAME		0x80000000
#define FRAME_MAP		0x300000
#define	FRAME_LIMIT		32768
#define FRAME_SIZE		0x1000


uint64_t kalloc_frame_int();

uint64_t kalloc_frame(uint32_t size);

void kfree_frame();