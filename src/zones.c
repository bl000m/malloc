#include "malloc.h"


t_zone *find_existing_zone(size_t size, e_zone zone_type) 
{
    t_zone *default_zone = g_zone_list;
    return find_available_zone(default_zone, zone_type, size + sizeof(t_block));
}



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



t_zone *create_and_add_zone(e_zone zone_type, size_t size) 
{
    t_zone *zone = create_zone(zone_type, size);

    if (zone) {
        add_zone_to_list(zone);
    }

    return zone;
}



t_zone *create_zone(e_zone zone_type, size_t block_size) 
{
    size_t zone_size = calculate_zone_size(block_size);

    if (!is_valid_zone_size(zone_size)) {
        return NULL;
    }

    t_zone *zone = map_zone_memory(zone_size);
    if (!zone) {
        return NULL;
    }

    initialize_zone(zone, zone_type, zone_size);
    log_detail(ZONE_CREATED);
    return zone;
}



bool is_valid_zone_size(size_t zone_size) 
{
    return zone_size <= get_system_limit();
}



t_zone *map_zone_memory(size_t zone_size) 
{
    t_zone *zone = (t_zone *)mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    return (zone != MAP_FAILED) ? zone : NULL;
}



void initialize_zone(t_zone *zone, e_zone zone_type, size_t zone_size) 
{
    ft_bzero(zone, sizeof(t_zone));
    zone->type = zone_type;
    zone->size = zone_size;
    zone->free_size = zone_size - sizeof(t_zone);
    zone->block_count = 0;
}



bool is_last_preallocated_zone(const t_zone *zone) 
{
    if (zone->type == LARGE_ZONE) {
        return false;
    }

    int zone_type_count = count_zones_of_type(zone->type);
    return is_only_one_zone_of_type(zone_type_count);
}



int count_zones_of_type(e_zone type) 
{
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



bool is_only_one_zone_of_type(int count) 
{
    return count == 1;
}



void add_zone_to_list(t_zone *zone) 
{
    zone->next = g_zone_list;
    if (g_zone_list) {
        g_zone_list->prev = zone;
    }
    g_zone_list = zone;
}



void delete_empty_zone(t_zone *zone) 
{
    if (zone->block_count > 0) return;

    unlink_zone_from_list(zone);

    if (!is_last_preallocated_zone(zone)) {
        if (zone == g_zone_list)
            g_zone_list = zone->next;
        remove_zone_from_memory(zone);
    }
}



void unlink_zone_from_list(t_zone *zone) 
{
    if (zone->prev) {
        zone->prev->next = zone->next;
    }
    if (zone->next) {
        zone->next->prev = zone->prev;
    }

    if (zone == g_zone_list) {
        g_zone_list = zone->next;
    }
}



void remove_zone_from_memory(t_zone *zone) 
{
    if (!is_last_preallocated_zone(zone)) {
        munmap(zone, zone->size);
        log_detail(ZONE_DELETED);
    }
}



void update_zone_metadata(t_zone *zone, t_block *new_block) 
{
    zone->block_count++;
    zone->free_size -= (new_block->size + sizeof(t_block));
}



t_zone *get_the_last_zone(t_zone *head)
{
    t_zone *last_zone = head;
    
    while (last_zone && last_zone->next)
    {
        last_zone = last_zone->next;
    }
    
    return last_zone;
}
