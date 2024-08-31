#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

// malloc null
// int main(void)
// {
// 	void *t = malloc(0);
//     	if (t)
//         	printf("malloc(0) should return NULL\n");
//     	free(t);
// 	return (0);
// }

// int main(void)
// {
//     char *t = (char *)malloc(1);
//     if (!t) {
//         printf("malloc(1) should return ptr\n");
//         return (0);
//     }
//     t[0] = 0xFF;
//     t[1] = 0xFF;
//     t[2] = 0xaa;
//     t[3] = 0xFF;
//     t[4] = 0xFF;
//     show_alloc_mem_hex();

//     t[0] = 0;
//     free(t);
// 	return (0);
// }

// z3r4p1% ./run.sh ./test/test1.c test1
// malloc() should return ptr

int main(void)
{
    char *t;
    struct rlimit rpl;

    rpl.rlim_cur = TINY_ZONE_SIZE;
    rpl.rlim_max = TINY_ZONE_SIZE;

    if (setrlimit(RLIMIT_DATA, &rpl) < 0)
        printf("setrlimit did not work\n");

    if (!(t = (char *)malloc(TINY_ZONE_SIZE - sizeof(t_block) - sizeof(t_zone))))
        printf("malloc() should return ptr\n");
    free(t);

    if ((t = (char *)malloc(TINY_ZONE_SIZE)))
        printf("malloc() should return NULL\n");
    free(t);
}

// int main(void)
// {
//     void	*t	= malloc(1);
//     void	*t0	= malloc(TINY);
//     void	*t00	= malloc(TINY);
//     void	*t000	= malloc(TINY);
//     void	*t1	= malloc(SMALL);
//     void	*t2	= malloc(SMALL + 1);

//     // Should print mallocs in all categories (TINY, SMALL, LARGE)
//     show_alloc_mem();
//     // show_alloc_mem_hex();
//     free(t0);
//     printf("size of t_block: %ld ", sizeof(t_block));
//     t0 = malloc(TINY - sizeof(t_block));
//     show_alloc_mem();
//     free(t0);
//     free(t00);
//     free(t000);
//     free(t1);
//     free(t2);
// }

// int main(void)
// {
// 	void *test = malloc(10);
// 	void *test2 = malloc(10);
// 	void *test3 = malloc(10);

// 	free(test);
//     show_alloc_mem();
// 	test2 = realloc(test2, 10);
// 	test2 = realloc(test2, 11);
// 	test2 = realloc(test2, 9);
//     show_alloc_mem();
// 	free(test2);
// 	free(test3);
//     show_alloc_mem();
// }

// int main(void){
// 	void *test = malloc(10);
//      show_alloc_mem();
// 	test = realloc(test, 11);
//      show_alloc_mem();
// 	test = realloc(test, 9);
//      show_alloc_mem();
// 	test = realloc(test, 8);
//     show_alloc_mem();
// }

// int main(void){
// 	char *test = (char*) malloc(20);
// 	char *test2 = (char*) malloc(22);
// 	test[0] = 5;
// 	test[1] = 5;
// 	test[2] = 5;
// 	test[3] = 5;
// 	test[4] = 5;
// 	test[5] = 5;
// 	show_alloc_mem_hex();
// 	free(test);
// 	free(test2);
//      show_alloc_mem();
// }
