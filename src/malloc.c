#include "malloc.h"

// Define global variable
t_malloc_manager g_malloc_manager = {0};

void initialize_malloc_manager(void) {
    g_malloc_manager.tiny_zones = NULL;
    g_malloc_manager.small_zones = NULL;
    g_malloc_manager.large_zones = NULL;
    pthread_mutex_init(&g_malloc_manager.malloc_lock, NULL);
    g_malloc_manager.initialized = 1;
}

void *malloc(size_t size) {
    if (!g_malloc_manager.initialized) {
        pthread_mutex_lock(&g_malloc_manager.malloc_lock);
        if (!g_malloc_manager.initialized) { // Double-checked locking
            initialize_malloc_manager();
        }
        pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
    }

    pthread_mutex_lock(&g_malloc_manager.malloc_lock);

    void *ptr = NULL;
    if (size <= TINY) {
        ptr = allocate_in_zone(&g_malloc_manager.tiny_zones, TINY_ZONE_SIZE, size);
    } else if (size <= SMALL) {
        ptr = allocate_in_zone(&g_malloc_manager.small_zones, SMALL_ZONE_SIZE, size);
    } else {
        ptr = allocate_large_block(&g_malloc_manager.large_zones, size);
    }

    pthread_mutex_unlock(&g_malloc_manager.malloc_lock);
    return ptr;
}

t_zone *create_zone(size_t size) {
    t_zone *zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (zone == MAP_FAILED) return NULL;

    zone->size = size;
    zone->blocks = (t_block*)((char*)zone + sizeof(t_zone));
    zone->blocks->size = size - sizeof(t_zone) - sizeof(t_block);
    zone->blocks->free = 1;
    zone->blocks->next = NULL;
    zone->next = NULL;

    return zone;
}

void add_zone(t_zone **zones, t_zone *new_zone) {
    if (!*zones) {
        *zones = new_zone;
    } else {
        t_zone *current = *zones;
        while (current->next) {
            current = current->next;
        }
        current->next = new_zone;
    }
}

t_block *find_free_block(t_zone *zone, size_t size) {
    t_block *current = zone->blocks;
    while (current && !(current->free && current->size >= size)) {
        current = current->next;
    }
    return current;
}

void *allocate_in_zone(t_zone **zones, size_t zone_size, size_t size) {
    t_zone *current_zone = *zones;
    t_block *block = NULL;

    while (current_zone) {
        block = find_free_block(current_zone, size);
        if (block) break;
        current_zone = current_zone->next;
    }

    if (!block) {
        t_zone *new_zone = create_zone(zone_size);
        if (!new_zone) return NULL;
        add_zone(zones, new_zone);
        block = new_zone->blocks;
    }

    block->free = 0;
    split_block(block, size);
    return (void*)(block + 1);
}

void *allocate_large_block(t_zone **zones, size_t size) {
    size_t total_size = size + sizeof(t_block);
    t_block *block = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) return NULL;

    block->size = size;
    block->free = 0;
    block->next = NULL;

    t_zone *zone = mmap(NULL, sizeof(t_zone), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (zone == MAP_FAILED) {
        munmap(block, total_size);
        return NULL;
    }

    zone->size = total_size;
    zone->blocks = block;
    zone->next = NULL;
    add_zone(zones, zone);

    return (void*)(block + 1);
}

void split_block(t_block *block, size_t size) {
    if (block->size > size + sizeof(t_block)) {
        t_block *new_block = (t_block*)((char*)(block + 1) + size);
        new_block->size = block->size - size - sizeof(t_block);
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;
    }
}
