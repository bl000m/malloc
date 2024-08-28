#include "malloc.h"

// Define global variable
t_zone *g_malloc_manager = {0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;


void *malloc(size_t size) {
    t_zone *zone;
    t_block *block;
    void *memory_address_allocated;

    if (size <= 0) return NULL;
    size = (size + 15) & ~15;
    block = find_and_split_block_if_available(size);
    return block ? (((void *)block + sizeof(t_block))) : NULL;
    zone = get_zone_for_needed_block((const size_t)size);
    if (!zone)
        return NULL;
    memory_address_allocated = append_empty_block(zone, size);
    // log_stack
    return memory_address_allocated;  
}

t_block *find_and_split_block_if_available(size_t size)
{
    t_block *block;
    t_zone *zone;

    // Find an available block of the requested size
    find_available_block(size, &zone, &block);

    if (block && zone)
    {
        split_block_and_create_free_block(block, size, zone);
        return block;
    }
    return NULL;
}

void split_block_and_create_free_block(t_block *block, size_t size, t_zone *zone)
{
    t_block *free_block;

    // Calculate the position of the new free block
    free_block = ((void *)block + sizeof(t_block)) + size;

    // Set up the new free block with the remaining space
    free_block->prev = NULL;
	free_block->next = NULL;
	free_block->size = block->next - free_block;
	// free_block->free = false;
    free_block->free = true;
    free_block->prev = block;
    free_block->next = block->next;
    zone->block_count++;

    // Update the original block to reflect the allocated size
    block->next = free_block;
    block->size = size;
    block->free = false;
}


/*
** Find an available zone in the list that belongs to the specified zone type
** and has enough free space to accommodate the required block size.
*/
t_zone *find_available_zone(const t_zone *zone_list, const e_zone zone_type, const size_t required_size) {
    t_zone *zone = (t_zone *)zone_list;
    
    while (zone) {
        if (zone->type == zone_type && zone->size >= required_size) {
            return zone;
        }
        zone = zone->next;
    }
    return NULL;
}

/*
** Retrieve or create a zone capable of holding a block of the given size.
** If no existing zone is found, a new one is created and added to the zone list.
*/
t_zone *get_zone_for_needed_block(const size_t size) {
    t_zone *default_zone;
    e_zone zone_type;
    t_zone *zone;

    default_zone = g_malloc_manager;
    zone_type = get_zone_type_from_block_size(size);
    zone = find_available_zone(default_zone, zone_type, size + sizeof(t_block));

    if (!zone) {
        zone = create_zone(zone_type, size);
        if (!zone) {
            return NULL;
        }

        // Insert the new zone at the beginning of the zone list
        zone->next = (t_zone *)default_zone;
        if (zone->next) {
            zone->next->prev = zone;
            g_malloc_manager = zone;
        }
    }
    return zone;
}

/*
** Get the last zone in the list of zones.
*/
t_zone *get_last_zone(t_zone *zone) {
    if (!zone) {
        return NULL;
    }

    while (zone->next) {
        zone = zone->next;
    }
    return zone;
}

#include "malloc.h"

/*
** Adds a new empty block to the zone, linking it to the previous block if one exists.
** Adjusts the zone's free size accordingly and returns a pointer to the new block's data area.
*/
void *append_empty_block(t_zone *zone, size_t size) {
    t_block *new_block = (t_block *)((void *)zone + sizeof(t_zone));
    t_block *last_block = (zone->block_count) ? get_last_block(new_block) : NULL;

    if (last_block) {
        new_block = (t_block *)((void *)last_block + sizeof(t_block))+ last_block->size;
        last_block->next = new_block;
        new_block->prev = last_block;
    }
    new_block->prev = NULL;
	new_block->next = NULL;
	new_block->size = size;
	new_block->free = false;

    zone->block_count++;
    zone->free_size -= (size + sizeof(t_block));
    
    return ((void *)new_block + sizeof(t_block));
}

/*
** Retrieves the system limit for data segment size.
*/
rlim_t get_system_limit(void) {
    struct rlimit rpl;
    if (getrlimit(RLIMIT_DATA, &rpl) < 0)
        return (rlim_t)-1;
    return rpl.rlim_max;
}

size_t get_zone_type_from_block_size(const size_t size) {
    if (size <= TINY)
        return TINY_ZONE;
    else if (size <= SMALL)
        return SMALL_ZONE;
    else
        return LARGE_ZONE; 
}

size_t get_zone_size_from_zone_type(e_zone zone_type) {
    if (zone_type == TINY_ZONE)
        return TINY_ZONE_SIZE;
    else if (zone_type == SMALL_ZONE)
        return SMALL_ZONE_SIZE;
    else
        return 0; 
}

/*
** Creates a new zone of the appropriate size based on the block size and type.
** Returns NULL if the allocation fails or exceeds system limits.
*/
t_zone *create_zone(e_zone zone_type, size_t block_size) {
    size_t zone_size = get_zone_size_from_zone_type(zone_type);
    if (zone_size == 0)
        zone_size = block_size + sizeof(t_zone) + sizeof(t_block);
    if (zone_size > get_system_limit()) return NULL;

    t_zone *zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (zone == MAP_FAILED) return NULL;

    ft_bzero(zone, sizeof(t_zone));
    zone->type = zone_type;
    zone->size = zone_size;
    zone->free_size = zone_size - sizeof(t_zone);
    
    // log_detail(CREATING_ZONE);
    return zone;
}

/*
** Checks if the zone is the last one of its type that was preallocated.
** Returns true if it is, false otherwise.
*/
bool is_last_preallocated_zone(const t_zone *zone) {
    if (zone->type == LARGE_ZONE) return false;

    t_zone *cur = g_malloc_manager;
    int count = 0;
    
    while (cur) {
        if (cur->type == zone->type) count++;
        cur = cur->next;
    }
    
    return (count == 1);
}

/*
** Deletes the zone if it has no blocks and is not the last of its preallocated type.
** Adjusts the zone list links and unmaps the zone's memory.
*/
void delete_empty_zone(t_zone *zone) {
    if (zone->block_count > 0) return;

    if (zone->prev) zone->prev->next = zone->next;
    if (zone->next) zone->next->prev = zone->prev;
    
    if (!is_last_preallocated_zone(zone)) {
        if (zone == g_malloc_manager) g_malloc_manager = zone->next;
        munmap(zone, zone->size);
        // log_detail(DELETING_ZONE);
    }
}
