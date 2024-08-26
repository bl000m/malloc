#include "malloc.h"

// Define global variable
t_malloc_manager g_malloc_manager = {0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;


void initialize_malloc_manager(void) {
    g_malloc_manager.tiny_zones = NULL;
    g_malloc_manager.small_zones = NULL;
    g_malloc_manager.large_zones = NULL;
    g_malloc_manager.initialized = 1;
}

void *malloc(size_t size) {
    if (!g_malloc_manager.initialized) {
        // pthread_mutex_lock(&g_malloc_mutex);
        if (!g_malloc_manager.initialized) { // Double-checked locking
            initialize_malloc_manager();
        }
        // pthread_mutex_unlock(&g_malloc_mutex);
    }

    // pthread_mutex_lock(&g_malloc_mutex);

    size = (size + 15) & ~15;

    void *ptr = NULL;
    if (size <= TINY) {
        ptr = allocate_in_zone(&g_malloc_manager.tiny_zones, TINY_ZONE_SIZE, size);
    } else if (size <= SMALL) {
        ptr = allocate_in_zone(&g_malloc_manager.small_zones, SMALL_ZONE_SIZE, size);
    } else {
        ptr = allocate_large_block(&g_malloc_manager.large_zones, size);
    }

    // if (getenv("DEBUG_MALLOC")) {
    //     show_alloc_mem();
    // }
    // pthread_mutex_unlock(&g_malloc_mutex);
    return ptr;
}

void *allocate_in_zone(t_zone **zones, size_t zone_size, size_t size) {
    // pthread_mutex_lock(&g_malloc_mutex);
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
    // pthread_mutex_unlock(&g_malloc_mutex);
    return (void*)(block + 1);
}

void *allocate_large_block(t_zone **zones, size_t size) {
    // pthread_mutex_lock(&g_malloc_mutex);
    size_t total_size = size + sizeof(t_block);
    t_block *block = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) {
        // pthread_mutex_unlock(&g_malloc_mutex);
        return NULL;
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;

    t_zone *zone = mmap(NULL, sizeof(t_zone), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (zone == MAP_FAILED) {
        munmap(block, total_size);
        // pthread_mutex_unlock(&g_malloc_mutex);
        return NULL;
    }

    zone->size = total_size;
    zone->blocks = block;
    zone->next = NULL;
    zone->type = LARGE_ZONE;
    add_zone(zones, zone);

    // pthread_mutex_unlock(&g_malloc_mutex);
    return (void*)(block + 1);
}

t_zone *create_zone(size_t size) {
    t_zone *zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (zone == MAP_FAILED) return NULL;

    zone->size = size;
    size_t alignment_offset = ((uintptr_t)(zone + 1) % 16 == 0) ? 0 : (16 - ((uintptr_t)(zone + 1) % 16));
    zone->blocks = (t_block*)((char*)zone + sizeof(t_zone) + alignment_offset);

    // Ensure there is enough space for the first block
    size_t aligned_size = size - sizeof(t_zone) - alignment_offset;
    zone->blocks->size = aligned_size - sizeof(t_block);
    zone->blocks->free = 1;
    zone->blocks->next = NULL;
    zone->next = NULL;
    if (size == TINY_ZONE_SIZE) {
        zone->type = TINY_ZONE;
    } else if (size == SMALL_ZONE_SIZE) {
        zone->type = SMALL_ZONE;
    } else {
        zone->type = LARGE_ZONE;
    }

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


void split_block(t_block *block, size_t size) {
    if (block->size > size + sizeof(t_block)) {
        uintptr_t block_end = (uintptr_t)(block + 1) + size;
        uintptr_t aligned_block_start = (block_end + 15) & ~15;

        t_block *new_block = (t_block*)aligned_block_start;
        new_block->size = block->size - (aligned_block_start - (uintptr_t)(block + 1)) - sizeof(t_block);
        
        // printf("Splitting block: Original size %zu\n", block->size);
        // printf("New block size: %zu\n", new_block->size);
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;
    }
}
