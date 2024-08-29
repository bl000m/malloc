#include "malloc.h"


void free(void *ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&g_malloc_mutex);

    t_block *block = (t_block *)ptr - 1;
    block->free = 1;

    t_zone *zone = find_zone_for_block(block);
    if (zone) {
        coalesce_free_blocks(zone);
    }

    pthread_mutex_unlock(&g_malloc_mutex);
}

t_zone *find_zone_for_block(t_block *block) {
    t_zone *current_zone = g_zone_list.tiny_zones;
    while (current_zone) {
        if (block >= (t_block *)((char *)current_zone + sizeof(t_zone)) &&
            block <= (t_block *)((char *)current_zone + current_zone->size)) {
            return current_zone;
        }
        current_zone = current_zone->next;
    }

    current_zone = g_zone_list.small_zones;
    while (current_zone) {
        if (block >= (t_block *)((char *)current_zone + sizeof(t_zone)) &&
            block <= (t_block *)((char *)current_zone + current_zone->size)) {
            return current_zone;
        }
        current_zone = current_zone->next;
    }

    current_zone = g_zone_list.large_zones;
    while (current_zone) {
        if (block >= (t_block *)((char *)current_zone + sizeof(t_zone)) &&
            block <= (t_block *)((char *)current_zone + current_zone->size)) {
            return current_zone;
        }
        current_zone = current_zone->next;
    }

    return NULL;
}

void coalesce_free_blocks(t_zone *zone) {
    t_block *current = zone->blocks;
    while (current) {
        t_block *next = current->next;
        if (current->free && next && next->free) {
            current->size += next->size + sizeof(t_block);
            current->next = next->next;
            if (next->next) {
                next->next->prev = current;
            }
        }
        current = next;
    }
}