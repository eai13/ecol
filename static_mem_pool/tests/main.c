#include <stdio.h>
#include <string.h>
#include "../static_mem_pool.h"

int main(){
    smp_initialize();
    print_map();
    printf("void * size : %d\r\n", sizeof(void *));
    uint8_t * ptr_1 = smp_malloc(8);
    memset(ptr_1, 0xAA, 8);
    printf("%d\r\n", get_local(ptr_1));
    print_map();
    ptr_1 = smp_malloc(8);
    memset(ptr_1, 0xBB, 8);
    printf("%d\r\n", get_local(ptr_1));
    print_map();
    // printf("%d\r\n", get_local(smp_malloc(8)));
    // print_map();
    return 0;
}