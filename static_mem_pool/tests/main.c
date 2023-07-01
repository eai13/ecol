#include <stdio.h>
#include <string.h>
#include "../static_mem_pool.h"

int main(){
    smp_initialize();
    print_map();
    
    uint8_t * ptr_1 = smp_malloc(10);
    memset(ptr_1, 0xAA, 10);
    print_map();
    printf("MALLOC 1 ADDR %d\r\n", ptr_1);
    
    uint8_t * ptr_2 = smp_malloc(5);
    memset(ptr_2, 0xBB, 5);
    print_map();
    printf("MALLOC 2 ADDR %d\r\n", ptr_2);
    
    uint8_t * ptr_3 = smp_malloc(5);
    memset(ptr_3, 0xCC, 5);
    print_map();
    printf("MALLOC 3 ADDR %d\r\n", ptr_3);
    
    uint8_t * ptr_4 = smp_realloc(ptr_1, 11);
    memset(ptr_4 + 10, 0xDD, 1);
    print_map();
    
    return 0;
}