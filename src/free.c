#include "malloc.h"

void	free(void *ptr)
{
	t_zone *zone;
    t_block *block;
    t_block *merged_block;

	pthread_mutex_lock(&g_malloc_mutex);
    zone = g_zone_list;
    if (!ptr || !zone)
        return;
    locate_block_by_ptr(&zone, &block, zone, ptr);
    if (block && zone)
    {
        block->free = true;
        merged_block = merge_adjacent_blocks(zone, block);
        block = merged_block ? merged_block : block;
        remove_last_block_if_needed(zone, block);
        delete_empty_zone(zone);
    }
	pthread_mutex_unlock(&g_malloc_mutex);
}



t_block *merge_adjacent_blocks(t_zone *zone, t_block *block)
{
    try_merge_with_next(zone, block);
    return try_merge_with_prev(zone, block);
}



void try_merge_with_next(t_zone *zone, t_block *block)
{
    if (block->next && block->next->free)
    {
        block->size += block->next->size + sizeof(t_block);
        if (block->next->next)
            block->next->next->prev = block;
        block->next = block->next->next;
        zone->block_count--;
    }
}



t_block *try_merge_with_prev(t_zone *zone, t_block *block)
{
    if (block->prev && block->prev->free)
    {
        block->prev->next = block->next;
        if (block->next)
            block->next->prev = block->prev;
        block->prev->size += block->size + sizeof(t_block);
        zone->block_count--;
        return block->prev;
    }
    return NULL;
}



void remove_last_block_if_needed(t_zone *zone, t_block *block)
{
    if (block->free && !block->next)
    {
        if (block->prev)
            block->prev->next = NULL;
        zone->free_size += block->size + sizeof(t_block);
        zone->block_count--;
    }
    else if (block->free)
        zone->block_count--;
}


