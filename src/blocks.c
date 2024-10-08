#include "malloc.h"


bool is_block_suitable(const t_block *block, size_t size) 
{
    return block->free && block->size >= size + sizeof(t_block);
}


t_block *find_suitable_block(t_zone *zone, size_t size) 
{
    t_block *block = (t_block *)SKIP_ZONE_METADATA(zone);

    while (block) 
    {
        if (is_block_suitable(block, size)) 
        {
            return block;
        }
        block = block->next;
    }
    return NULL;
}

void initialize_new_free_block_in_remaining_space(t_block **remaining_free_block, t_block *allocated_block, size_t requested_size)
{
    *remaining_free_block = (t_block *)((char *)SKIP_BLOCK_METADATA(allocated_block) + requested_size);

    // size_t remaining_size = allocated_block->next - *remaining_free_block;
    size_t remaining_size = allocated_block->size - (requested_size + sizeof(t_block));

    setup_block(*remaining_free_block, remaining_size);
    (*remaining_free_block)->free = true;
    (*remaining_free_block)->prev = allocated_block;
    (*remaining_free_block)->next = allocated_block->next;
}

void update_allocated_block_and_zone_metadata(t_zone *zone, t_block *allocated_block, t_block *remaining_free_block, size_t requested_size)
{
    zone->block_count++;
    allocated_block->next = remaining_free_block;
    allocated_block->size = requested_size;
    allocated_block->free = false;
}


void setup_block(t_block *block,  size_t size) {
	block->prev = NULL;
	block->next = NULL;
	block->size = size;
	block->free = false;
}


t_block *position_block_after_last(t_block *last_block)
{
    return (t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);

}



void link_block_to_last(t_block *new_block, t_block *last_block) 
{
    new_block = (t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);
    last_block->next = new_block;
    new_block->prev = last_block;
}


t_block *get_last_block(t_block *block) 
{
    while (block->next)
        block = block->next;
    return block;
}



void locate_block_by_ptr(t_zone **founded_zone, t_block **founded_block, t_zone *zone, void *ptr)
{
    t_block *block;
    while (zone)
    {
        block = (t_block *)SKIP_ZONE_METADATA(zone);
        while (block)
        {
            if (SKIP_BLOCK_METADATA(block) == ptr)
            {
                *founded_zone = zone;
                *founded_block = block;
                return;
            }
            block = block->next;
        }
        zone = zone->next;
    }
    *founded_zone = NULL;
    *founded_block = NULL;
}


bool are_blocks_free(t_zone *zone)
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