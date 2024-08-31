#include "malloc.h"


void *realloc(void *old_pointer, size_t new_size)
{
    pthread_mutex_lock(&g_malloc_mutex);

    void *result = check_pointer_and_size(old_pointer, new_size);
    if (result != old_pointer) {
        pthread_mutex_unlock(&g_malloc_mutex);
        return result;
    }

	t_zone	*zone;
	t_block	*block;
	void	*new_pointer;

    log_detail(REALLOC);
	zone = g_zone_list;
	locate_block_by_ptr(&zone, &block, zone, old_pointer);
	if (!zone || !block) {
        pthread_mutex_unlock(&g_malloc_mutex);
        return NULL;
    }
	if (new_size == block->size) {
        pthread_mutex_unlock(&g_malloc_mutex);
        return old_pointer;
    }
    pthread_mutex_unlock(&g_malloc_mutex);
	new_pointer = malloc(new_size);
    if (!new_pointer) {
        return NULL; 
    }
    size_t move_size =  block->size > new_size ? new_size : block->size;
    relocate_and_free(new_pointer, old_pointer, move_size);
	pthread_mutex_unlock(&g_malloc_mutex);

	return new_pointer;
}



void *check_pointer_and_size(void *old_pointer, size_t new_size) 
{
    if (!old_pointer) {
        return malloc(new_size);
    } else if (new_size == 0) {
        free(old_pointer);
        return NULL;
    }
    return old_pointer;
}



void relocate_and_free(void *new_pointer, void *old_pointer, size_t move_size) {
    ft_memmove(new_pointer, old_pointer, move_size);
    free(old_pointer);
}
