#ifndef MALLOC_H
#define MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h> // For size_t
#include <string.h>

#define TINY 128
#define SMALL 1024




#define TINY_ZONE_SIZE (getpagesize() * 2)
#define SMALL_ZONE_SIZE (getpagesize() * 4)

typedef struct s_block {
    size_t size;
    int free;
    struct s_block *next;
} t_block;

typedef struct s_zone {
    size_t size;
    t_block *blocks;
    struct s_zone *next;
} t_zone;

typedef struct s_malloc_manager {
    t_zone *tiny_zones;
    t_zone *small_zones;
    t_zone *large_zones;
    pthread_mutex_t malloc_lock;
    int initialized;
} t_malloc_manager;

extern t_malloc_manager g_malloc_manager;


// Prototypes
void *malloc(size_t size);
void initialize_malloc_manager(void);
void split_block(t_block *block, size_t size);
t_block *find_free_block(t_zone *zone, size_t size);
t_zone *create_zone(size_t size);
void add_zone(t_zone **zones, t_zone *new_zone);
void *allocate_in_zone(t_zone **zones, size_t zone_size, size_t size);
void *allocate_large_block(t_zone **zones, size_t size);

#endif // MALLOC_H