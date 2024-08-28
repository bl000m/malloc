#include "malloc.h"


void *realloc(void *ptr, size_t size) {
    if (!ptr) return malloc(size);

    t_block *block = (t_block *)ptr - 1;
    size_t old_size = block->size;

    if (size <= old_size) {
        split_block(block, size);
        return ptr;
    }

    void *new_ptr = malloc(size);
    if (new_ptr) {
        ft_memcpy(new_ptr, ptr, old_size);
        free(ptr);
    }
    return new_ptr;
}
