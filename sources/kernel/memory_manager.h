#ifndef KERNEL_MEMORY_MANAGER
#define KERNEL_MEMORY_MANAGER

#include "../header/standard_type.h"
#include "../vga/write.h"

#define KERNEL_MEMORY_POOL_SIZE 2000000
#define KERNEL_MEMORY_PAGE_SIZE 4096  
#define KERNEL_MEMORY_ALIGNEMENT 8

typedef struct Block 
{
    size_t size;
    struct Block* next;
    int free;
} Block;

extern uint8_t memory_pool[KERNEL_MEMORY_POOL_SIZE];
extern Block* free_list;

size_t align(size_t size);
void k_memory_init();
void* malloc(size_t size);
void free(void* ptr);

#endif // !KERNEL_MEMORY_MANAGER