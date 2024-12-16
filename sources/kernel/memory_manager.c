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

// Initializes the memory management system by setting up the free list of memory blocks.
void k_memory_init()
{
    // The memory pool starts at the location of 'memory_pool' and the free list is initialized.
    free_list = (Block*) memory_pool;

    // Make sure the free list is aligned to the desired boundary (align the start of the block)
    uintptr_t aligned_memory = (uintptr_t) free_list;
    aligned_memory = (aligned_memory + KERNEL_MEMORY_ALIGNEMENT - 1) & ~(KERNEL_MEMORY_ALIGNEMENT - 1);
    free_list = (Block*) aligned_memory;

    // The size of the first free block is set to the total size of the pool minus the size of the Block header.
    free_list->size = KERNEL_MEMORY_POOL_SIZE - (aligned_memory - (uintptr_t) memory_pool) - sizeof(Block);
    free_list->next = NULL;
    free_list->free = 1;
}

// Custom malloc function to allocate memory from the memory pool.
void* malloc(size_t size)
{
    // Align the size of the requested memory to the kernel's alignment size.
    size = align(size);

    // Traverse the free list to find a suitable block for allocation.
    Block* current = free_list;
    Block* previous = NULL;

    while (current != NULL)
    {
        // If the current block is free and large enough to hold the requested size.
        if(current->free && current->size >= size)
        {
            // If the current block has more space than needed, split it.
            if(current->size > size + sizeof(Block))
            {
                // Create a new block at the end of the current block for the remaining free space.
                Block* next_block = (Block*) ((uint8_t*) current + sizeof(Block) + size);
                next_block->size = current->size - size - sizeof(Block);  // Set the new block's size.
                next_block->free = 1;  // Mark the new block as free.
                next_block->next = current->next;  // Link the new block to the next block.

                // Adjust the current block's size and set it to point to the newly created block.
                current->size = size;
                current->next = next_block;
            }

            // Mark the block as allocated (no longer free).
            current->free = 0;
            // Return a pointer to the memory after the Block header.
            return (uint8_t*) current + sizeof(Block);
        }

        // Move to the next block in the list.
        previous = current;
        current = current->next;
    }

    // If no suitable block is found, return NULL to indicate failure.
    return NULL;
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
