#include "malloc.h"

size_t print_block_list(t_block *block) {
    char *start_address;
    char *end_address;
    size_t total = 0;
    size_t index = 0;

    while (block) {
        start_address = (char *)block + BLOCK_OVERHEAD;
        end_address = start_address + block->size;

        // Debug prints
        // printf("Processing block #%zu at %p\n", index, (void *)block);
        // printf("  Block start address: %p\n", (void *)start_address);
        // printf("  Block end address: %p\n", (void *)end_address);
        // printf("  Block size: %zu\n", block->size);
        // printf("  Block free status: %d\n", block->free);

        if (!block->free) {
            // Print block range and size
            printf("%p - %p : %zu octets\n", (void *)start_address, (void *)end_address, block->size);
            total += block->size;
        }

        // Debug logging for the next block
        // if (block->next) {
        //     printf("  Next block is at %p\n", (void *)block->next);
        // } else {
        //     printf("  No next block (end of list)\n");
        // }

        // Move to the next block
        block = block->next;
        index++;
    }

    return total;
}

// void print_zone(const char *name, t_zone *zone) {
//     printf("%s : %p\n", name, (void *)zone);
//     if (zone->blocks) {
//         size_t zone_total = print_block_list(zone->blocks);
//         printf("Zone total : %zu octets\n", zone_total);
//     }
// }

void start_show_alloc_mem(void) {
    printf("\n___SHOW MEMORY ALLOCATED___\n");
    t_zone *zone;
    size_t total = 0;

    // Print TINY zones
    zone = g_malloc_manager.tiny_zones;
    while (zone) {
        printf("\nTINY : %p\n", (void *)zone);
        total += print_block_list(zone->blocks);
        zone = zone->next;
    }

    // Print SMALL zones
    zone = g_malloc_manager.small_zones;
    while (zone) {
        printf("\nSMALL : %p\n", (void *)zone);
        total += print_block_list(zone->blocks);
        zone = zone->next;
    }

    // Print LARGE zones
    zone = g_malloc_manager.large_zones;
    while (zone) {
        printf("\nLARGE : %p\n", (void *)zone);
        total += print_block_list(zone->blocks);
        zone = zone->next;
    }

    // Print total memory usage
    printf("Total : %zu octets\n", total);
}

void show_alloc_mem(void) {
    pthread_mutex_lock(&g_malloc_mutex);
    start_show_alloc_mem();
    pthread_mutex_unlock(&g_malloc_mutex);
}
