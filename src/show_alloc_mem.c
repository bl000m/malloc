#include "malloc.h"

/*
 * Print the list of blocks in the given zone.
 * Returns the total size of allocated blocks.
 */
size_t print_block_list(t_block *block)
{
    char    *start_address;
    char    *end_address;
    size_t  total = 0;

    while (block)
    {
        start_address = (char *)((void *)block + sizeof(t_block));
        end_address = start_address + block->size;
        if (!block->free)
        {
            ft_itoa_base((size_t)start_address, 16, 9, true);
            ft_putstr(" - ");
            ft_itoa_base((size_t)end_address, 16, 9, true);
            ft_putstr(" : ");
            ft_itoa_base(block->size, 10, 0, false);
            ft_putstr(" octets\n");
            total += block->size;
        }
        block = block->next;
    }
    return total;
}


/*
 * Print the header for a zone.
 */
static void print_zone_header(const char *name, t_zone *zone)
{
    ft_putstr(name);
    ft_putstr(" : ");
    ft_itoa_base((size_t)zone, 16, 9, true);
    ft_putstr("\n");
}

/*
 * Show the memory allocation for each zone.
 */
void start_show_alloc_mem(void)
{
    t_zone  *zone;
    size_t  total = 0;

    zone = g_zone_list;
    while (zone)
    {
        if (zone->type == TINY_ZONE)
            print_zone_header("TINY", zone);
        else if (zone->type == SMALL_ZONE)
            print_zone_header("SMALL", zone);
        else
            print_zone_header("LARGE", zone);

        if (zone->block_count)
            total += print_block_list((t_block *)((void *)zone + sizeof(t_zone)));
        
        zone = zone->next;
    }
    ft_putstr("Total : ");
    ft_itoa_base(total, 10, 0, false);
    ft_putstr(" octets\n");
}


/*
 * Thread-safe function to show all allocated memory.
 */
void show_alloc_mem(void)
{
    pthread_mutex_lock(&g_malloc_mutex);
    start_show_alloc_mem();
    pthread_mutex_unlock(&g_malloc_mutex);
}