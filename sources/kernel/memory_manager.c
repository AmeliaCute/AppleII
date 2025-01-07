#include "memory_manager.h"

uint8_t memory_pool[KERNEL_MEMORY_POOL_SIZE];
Block* free_list = NULL;

// Function to align a given size to the nearest multiple of the kernel's memory alignment size.
size_t align(size_t size)
{
    // This adds the alignment size - 1 to ensure rounding up when dividing by the alignment.
    // Then, it uses a bitwise AND to clear the lower bits, rounding the size up to the nearest multiple of KERNEL_MEMORY_ALIGNEMENT.
    return (size + KERNEL_MEMORY_ALIGNEMENT - 1) & ~(KERNEL_MEMORY_ALIGNEMENT - 1);
}

void* request_memory(size_t size)
{
    size = align(size);

    void* pointer = (void*) mmap(NULL, size, PROT_READ | MAP_PRIVATE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(pointer == MAP_FAILED) return NULL;

    return pointer;
} 

// Initializes the memory management system by setting up the free list of memory blocks.
void k_memory_init()
{
    size_t initial_size = align(KERNEL_MEMORY_PAGE_SIZE);
    free_list = (Block*) request_memory(initial_size);

    if(free_list == NULL)
    {
        print("Memory allocation failure\n");
        return;
    }

    free_list->size = initial_size - sizeof(Block);
    free_list->next = NULL;
}

// Custom malloc function to allocate memory from the memory pool.
void* malloc(size_t size)
{
    size = align(size);

    Block* current = free_list;
    Block* previus = NULL;

    while(current != NULL)
    {
        if(current->free && current->size >= size)
        {
            if(current->size > size + sizeof(Block))
            {
                Block* new_block = (Block*) ((uint8_t*) current + sizeof(Block) + size);
                new_block->size = current->size - sizeof(Block) - size;
                new_block->next = current->next;
                new_block->free = 1;

                current->size = size;
                current->next = new_block;
            }

            current->free = 0;
            return (uint8_t*) current + sizeof(Block);
        }

        previus = current;
        current = current->next;
    }

    size_t allocation_size = align(size + sizeof(Block));
    Block* new_block = (Block*) request_memory(allocation_size);

    if(new_block == NULL)
    {
        print("Memory allocation failure, new block is null\n");
        return NULL;
    }

    new_block->size = allocation_size - sizeof(Block);
    new_block->next = NULL;
    new_block->free = 0;

    if(previus != NULL)
        previus->next = new_block;
    else free_list = new_block;

    return (uint8_t*) new_block + sizeof(Block);
}

// Custom free function to release previously allocated memory.
void free(void* ptr)
{
    // If the pointer is null, there's nothing to free.
    if(ptr == NULL) return;

    // Get the Block header of the given pointer by subtracting the size of the Block from the pointer.
    Block* block = (Block*) ((uint8_t*) ptr - sizeof(Block));
    // Mark the block as free.
    block->free = 1;

    // Traverse the free list to coalesce adjacent free blocks into one larger block.
    Block* current = free_list;
    while(current != NULL && current->next != NULL)
    {
        // If the current block is adjacent to the next block (i.e., they are contiguous in memory),
        // merge them into a single block.
        if((uint8_t*) current + sizeof(Block) + current->size == (uint8_t*)current->next)
        {
            current->size += sizeof(Block) + current->next->size;  // Increase the current block's size.
            current->next = current->next->next;  // Remove the merged block from the free list.
        } else 
            // If the current block is not adjacent to the next one, move to the next block.
            current = current->next;
    }
}