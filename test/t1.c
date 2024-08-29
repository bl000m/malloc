// #include <stdbool.h>
// #include <stdio.h>
// #include <string.h>

// #include "malloc.h"

// int main() {
//   char *addr[20];

//   addr[0] = malloc(20);
//   addr[1] = malloc(40);
//   addr[2] = malloc(4);
//   addr[3] = malloc(15);
//   addr[4] = malloc(42);
//   addr[5] = malloc(89);
//   addr[6] = malloc(589);
//   addr[7] = malloc(400);
//   addr[8] = malloc(400);
//   show_alloc_mem();

//   free(addr[0]);
//   free(addr[3]);
//   free(addr[5]);
//   free(addr[7]);
//   show_alloc_mem();

//   addr[0] = malloc(42);
//   addr[3] = malloc(5);
//   addr[5] = malloc(100);
//   show_alloc_mem();

//   addr[9] = realloc(addr[1], 156);
//   show_alloc_mem();
//   cleanup();
//   return 0;
// }


#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "malloc.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void error(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

int main()
{
    void *ptr = malloc(8);
    uintptr_t addr = (uintptr_t)ptr;

    show_alloc_mem();
    if (addr == 0)
        error("malloc returned NULL");
    if ((addr % 0x10) != 0)
        error("malloc returned unaligned address");

    // free(ptr);
    // ptr = malloc(8);
    // if ((uintptr_t)ptr != addr)
    //     error("malloc did not return the same address after free");

    // ptr = realloc(ptr, 1024 * 1024 * 128);
    // if (ptr == NULL)
    //     error("realloc returned NULL with the size of 128M");

    // free(ptr);
    // for (int i = 0; i < 16; i++)
    //     free(malloc(sysconf(_SC_PAGESIZE) * 2 << i)); // try all sizes

    // printf("malloc, free, realloc: OK\n");
    return 0;
}