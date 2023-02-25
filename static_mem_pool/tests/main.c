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
    uint8_t * ptr_2 = smp_malloc(8);
    memset(ptr_2, 0xBB, 8);
    printf("%d\r\n", get_local(ptr_2));
    print_map();
    uint8_t * ptr_3 = smp_malloc(8);
    memset(ptr_3, 0xCC, 8);
    printf("%d\r\n", get_local(ptr_3));
    print_map();
    uint8_t * ptr_4 = smp_malloc(8);
    memset(ptr_4, 0xDD, 8);
    printf("%d\r\n", get_local(ptr_4));
    print_map();

    printf("Free status 0x%X\r\n", smp_free(ptr_2));
    print_map();

    ptr_2 = smp_malloc(8);
    memset(ptr_2, 0xEE, 8);
    printf("%d\r\n", get_local(ptr_2));
    print_map();

    printf("Free status 0x%X\r\n", smp_free(ptr_1));
    print_map();
    
    ptr_1 = smp_malloc(8);
    memset(ptr_1, 0xFF, 8);
    printf("%d\r\n", get_local(ptr_1));
    print_map();

    printf("Free status 0x%X\r\n", smp_free(ptr_1));
    print_map();
    printf("Free status 0x%X\r\n", smp_free(ptr_2));
    print_map();
    printf("Free status 0x%X\r\n", smp_free(ptr_3));
    print_map();
    printf("Free status 0x%X\r\n", smp_free(ptr_4));
    print_map();

    printf("void * size : %d\r\n", sizeof(void *));
    ptr_1 = smp_malloc(8);
    memset(ptr_1, 0xAA, 8);
    printf("%d\r\n", get_local(ptr_1));
    print_map();
    ptr_2 = smp_malloc(8);
    memset(ptr_2, 0xBB, 8);
    printf("%d\r\n", get_local(ptr_2));
    print_map();
    ptr_3 = smp_malloc(8);
    memset(ptr_3, 0xCC, 8);
    printf("%d\r\n", get_local(ptr_3));
    print_map();
    ptr_4 = smp_malloc(8);
    memset(ptr_4, 0xDD, 8);
    printf("%d\r\n", get_local(ptr_4));
    print_map();

    
    // printf("%d\r\n", get_local(smp_malloc(8)));
    // print_map();
    return 0;
}