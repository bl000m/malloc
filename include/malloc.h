#ifndef MALLOC_H
#define MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h> // For size_t
#include <string.h> // for memset
#include <stdint.h> // for uintptr_t
#include <stdlib.h>
#include <libft.h>

#define TINY 128
#define SMALL 1024

// Macro to retrieve the system's page size at runtime
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))

#define ALIGNMENT 16       // Memory alignment requirement
#define BLOCK_OVERHEAD sizeof(t_block)  // Size of the t_block structure, which includes metadata
#define MIN_BLOCKS_PER_ZONE 100         // Minimum number of blocks per zone

// Calculate zone sizes to accommodate at least MIN_BLOCKS_PER_ZONE blocks
// Each block needs to be aligned to ALIGNMENT bytes and include BLOCK_OVERHEAD for metadata
// Ensure that the zone size is a multiple of the page size for proper memory management

// Calculate TINY_ZONE_SIZE to fit at least MIN_BLOCKS_PER_ZONE blocks of size TINY
// Add ALIGNMENT - 1 to TINY to ensure proper alignment, and add BLOCK_OVERHEAD for metadata
// The zone size is rounded up to the nearest multiple of PAGE_SIZE, ensuring efficient allocation
#define TINY_ZONE_SIZE (PAGE_SIZE * (((TINY + ALIGNMENT - 1 + BLOCK_OVERHEAD) * MIN_BLOCKS_PER_ZONE + PAGE_SIZE - 1) / PAGE_SIZE))

// Calculate SMALL_ZONE_SIZE to fit at least MIN_BLOCKS_PER_ZONE blocks of size SMALL
// Add ALIGNMENT - 1 to SMALL to ensure proper alignment, and add BLOCK_OVERHEAD for metadata
// The zone size is rounded up to the nearest multiple of PAGE_SIZE, ensuring efficient allocation
#define SMALL_ZONE_SIZE (PAGE_SIZE * (((SMALL + ALIGNMENT - 1 + BLOCK_OVERHEAD) * MIN_BLOCKS_PER_ZONE + PAGE_SIZE - 1) / PAGE_SIZE))

typedef enum e_zone {
    TINY_ZONE,
    SMALL_ZONE,
    LARGE_ZONE
} e_zone;

typedef struct s_block {
    size_t size;
    int free;
    struct s_block *next;
} t_block;

typedef struct s_zone {
    size_t size;
    t_block *blocks;
    struct s_zone *next;
    e_zone type;
} t_zone;

typedef struct s_malloc_manager {
    t_zone *tiny_zones;
    t_zone *small_zones;
    t_zone *large_zones;
    int initialized;
} t_malloc_manager;

extern t_malloc_manager g_malloc_manager;
extern pthread_mutex_t g_malloc_mutex;


// Prototypes
void *malloc(size_t size);
void initialize_malloc_manager(void);
void split_block(t_block *block, size_t size);
t_block *find_free_block(t_zone *zone, size_t size);
t_zone *create_zone(size_t size);
void add_zone(t_zone **zones, t_zone *new_zone);
void *allocate_in_zone(t_zone **zones, size_t zone_size, size_t size);
void *allocate_large_block(t_zone **zones, size_t size);
// size_t print_zone(t_zone *zone);
// size_t print_large_zone(t_zone *zone);
void free(void *ptr);
void coalesce_free_blocks(t_zone *zone);
t_zone *find_zone_for_block(t_block *block);
void *realloc(void *ptr, size_t size);
void _optional_abort(const char *message);
void show_alloc_mem();
void start_show_alloc_mem(void);
size_t print_block_list(t_block *block);
void cleanup(void);
// void print_zone(const char *name, t_zone *zone);



#endif // MALLOC_H