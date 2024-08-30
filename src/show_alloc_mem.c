#include "malloc.h"

/*
 * Print a formatted address and size.
 */
static void print_block_info(char *start, char *end, size_t size)
{
    ft_itoa_base((size_t)start, 16, 9, true);
    ft_putstr(" - ");
    ft_itoa_base((size_t)end, 16, 9, true);
    ft_putstr(" : ");
    ft_itoa_base(size, 10, 0, false);
    ft_putstr(" octets\n");
}

/*
 * Print allocated memory blocks and return the total size.
 */
static size_t print_allocated_blocks(t_block *block)
{
    size_t total_size = 0;

    while (block)
    {
        if (!block->free)
        {
            char *start_address = (char *)((void *)block + sizeof(t_block));
            char *end_address = start_address + block->size;
            print_block_info(start_address, end_address, block->size);
            total_size += block->size;
        }
        block = block->next;
    }
    return total_size;
}

/*
 * Print the header for a memory zone with color formatting.
 */
static void print_zone_header(const char *zone_name, t_zone *zone)
{
    ft_putstr(GREEN);
    ft_putstr(zone_name);
    ft_putstr(" : ");
    ft_itoa_base((size_t)zone, 16, 9, true);
    ft_putstr(RESET_COLOR);
}

/*
 * Check if all blocks in the zone are free.
 */
static bool are_blocks_free(t_zone *zone)
{
    t_block *block = (t_block *)((void *)zone + sizeof(t_zone));

    while (block)
    {
        if (!block->free)
            return false;
        block = block->next;
    }
    return true;
}

/*
 * Show memory allocations in each zone.
 */
void show_alloc_mem(void)
{
    pthread_mutex_lock(&g_malloc_mutex);
    t_zone *zone = g_zone_list;
    size_t total_allocated = 0;

    ft_putstr("\n\nALLOCATED MEMORY RECAP\n**********************\n");

    while (zone)
    {
        if (!are_blocks_free(zone))
        {
            switch (zone->type)
            {
                case TINY_ZONE: print_zone_header("TINY", zone); break;
                case SMALL_ZONE: print_zone_header("SMALL", zone); break;
                case LARGE_ZONE: print_zone_header("LARGE", zone); break;
            }
            total_allocated += print_allocated_blocks((t_block *)((void *)zone + sizeof(t_zone)));
        }
        zone = zone->next;
    }

    ft_putstr("Total : ");
    ft_itoa_base(total_allocated, 10, 0, false);
    ft_putstr(" octets\n");
    pthread_mutex_unlock(&g_malloc_mutex);
}
