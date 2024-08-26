#include "malloc.h"

void *realloc(void *ptr, size_t size) {
    // Size needs to be aligned
    size = (size + 15) & ~15;

    // If size is zero, free the pointer and return NULL
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    // If pointer is NULL, behave like malloc
    if (ptr == NULL) {
        return malloc(size);
    }

    // Lock for thread safety
    // pthread_mutex_lock(&g_malloc_manager.malloc_lock);

    // Find the block associated with the pointer
    t_block *block = (t_block*)ptr - 1;
    t_zone *zone = find_zone_for_block(block);
    if (!zone) {
        // Pointer not found in any zone
        // pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
        _optional_abort("pointer being realloc'd was not allocated");
        return NULL;
    }

    // Check if the block size is already sufficient
    if (block->size >= size) {
        // Optionally shrink the block if it's too large
        if (block->size > size + sizeof(t_block)) {
            split_block(block, size);
        }

        // pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
        return ptr;
    }

    // Allocate new block
    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        // pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
        return NULL;
    }

    // Copy old data to new block
    size_t copy_size = block->size < size ? block->size : size;
    ft_memcpy(new_ptr, ptr, copy_size);

    // Free the old block
    free(ptr);

    // pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
    return new_ptr;
}

void _optional_abort(const char *message) {
    fprintf(stderr, "%s\n", message);
    abort(); 
}