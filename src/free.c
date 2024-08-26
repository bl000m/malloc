#include "malloc.h"

void free(void *ptr) {
    if (!ptr) return;

    // pthread_mutex_lock(&g_malloc_manager.malloc_lock);

    t_block *block = (t_block*)ptr - 1;
    block->free = 1;

    // Determine the zone and coalesce free blocks
    t_zone *zone = find_zone_for_block(block);
    if (zone) {
        coalesce_free_blocks(zone);
    }

    // if (getenv("DEBUG_MALLOC")) {
    //     show_alloc_mem();
    // }
    // pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
    ptr = NULL;

}

void coalesce_free_blocks(t_zone *zone) {
    t_block *current = zone->blocks;
    while (current && current->next) {
        if (current->free && current->next->free) {
            // Coalesce adjacent free blocks
            current->size += sizeof(t_block) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

t_zone *find_zone_for_block(t_block *block) {
    t_zone *zone = g_malloc_manager.tiny_zones;
    while (zone) {
        if ((uintptr_t)zone->blocks <= (uintptr_t)block && (uintptr_t)block < (uintptr_t)zone + zone->size) {
            return zone;
        }
        zone = zone->next;
    }
    return NULL; // Or handle large blocks separately
}