#include "malloc.h"

t_zone *g_zone_list = {0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;


void *malloc(size_t size) 
{
    if (!validate_allocation_size(size)) return NULL;

    pthread_mutex_lock(&g_malloc_mutex);
    size = ALIGN(size);
    void *memory_address_allocated = try_allocate_block(size);
    clear_memory_contents(memory_address_allocated, size);
    pthread_mutex_unlock(&g_malloc_mutex);

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


/* ** Finds an available block and zone for the given size ** */
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

/* ** Finds and splits the block if available ** */
t_block *find_and_split_block_if_available(size_t size) 
{
    t_block *block = NULL;
    t_zone *zone = NULL;

    find_available_block(size, &zone, &block);

    if (block) 
        split_block_and_create_free_block(block, size, zone);

    return block;
}


void split_block_and_create_free_block(t_block *block, size_t size, t_zone *zone) {
    t_block *free_block;

    // Calculate the position of the new free block
    free_block = SKIP_BLOCK_METADATA(block) + size;

    // Set up the new free block with the remaining space
    setup_block(free_block, block->next - free_block);
    free_block->free = true;
	free_block->prev = block;
	free_block->next = block->next;
	zone->block_count++;
	block->next = free_block;
	block->size = size;
	block->free = false;


}

void setup_block(t_block *block,  size_t size) {
	block->prev = NULL;
	block->next = NULL;
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
        if (zone->type == zone_type && zone->free_size >= required_size) {
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

    default_zone = g_zone_list;
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
        }
        g_zone_list = zone;
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

t_block *get_last_block(t_block *block) {
    while (block->next)
        block = block->next;
    return block;
}


void *allocate_block_in_zone(t_zone *zone, size_t size)
{
	t_block	*new_block;
	t_block	*last_block;

	new_block = (t_block *)SKIP_ZONE_METADATA(zone);
	last_block = NULL;
	if (zone->block_count)
	{
		last_block = get_last_block(new_block);
		new_block =
			(t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);
	}
	setup_block(new_block, size);
	if (zone->block_count)
	{
		last_block->next = new_block;
		new_block->prev = last_block;
	}
	zone->block_count++;
	zone->free_size -= (new_block->size + sizeof(t_block));
	return ((void *)SKIP_BLOCK_METADATA(new_block));
}





/*
** Retrieves the system limit for data segment size.
*/
rlim_t get_system_limit(void) {
	struct rlimit rpl;

	if (getrlimit(RLIMIT_DATA, &rpl) < 0)
		return (-1);
	return (rpl.rlim_max);
}

e_zone get_zone_type_from_block_size(const size_t size) {
    if (size <= (size_t)TINY)
        return TINY_ZONE;
    else if (size <= (size_t)SMALL)
        return SMALL_ZONE;
    else
        return LARGE_ZONE;
}

size_t			get_zone_size_from_block_size(size_t size)
{
	e_zone zone_type;

	zone_type = get_zone_type_from_block_size(size);
	if (zone_type == TINY_ZONE)
		return ((size_t)TINY_ZONE_SIZE);
	else if (zone_type == SMALL_ZONE)
		return ((size_t)SMALL_ZONE_SIZE);
	return (size + sizeof(t_zone) + sizeof(t_block));
}

/*
** Creates a new zone of the appropriate size based on the block size and type.
** Returns NULL if the allocation fails or exceeds system limits.
*/
t_zone *create_zone(e_zone zone_type, size_t block_size) {
    
    size_t zone_size = get_zone_size_from_block_size(block_size);
    
    if (zone_size > get_system_limit()) return NULL;

    t_zone *zone = (t_zone *)mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
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

    t_zone *cur = g_zone_list;
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
        if (zone == g_zone_list) g_zone_list = zone->next;
        munmap(zone, zone->size);
        // log_detail(DELETING_ZONE);
    }
}
