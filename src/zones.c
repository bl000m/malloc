#include "malloc.h"

t_zone *get_or_create_zone(size_t size) 
{
    e_zone zone_type = get_zone_type_from_block_size(size);
    t_zone *zone = find_existing_zone(size, zone_type);

    if (!zone) {
        zone = create_and_add_zone(zone_type, size);
    }

    return zone;
}


t_zone *find_existing_zone(size_t size, e_zone zone_type) 
{
    t_zone *default_zone = g_zone_list;
    return find_available_zone(default_zone, zone_type, size + sizeof(t_block));
}


t_zone *create_and_add_zone(e_zone zone_type, size_t size) 
{
    t_zone *zone = create_zone(zone_type, size);

    if (zone) {
        add_zone_to_list(zone);
    }

    return zone;
}


void add_zone_to_list(t_zone *zone) 
{
    zone->next = g_zone_list;
    if (g_zone_list) {
        g_zone_list->prev = zone;
    }
    g_zone_list = zone;
}