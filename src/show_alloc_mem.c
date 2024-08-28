#include "malloc.h"

size_t print_block_list(t_block *block) {
    char *start_address;
    char *end_address;
    size_t total = 0;

    while (block) {
        start_address = (char *)block + sizeof(t_block);
        end_address = start_address + block->size;

        if (!block->free) {
            printf("0x%zx - 0x%zx : %zu octets\n",
                (size_t)start_address, (size_t)end_address, block->size);
            total += block->size;
        }
        block = block->next;
    }
    return total;
}

void print_zone_header(const char *name, t_zone *zone) {
    printf("%s : 0x%zx\n", name, (size_t)zone);
}

void start_show_alloc_mem(void) {
    t_zone *current_zone;
    size_t total = 0;

    // Print details for TINY zones
    current_zone = g_malloc_manager.tiny_zones;
    while (current_zone) {
        print_zone_header("TINY", current_zone);
        if (current_zone->blocks) {
            total += print_block_list(current_zone->blocks);
        }
        current_zone = current_zone->next;
    }

    // Print details for SMALL zones
    current_zone = g_malloc_manager.small_zones;
    while (current_zone) {
        print_zone_header("SMALL", current_zone);
        if (current_zone->blocks) {
            total += print_block_list(current_zone->blocks);
        }
        current_zone = current_zone->next;
    }

    // Print details for LARGE zones
    current_zone = g_malloc_manager.large_zones;
    while (current_zone) {
        print_zone_header("LARGE", current_zone);
        if (current_zone->blocks) {
            total += print_block_list(current_zone->blocks);
        }
        current_zone = current_zone->next;
    }

    printf("Total : %zu octets\n", total);
}

void show_alloc_mem(void) {
    pthread_mutex_lock(&g_malloc_mutex);
    start_show_alloc_mem();
    pthread_mutex_unlock(&g_malloc_mutex);
}
