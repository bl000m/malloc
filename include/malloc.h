#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <libft.h>
#include <sys/mman.h>      
#include <sys/resource.h>  
#include <stdbool.h>     
#include <fcntl.h>  


/* Constants and Macros */

#define TINY 128
#define SMALL 1024
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define MIN_BLOCK_SIZE (ALIGN(sizeof(t_block)))
#define TINY_ZONE_SIZE (PAGE_SIZE * (((TINY + ALIGNMENT - 1 + MIN_BLOCK_SIZE) * 100 + PAGE_SIZE - 1) / PAGE_SIZE))
#define SMALL_ZONE_SIZE (PAGE_SIZE * (((SMALL + ALIGNMENT - 1 + MIN_BLOCK_SIZE) * 100 + PAGE_SIZE - 1) / PAGE_SIZE))

#define SKIP_ZONE_METADATA(ptr) ((void *)((char *)(ptr) + sizeof(t_zone)))
#define SKIP_BLOCK_METADATA(ptr) ((void *)((char *)(ptr) + sizeof(t_block)))

#define GREEN "\033[0;32m"
#define RESET_COLOR "\033[0m\n"

#define LOGS_PATH "/tmp/ft_malloc.log"


/* Data Structures */

typedef enum	e_detail_event {
	MALLOC,
	FREE,
	REALLOC,
	ZONE_CREATED,
	ZONE_DELETED
} t_detail_event;

typedef enum e_zone {
    TINY_ZONE,
    SMALL_ZONE,
    LARGE_ZONE
} e_zone;

typedef struct s_block {
    size_t          size;
    bool            free;
    struct s_block *prev;
    struct s_block *next;
} t_block;

typedef struct s_zone {
    size_t          size;
    struct s_zone  *prev;
    struct s_zone  *next;
    e_zone          type;
    size_t          block_count;
    size_t          free_size;
} t_zone;


/* Global Variables */

extern t_zone           *g_zone_list;
extern pthread_mutex_t   g_malloc_mutex;



/*************** FUNCTIONS ***************/


/* Main Memory Management Functions */

void   *malloc(size_t size);
void    free(void *ptr);
void   *realloc(void *old_pointer, size_t new_size);


/* Zone Management Functions */

t_zone  *get_or_create_zone(size_t size);
t_zone  *create_zone(e_zone zone_type, size_t block_size);
void    delete_empty_zone(t_zone *zone);
t_zone  *find_available_zone(const t_zone *zone_list, const e_zone zone_type, const size_t required_size);
t_zone  *find_existing_zone(size_t size, e_zone zone_type);
t_zone  *create_and_add_zone(e_zone zone_type, size_t size);
void    add_zone_to_list(t_zone *zone);
bool    is_valid_zone_size(size_t zone_size); 
t_zone  *map_zone_memory(size_t zone_size); 
void    initialize_zone(t_zone *zone, e_zone zone_type, size_t zone_size); 
bool    is_last_preallocated_zone(const t_zone *zone); 
int     count_zones_of_type(e_zone type);
bool    is_only_one_zone_of_type(int count); 
void    unlink_zone_from_list(t_zone *zone);
void    remove_zone_from_memory(t_zone *zone);
t_zone  *get_the_last_zone(t_zone *head);


/* Block Management Functions */

void    *allocate_block_in_zone(t_zone *zone, size_t size);
t_block *get_last_block(t_block *block);
void    setup_block(t_block *block,  size_t size);
void    *try_allocate_block(size_t size);
t_block *find_and_split_block_if_available(size_t size);
void    split_block_and_create_remaining_free_block(t_block *allocated_block, size_t requested_size, t_zone *zone);
void    locate_block_by_ptr(t_zone **zone_out, t_block **block_out, t_zone *zone, void *ptr);
void	find_available_block(size_t size, t_zone **res_zone, t_block **res_block);
t_block *get_last_block(t_block *block);
bool    is_block_suitable(const t_block *block, size_t size);
t_block *find_suitable_block(t_zone *zone, size_t size); 
void    initialize_new_free_block_in_remaining_space(t_block **remaining_free_block, t_block *allocated_block, size_t requested_size);
void    update_allocated_block_and_zone_metadata(t_zone *zone, t_block *allocated_block, t_block *remaining_free_block, size_t requested_size);
t_block *create_and_initialize_block(t_zone *zone, size_t size);
void    link_block_to_last(t_block *new_block, t_block *last_block);
void    update_zone_metadata(t_zone *zone, t_block *new_block);
t_block *position_block_after_last(t_block *last_block);
void    remove_last_block_if_needed(t_zone *zone, t_block *block);
t_block *merge_adjacent_blocks(t_zone *zone, t_block *block);
void    try_merge_with_next(t_zone *zone, t_block *block);
t_block *try_merge_with_prev(t_zone *zone, t_block *block);
bool    are_blocks_free(t_zone *zone);


/* Utility Functions */

void    clear_memory_contents(void *ptr, size_t size);
bool    validate_allocation_size(size_t size);
void    *address_after_metadata(t_block *block);
rlim_t  get_system_limit(void);
size_t  calculate_zone_size(size_t block_size);
e_zone  get_zone_type_from_block_size(const size_t size);
void    *check_pointer_and_size(void *old_pointer, size_t new_size);
void    *allocate_and_clear_memory(size_t size);
void    relocate_and_free(void *new_pointer, void *old_pointer, size_t move_size);


/* Logging */

void    show_alloc_mem(void);
void    print_zone_header(const char *zone_name, t_zone *zone);
size_t  print_allocated_blocks(t_block *block);
void    print_block_info(char *start, char *end, size_t size);
void    show_alloc_mem_hex(void);
size_t  dump_zone_memory(t_zone *zone);
void    print_hex_line(char *start, size_t size);
void	log_detail(t_detail_event event);


// void    log_detail(int detail);

#endif // MALLOC_H
