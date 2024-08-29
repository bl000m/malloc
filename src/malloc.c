#include "malloc.h"

// Define global variable
t_zone *g_malloc_manager = {0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

// void	*malloc(size_t size)
// {
// 	void *res;
// 	pthread_mutex_lock(&g_malloc_mutex);
// 	// log_detail(MALLOC);
// 	if ((res = start_malloc(size)))
// 		ft_memset(res, 0xaa, size);
// 	pthread_mutex_unlock(&g_malloc_mutex);
// 	return (res);
// }

// void *start_malloc(size_t size) {
//     t_zone *zone;
//     t_block *block;
//     void *memory_address_allocated;

//     if (size <= 0) return NULL;

//     // Ensure size is aligned
//     size = (size + 15) & ~15;

//     // Attempt to find and split an available block
//     block = find_and_split_block_if_available(size);
//     if (block) {
//         return (BLOCK_SHIFT(block));
//     }

//     // Find or create a suitable zone
//     zone = get_zone_for_needed_block((const size_t)size);
//     if (!zone) {
//         return NULL; // Return NULL if no zone could be created
//     }

//     // Allocate a block within the zone
//     memory_address_allocated = append_empty_block(zone, size);
//     if (!memory_address_allocated) {
//         return NULL; // Return NULL if allocation within the zone failed
//     }

//     return memory_address_allocated;
// }

void *malloc(size_t size) {
    void *res = NULL;
    pthread_mutex_lock(&g_malloc_mutex);

    if (size > 0) {
        size = ALIGN(size);  // Ensure size is aligned
        t_block *block = find_and_split_block_if_available(size);
        if (block) {
            res = (void *)BLOCK_SHIFT(block);
        } else {
            t_zone *zone = get_or_create_zone(size);
            if (zone) {
                res = allocate_block_in_zone(zone, size);
            }
        }
        if (res) {
            ft_memset(res, 0xaa, size);  // Optionally initialize memory
        }
    }

    pthread_mutex_unlock(&g_malloc_mutex);
    return res;
}

t_zone *get_or_create_zone(size_t size) {
    t_zone *default_zone = g_malloc_manager;
    e_zone zone_type = get_zone_type_from_block_size(size);
    t_zone *zone = find_available_zone(default_zone, zone_type, size + sizeof(t_block));
    if (!zone) {
        zone = create_zone(zone_type, size);
        if (zone) {
            zone->next = g_malloc_manager;
            if (g_malloc_manager) {
                g_malloc_manager->prev = zone;
            }
            g_malloc_manager = zone;
        }
    }
    return zone;
}



void find_available_block(size_t size, t_zone **res_zone, t_block **res_block) {
    t_zone *zone;
    t_block *block;
    e_zone type;

    zone = g_malloc_manager;
    type = get_zone_type_from_block_size(size);
    while (zone) {
        block = (t_block *)ZONE_SHIFT(zone);
        while (zone->type == type && block) {
            if (block->free && (block->size >= size + sizeof(t_block))) {
                *res_zone = zone;
                *res_block = block;
                return;
            }
            block = block->next;
        }
        zone = zone->next;
    }
    *res_zone = NULL;
    *res_block = NULL;
}

t_block *find_and_split_block_if_available(size_t size) {
    t_block *block;
    t_zone *zone;

    // Find an available block of the requested size
    find_available_block(size, &zone, &block);

    if (block && zone) {
        split_block_and_create_free_block(block, size, zone);
        return block;
    }
    return NULL;
}

void split_block_and_create_free_block(t_block *block, size_t size, t_zone *zone) {
    t_block *free_block;

    // Calculate the position of the new free block
    free_block = BLOCK_SHIFT(block) + size;

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
        }
        g_malloc_manager = zone;
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

	new_block = (t_block *)ZONE_SHIFT(zone);
	last_block = NULL;
	if (zone->block_count)
	{
		last_block = get_last_block(new_block);
		new_block =
			(t_block *)(BLOCK_SHIFT(last_block) + last_block->size);
	}
	setup_block(new_block, size);
	if (zone->block_count)
	{
		last_block->next = new_block;
		new_block->prev = last_block;
	}
	zone->block_count++;
	zone->free_size -= (new_block->size + sizeof(t_block));
	return ((void *)BLOCK_SHIFT(new_block));
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

// size_t get_zone_size_from_zone_type(e_zone zone_type) {
//     if (zone_type == TINY_ZONE)
//         return TINY_ZONE_SIZE;
//     else if (zone_type == SMALL_ZONE)
//         return SMALL_ZONE_SIZE;
//     else
//         return 0;
// }

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
