#include "malloc.h"


// void free(void *ptr) {
//     if (!ptr) return;

//     pthread_mutex_lock(&g_malloc_mutex);

//     t_block *block = (t_block *)ptr - 1;
//     block->free = 1;

//     t_zone *zone = find_zone_for_block(block);
//     if (zone) {
//         coalesce_free_blocks(ptr);
//     }

//     pthread_mutex_unlock(&g_malloc_mutex);
// }

// t_zone *find_zone_for_block(t_block *block) {
//     t_zone *current_zone = g_zone_list;
//     while (current_zone) {
//         if (block >= (t_block *)((char *)current_zone + sizeof(t_zone)) &&
//             block <= (t_block *)((char *)current_zone + current_zone->size)) {
//             return current_zone;
//         }
//         current_zone = current_zone->next;
//     }

//     return NULL;
// }

// void coalesce_free_blocks(t_block *ptr) {
//     t_block *current = ptr;
//     while (current) {
//         t_block *next = current->next;
//         if (current->free && next && next->free) {
//             current->size += next->size + sizeof(t_block);
//             current->next = next->next;
//             if (next->next) {
//                 next->next->prev = current;
//             }
//         }
//         current = next;
//     }
// }

static t_block *merge_block(t_zone *zone, t_block *block) {
    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(t_block);
        block->next = block->next->next;
        if (block->next) block->next->prev = block;
        zone->block_count--;
    }
    if (block->prev && block->prev->free) {
        block->prev->size += block->size + sizeof(t_block);
        block->prev->next = block->next;
        if (block->next) block->next->prev = block->prev;
        zone->block_count--;
        return block->prev;
    }
    return block;
}

static void remove_block_if_last(t_zone *zone, t_block *block) {
    if (block->free && !block->next) {
        if (block->prev) block->prev->next = NULL;
        zone->free_size += block->size + sizeof(t_block);
        zone->block_count--;
    }
}


static void search_ptr(t_zone **ptr_zone, t_block **ptr_block, t_zone *zone, void *ptr) {
    while (zone) {
        t_block *block = (t_block *)((char *)zone + sizeof(t_zone));
        while (block) {
            if ((void *)((char *)block + sizeof(t_block)) == ptr) {
                *ptr_zone = zone;
                *ptr_block = block;
                return;
            }
            block = block->next;
        }
        zone = zone->next;
    }
    *ptr_zone = NULL;
    *ptr_block = NULL;
}

static void start_free(void *ptr) {
    if (!ptr || !g_zone_list) return;

    t_zone *zone = g_zone_list;
    t_block *block = NULL;
    search_ptr(&zone, &block, zone, ptr);

    if (block && zone) {
        block->free = true;
        // log_stack(DEALLOCATE, block->size);
        // if (getenv_cached(ENV_SCRIBBLE)) ft_memset(ptr, 0x55, block->size);
        block = merge_block(zone, block);
        remove_block_if_last(zone, block);
        delete_empty_zone(zone);
    }
}

void free(void *ptr) {
    pthread_mutex_lock(&g_malloc_mutex);
    // log_detail(FREE);
    start_free(ptr);
    pthread_mutex_unlock(&g_malloc_mutex);
}