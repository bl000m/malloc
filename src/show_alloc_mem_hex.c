#include "malloc.h"


void show_alloc_mem_hex(void)
{
    pthread_mutex_lock(&g_malloc_mutex);
    t_zone *zone = get_the_last_zone(g_zone_list);
    size_t total_allocated = 0;

    ft_putstr("\n\nALLOCATED HEX DUMP MEMORY RECAP\n**********************\n");

    total_allocated = dump_zone_memory(zone);

    ft_putstr("Total : ");
    ft_itoa_base(total_allocated, 10, 0, false);
    ft_putstr(" octets\n");

    pthread_mutex_unlock(&g_malloc_mutex);
}



size_t dump_zone_memory(t_zone *zone)
{
    size_t i;
    char *ptr;
    size_t total_size = 0;

    while (zone) {
        print_zone_header(zone->type == TINY_ZONE ? "TINY" :
                          zone->type == SMALL_ZONE ? "SMALL" : "LARGE", zone);

        i = 0;
        ptr = (char *)((void *)zone + sizeof(t_zone));
        while (i < zone->size) {
            size_t line_size = (zone->size - i < 16) ? zone->size - i : 16;
            print_hex_line(ptr + i, line_size);
            i += 16;
        }

        total_size += zone->size;
        zone = zone->prev;
    }

    return total_size;
}



void print_hex_line(char *start, size_t size)
{
    uint8_t i;

    // Print the starting address of the line in hexadecimal format (true for prefix)
    ft_itoa_base((size_t)start, 16, 0, true);

    // Loop through each byte in the line
    for (i = 0; i < 16; i++) {
        if (i < size) {
            ft_putstr(" ");
            // Print the byte content at the address in hexadecimal format
            ft_itoa_base((unsigned char)start[i], 16, 2, false);
        } else {
            ft_putstr("   ");
        }
    }
    ft_putstr("\n");
}
