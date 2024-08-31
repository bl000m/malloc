#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include <stdlib.h>
#include <stdint.h>

// void error(const char *msg)
// {
//     fprintf(stderr, "Error: %s\n", msg);
//     exit(1);
// }

// int main()
// {
//     void	*t	= malloc(1);
//     void	*tb	= malloc(2);
//     void	*tc	= malloc(3);
//     void	*td	= malloc(4);
//     void	*te	= malloc(5);
//     void	*t0	= malloc(129);
//     void	*t00	= malloc(200);
//     void	*t000	= malloc(300);
//     void	*t1	= malloc(SMALL);
//     void	*t2	= malloc(SMALL + 1);
//     void	*t3	= malloc(SMALL + 2);

//     show_alloc_mem();
//     // show_alloc_mem_hex();
   
//     free(t);
//     free(t0);
//     free(t00);
//     free(t000);
//     free(t1);
//     free(t2);
//     free(tb);
//     free(tc);
//     free(td);
//     free(te);
//     free(t3);
//     show_alloc_mem();

//     void *ptr = malloc(8);
//     uintptr_t addr = (uintptr_t)ptr;

//     show_alloc_mem();
//     if (addr == 0)
//         error("malloc returned NULL");
//     if ((addr % 0x10) != 0)
//         error("malloc returned unaligned address");

//     free(ptr);
//     show_alloc_mem();

//     ptr = malloc(8);
//     if ((uintptr_t)ptr != addr)
//         error("malloc did not return the same address after free");

//     ptr = realloc(ptr, 1024 * 1024 * 128);
//     if (ptr == NULL)
//         error("realloc returned NULL with the size of 128M");
//     show_alloc_mem();

//     free(ptr);
//     // for (int i = 0; i < 16; i++){
//     //     free(malloc(4096 * 2 << i)); // try all sizes
//     // }
//     printf("malloc, free, realloc: OK\n");
//     return 0;
// }

// int		main(void)
// {
// 	int		i;
// 	char	*addr;

// 	i = 0;
// 	while (i < 1024)
// 	{
// 		i++;
// 	}
// 	return (0);
// }


// int		main(void)
// {
// 	int		i;
// 	char	*addr;

// 	i = 0;
// 	while (i < 1024)
// 	{
// 		addr = (char*)malloc(1024);
// 		addr[0] = 42;
// 		i++;
// 	}
// 	return (0);
// }


// int		main(void)
// {
// 	int		i;
// 	char	*addr;

// 	i = 0;
// 	while (i < 1024)
// 	{
// 		addr = (char*)malloc(1024);
// 		addr[0] = 42;
// 		free(addr);
// 		i++;
// 	}
// 	return (0);
// }

// #define M (1024 * 1024)

// void print(char *s)
// {
// 	write(1, s, strlen(s));
// }

// int main()
// {
// 	char *addr1;
// 	char *addr3;

// 	addr1 = (char*)malloc(16*M);
// 	strcpy(addr1, "Bonjours\n");
// 	print(addr1);
// 	addr3 = (char*)realloc(addr1, 128*M);
// 	addr3[127*M] = 42;
// 	print(addr3);
// 	return (0);
// }

// #define M (1024 * 1024)

// void print(char *s)
// {
// 	write(1, s, strlen(s));
// }

// int main()
// {
// 	char *addr1;
// 	char *addr2;
// 	char *addr3;

// 	addr1 = (char*)malloc(16*M);
// 	strcpy(addr1, "Bonjours\n");
// 	print(addr1);
// 	addr2 = (char*)malloc(16*M);
// 	addr3 = (char*)realloc(addr1, 128*M);
// 	addr3[127*M] = 42;
// 	print(addr3);
// 	return (0);
// }

// void print(char *s)
// {
// 	write(1, s, strlen(s));
// }

// int		main()
// {
// 	char *addr;

// 	addr = malloc(16);
// 	free(NULL);
// 	free((void *)addr + 5);
// 	if (realloc((void *)addr + 5, 10) == NULL)
// 		print("Bonjours\n");
// }

int main()
{
	malloc(1024);
	malloc(1024 * 32);
	malloc(1024 * 1024);
	malloc(1024 * 1024 * 16);
	malloc(1024 * 1024 * 128);
	show_alloc_mem();
	return (0);
}