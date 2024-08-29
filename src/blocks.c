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