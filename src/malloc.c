#include "malloc.h"


t_zone *g_zone_list = {0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;


void *malloc(size_t size) 
{
    if (!validate_allocation_size(size)) return NULL;

    pthread_mutex_lock(&g_malloc_mutex);
    void *memory_address_allocated = allocate_and_clear_memory(size);
    pthread_mutex_unlock(&g_malloc_mutex);

    return memory_address_allocated;
}



void *allocate_and_clear_memory(size_t size)
{
    size = ALIGN(size);
    void *memory_address_allocated = try_allocate_block(size);
    if (memory_address_allocated) {
        clear_memory_contents(memory_address_allocated, size);
    }
    return memory_address_allocated;
}



void *try_allocate_block(size_t size) 
{
    t_block *block = find_and_split_block_if_available(size);
    if (block) return address_after_metadata(block);

    t_zone *zone = get_or_create_zone(size);
    if (!zone) return NULL;

    return allocate_block_in_zone(zone, size);
}



t_block *find_and_split_block_if_available(size_t size) 
{
    t_block *block = NULL;
    t_zone *zone = NULL;

    find_available_block(size, &zone, &block);

    if (block && zone) 
        split_block_and_create_remaining_free_block(block, size, zone);

    return block;
}



void find_available_block(size_t size, t_zone **found_zone, t_block **found_block) 
{
    e_zone target_type = get_zone_type_from_block_size(size);
    t_zone *current_zone = g_zone_list;

    while (current_zone) 
    {
        if (current_zone->type == target_type) 
        {
            *found_block = find_suitable_block(current_zone, size);
            if (*found_block) 
            {
                *found_zone = current_zone;
                return;
            }
        }
        current_zone = current_zone->next;
    }
}



void split_block_and_create_remaining_free_block(t_block *allocated_block, size_t requested_size, t_zone *zone) 
{
    t_block *remaining_free_block;
    initialize_new_free_block_in_remaining_space(&remaining_free_block, allocated_block, requested_size);
    update_allocated_block_and_zone_metadata(zone, allocated_block, remaining_free_block, requested_size);
}



t_zone *get_or_create_zone(size_t size) 
{
    e_zone zone_type = get_zone_type_from_block_size(size);
    t_zone *zone = find_existing_zone(size, zone_type);

    if (!zone) {
        zone = create_and_add_zone(zone_type, size);
    }

    return zone;
}



void *allocate_block_in_zone(t_zone *zone, size_t size) 
{
    t_block *new_block;
    t_block *last_block = NULL;

    new_block = (t_block *)SKIP_ZONE_METADATA(zone); // if zone empty block allocated at beginning of zone

    if (zone->block_count) {
        last_block = get_last_block(new_block);
        new_block = position_block_after_last(last_block);
        link_block_to_last(new_block, last_block);
    }
    setup_block(new_block, size);
    update_zone_metadata(zone, new_block);

    return (void *)SKIP_BLOCK_METADATA(new_block);
}















