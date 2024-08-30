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

    if (block && zone) 
        split_block_and_create_remaining_free_block(block, size, zone);

    return block;
}


void split_block_and_create_remaining_free_block(t_block *allocated_block, size_t requested_size, t_zone *zone) 
{
    t_block *remaining_free_block;

    initialize_new_free_block_in_remaining_space(&remaining_free_block, allocated_block, requested_size);

    update_allocated_block_and_zone_metadata(zone, allocated_block, remaining_free_block, requested_size);
}





t_block *get_last_block(t_block *block) {
    while (block->next)
        block = block->next;
    return block;
}


// void *allocate_block_in_zone(t_zone *zone, size_t size)
// {
// 	t_block	*new_block;
// 	t_block	*last_block;

// 	new_block = (t_block *)SKIP_ZONE_METADATA(zone);
// 	last_block = NULL;
// 	if (zone->block_count)
// 	{
// 		last_block = get_last_block(new_block);
// 		new_block =
// 			(t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);
// 	}
// 	setup_block(new_block, size);
// 	if (zone->block_count)
// 	{
// 		last_block->next = new_block;
// 		new_block->prev = last_block;
// 	}
// 	zone->block_count++;
// 	zone->free_size -= (new_block->size + sizeof(t_block));
// 	return ((void *)SKIP_BLOCK_METADATA(new_block));
// }


void *allocate_block_in_zone(t_zone *zone, size_t size) {
    t_block *new_block;
    t_block *last_block = NULL;

    new_block = (t_block *)SKIP_ZONE_METADATA(zone); // if zone empty block allocated at beginning of zone

    if (zone->block_count) {
        last_block = get_last_block(new_block);
        new_block = position_block_after_last(last_block);

        // new_block = (t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);
        link_block_to_last(new_block, last_block);
    }
    // else {
    //     zone->block_count++;
    // }
    setup_block(new_block, size);
    update_zone_metadata(zone, new_block);

    return (void *)SKIP_BLOCK_METADATA(new_block);
}


t_block *position_block_after_last(t_block *last_block)
{
    return (t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);

}


void link_block_to_last(t_block *new_block, t_block *last_block) {
    new_block = (t_block *)(SKIP_BLOCK_METADATA(last_block) + last_block->size);
    last_block->next = new_block;
    new_block->prev = last_block;
}

void update_zone_metadata(t_zone *zone, t_block *new_block) {
    zone->block_count++;
    zone->free_size -= (new_block->size + sizeof(t_block));
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
    if (size <= TINY)
        return TINY_ZONE;
    else if (size <= SMALL)
        return SMALL_ZONE;
    else
        return LARGE_ZONE;
}



size_t calculate_zone_size(size_t size) {
    e_zone zone_type = get_zone_type_from_block_size(size);

    switch (zone_type) {
        case TINY_ZONE:
            return TINY_ZONE_SIZE;
        case SMALL_ZONE:
            return SMALL_ZONE_SIZE;
        case LARGE_ZONE:
        default:
            return size + sizeof(t_zone) + sizeof(t_block);
    }
}

/*
** Creates a new zone of the appropriate size based on the block size and type.
** Returns NULL if the allocation fails or exceeds system limits.
*/
t_zone *create_zone(e_zone zone_type, size_t block_size) {
    size_t zone_size = calculate_zone_size(block_size);

    if (!is_valid_zone_size(zone_size)) {
        return NULL;
    }

    t_zone *zone = map_zone_memory(zone_size);
    if (!zone) {
        return NULL;
    }

    initialize_zone(zone, zone_type, zone_size);
    return zone;
}


bool is_valid_zone_size(size_t zone_size) {
    return zone_size <= get_system_limit();
}

t_zone *map_zone_memory(size_t zone_size) {
    t_zone *zone = (t_zone *)mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    return (zone != MAP_FAILED) ? zone : NULL;
}

void initialize_zone(t_zone *zone, e_zone zone_type, size_t zone_size) {
    ft_bzero(zone, sizeof(t_zone));
    zone->type = zone_type;
    zone->size = zone_size;
    zone->free_size = zone_size - sizeof(t_zone);
    zone->block_count = 0;
}



/*
** Checks if the zone is the last one of its type that was preallocated.
** Returns true if it is, false otherwise.
*/
// bool is_last_preallocated_zone(const t_zone *zone) {
//     if (zone->type == LARGE_ZONE) {
//         return false;
//     }

//     int zone_type_count = count_zones_of_type(zone->type);
//     return is_only_one_zone_of_type(zone_type_count);
// }


bool	is_last_preallocated_zone(t_zone *zone)
{
	t_zone			*zone_el;
	e_zone	type;
	int				i;

	zone_el = g_zone_list;
	type = zone->type;
	if (type == LARGE_ZONE)
		return (false);
	i = 0;
	while (zone_el)
	{
		if (zone_el->type == type)
			i++;
		zone_el = zone_el->next;
	}
	return (i == 1);
}


int count_zones_of_type(e_zone type) {
    int count = 0;
    t_zone *current_zone = g_zone_list;

    while (current_zone) {
        if (current_zone->type == type) {
            count++;
        }
        current_zone = current_zone->next;
    }

    return count;
}

bool is_only_one_zone_of_type(int count) {
    return count == 1;
}


/*
** Deletes the zone if it has no blocks and is not the last of its preallocated type.
** Adjusts the zone list links and unmaps the zone's memory.
*/
// void delete_empty_zone(t_zone *zone) {
//     if (zone->block_count > 0) return;

//     unlink_zone_from_list(zone);

//     if (!is_last_preallocated_zone(zone)) {
//         if (zone == g_zone_list)
//             g_zone_list = zone->next;
//         remove_zone_from_memory(zone);
//     }
// }


void			delete_empty_zone(t_zone *zone)
{
	if (zone->block_count)
		return ;
	if (zone->prev)
		zone->prev->next = zone->next;
	if (zone->next)
		zone->next->prev = zone->prev;
	if (!is_last_preallocated_zone(zone))
	{
		if (zone == g_zone_list)
			g_zone_list = zone->next;
		munmap(zone, zone->size);
		// log_detail(zone_DELETE);
	}
}

void unlink_zone_from_list(t_zone *zone) {
    if (zone->prev) {
        zone->prev->next = zone->next;
    }
    if (zone->next) {
        zone->next->prev = zone->prev;
    }

    // Handle the case where the zone is the head of the list
    if (zone == g_zone_list) {
        g_zone_list = zone->next;
    }
}

void remove_zone_from_memory(t_zone *zone) {
    if (!is_last_preallocated_zone(zone)) {
        munmap(zone, zone->size);
        // log_detail(DELETING_ZONE);
    }
}