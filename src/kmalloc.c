#include "inc/kmalloc.h"

#define KERNEL_HEAP_START 0x01000000
#define KERNEL_HEAP_SIZE  0x00100000
#define KERNEL_HEAP_END   (KERNEL_HEAP_START + KERNEL_HEAP_SIZE)

static uint32_t heap_pointer = KERNEL_HEAP_START;

void *kmalloc(uint32_t size)
{
	if (heap_pointer + size > KERNEL_HEAP_END) {
		return NULL;
	}

	void *allocated_memory = (void *)heap_pointer;
	heap_pointer += size;

	return allocated_memory;
}
