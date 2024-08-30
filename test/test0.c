#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "malloc.h"

int main() {
    char *addr;
    addr = malloc(2);
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
  show_alloc_mem();
//   show_alloc_mem_hex();

//   free(addr[0]);
//   free(addr[3]);
//   free(addr[5]);
//   free(addr[7]);
//   show_alloc_mem();

//   addr[0] = malloc(42);
//   addr[3] = malloc(5);
//   addr[5] = malloc(100);
//   show_alloc_mem();

//   strncpy(addr[1], "This is a test", 30);
//   printf("This is the content just written on addr[1]: |%s|\n", addr[1]);

//   addr[9] = realloc(addr[1], 156);
//   show_alloc_mem();

//   printf("We reallocced 156 bytes on the 40 of addr[1] that becomes addr[9] but content is still what we strncpied into it: |%s|\n", addr[9]);
//   printf("in fact this is still the content of addr[1] before that someone allocate it again (it has been freed by realloc): |%s|\n",
//       addr[1]);
  return 0;
}