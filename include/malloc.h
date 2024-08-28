#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <libft.h>
#include <sys/mman.h>      // For mmap, munmap
#include <sys/resource.h>  // For rlimit, getrlimit
#include <stdbool.h>       // For bool, true, false

#define TINY 128
#define SMALL 1024

#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define MIN_BLOCK_SIZE (ALIGN(sizeof(t_block)))

#define TINY_ZONE_SIZE (PAGE_SIZE * (((TINY + ALIGNMENT - 1 + MIN_BLOCK_SIZE) * 100 + PAGE_SIZE - 1) / PAGE_SIZE))
#define SMALL_ZONE_SIZE (PAGE_SIZE * (((SMALL + ALIGNMENT - 1 + MIN_BLOCK_SIZE) * 100 + PAGE_SIZE - 1) / PAGE_SIZE))

typedef enum e_zone {
    TINY_ZONE,
    SMALL_ZONE,
    LARGE_ZONE
} e_zone;

typedef struct s_block {
    size_t size;
    bool free;
    struct s_block *prev;
    struct s_block *next;
} t_block;

typedef struct s_zone {
    size_t size;
    t_block *blocks;
    struct s_zone *prev;
    struct s_zone *next;
    e_zone type;
    size_t block_count;
    size_t free_size;
} t_zone;

// typedef struct s_malloc_manager {
//     t_zone *tiny_zones;
//     t_zone *small_zones;
//     t_zone *large_zones;
//     int initialized;
// } t_malloc_manager;

extern t_zone *g_malloc_manager;
extern pthread_mutex_t g_malloc_mutex;

// Prototypes
void *malloc(size_t size);
void initialize_malloc_manager(void);
void split_block(t_block *block, size_t size);
t_block *find_free_block(t_zone *zone, size_t size);
// t_zone *create_zone(size_t size, e_zone type);
void add_zone(t_zone **zones, t_zone *new_zone);
void *allocate_in_zone(t_zone **zones, size_t zone_size, size_t size);
void *allocate_large_block(size_t size);
void free(void *ptr);
void coalesce_free_blocks(t_zone *zone);
t_zone *find_zone_for_block(t_block *block);
void *realloc(void *ptr, size_t size);
void _optional_abort(const char *message);
void show_alloc_mem(void);
void start_show_alloc_mem(void);
size_t print_block_list(t_block *block);
void cleanup(void);
// Add these prototypes to your header file (e.g., malloc.h)

t_block *find_and_split_block_if_available(size_t size);
t_zone *get_zone_for_needed_block(const size_t size);
void split_block_and_create_free_block(t_block *block, size_t size, t_zone *zone);
t_zone *create_zone(e_zone zone_type, size_t block_size);
void delete_empty_zone(t_zone *zone);
t_block *find_available_block(size_t size, t_zone **zone, t_block **block);
t_block *get_last_block(t_block *block);
void setup_block(t_block *new_block, size_t size);
rlim_t get_system_limit(void);
e_zone get_zone_type_for_size(size_t size);
size_t get_zone_size_from_block_size(size_t block_size);
void log_detail(int detail);
void *BLOCK_SHIFT(t_block *block);
void *zone_SHIFT(t_zone *zone);
size_t get_zone_size_from_zone_type(e_zone zone_type);
size_t get_zone_type_from_block_size(const size_t size);


#endif // MALLOC_H
