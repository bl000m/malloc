#include "malloc.h"

void cleanup(void) {
    pthread_mutex_destroy(&g_malloc_mutex);
}