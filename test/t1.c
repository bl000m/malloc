#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "malloc.h"

int main() {
  char *addr[20];

  addr[0] = malloc(20);
  addr[1] = malloc(40);
  addr[2] = malloc(4);
  addr[3] = malloc(15);
  addr[4] = malloc(42);
  addr[5] = malloc(89);
  addr[6] = malloc(589);
  addr[7] = malloc(400);
  addr[8] = malloc(400);
  show_alloc_mem();

  free(addr[0]);
  free(addr[3]);
  free(addr[5]);
  free(addr[7]);
  show_alloc_mem();

  addr[0] = malloc(42);
  addr[3] = malloc(5);
  addr[5] = malloc(100);
  show_alloc_mem();

  addr[9] = realloc(addr[1], 156);
  show_alloc_mem();
  cleanup();
  return 0;
}
