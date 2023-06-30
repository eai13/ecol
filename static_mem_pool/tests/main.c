#include <stdio.h>
#include <string.h>
#include "../static_mem_pool.h"

int main(){
    smp_initialize();
    print_map();
    uint8_t * ptr_1 = smp_malloc(16);
    memset(ptr_1, 0xAA, 16);
    print_map();

    printf("status %d\r\n",ptr_1 == smp_realloc(ptr_1, 4));
    print_map();

    uint8_t * ptr_2 = smp_malloc(6);
    memset(ptr_2, 0xBB, 6);
    print_map();
    return 0;
}