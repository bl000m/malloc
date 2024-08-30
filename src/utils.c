#include "malloc.h"

void cleanup(void) {
    pthread_mutex_destroy(&g_malloc_mutex);
}

void clear_memory_contents(void *ptr, size_t size) {
    if (ptr) {
        ft_memset(ptr, 0xaa, size);
    }
}

bool validate_allocation_size(size_t size)
{
    size_t aligned_size = (size + 15) & ~15;
    rlim_t system_limit = get_system_limit();

    if (aligned_size <= 0 || aligned_size > system_limit) {
        return false;
    }
    return true;
}

void *address_after_metadata(t_block *block) {
    if (block) {
        return (void *)SKIP_BLOCK_METADATA(block);
    }
    return NULL;
}