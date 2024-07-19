#ifndef MALLOC_H
#define MALLOC_H

// #include <stdio.h> // size_t
#include <unistd.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define TINY 128
#define SMALL 1024


// Prototypes

void *malloc(size_t size);

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

t_zone *tiny_zones = NULL;
t_zone *small_zones = NULL;
t_zone *large_zones = NULL;

pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

#endif // MALLOC_H