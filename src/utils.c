#include "malloc.h"


void clear_memory_contents(void *ptr, size_t size) 
{
    if (ptr) {
        ft_memset(ptr, 0xaa, size);
    }
}



bool validate_allocation_size(size_t size)
{
    size_t aligned_size = (size + 15) & ~15;
    rlim_t system_limit = get_system_limit();

    if (aligned_size <= 0 || aligned_size > system_limit) {
        return false;
    }
    return true;
}



void *address_after_metadata(t_block *block) 
{
    if (block) {
        return (void *)SKIP_BLOCK_METADATA(block);
    }
    return NULL;
}



rlim_t get_system_limit(void) 
{
	struct rlimit rpl;

	if (getrlimit(RLIMIT_DATA, &rpl) < 0)
		return (-1);
	return (rpl.rlim_max);
}



e_zone get_zone_type_from_block_size(const size_t size) 
{
    if (size <= TINY)
        return TINY_ZONE;
    else if (size <= SMALL)
        return SMALL_ZONE;
    else
        return LARGE_ZONE;
}



size_t calculate_zone_size(size_t size) 
{
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