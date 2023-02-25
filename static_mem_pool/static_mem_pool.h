#ifndef _ECOL_STATIC_MEM_POOL_H_
#define _ECOL_STATIC_MEM_POOL_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STATIC_MEM_POOL_SIZE    56

void smp_initialize(void);

typedef enum{
    SMP_FREE_OK                 = ((uint8_t)(0x00)),
    SMP_FREE_NOT_IN_POOL        = ((uint8_t)(0x01)),
    SMP_FREE_ALREADY_FREED      = ((uint8_t)(0x02)),
    SMP_FREE_IS_RESOURCE_PTR    = ((uint8_t)(0x03))
}smp_free_status_e;

void *              smp_malloc(uint16_t size);
smp_free_status_e   smp_free(void * ptr);

uint32_t get_local(void * ptr);
void print_map(void);

// void * smp_malloc_m(uint16_t size);

// void * smp_realloc(void * p_chunk, uint16_t size);
// void * smp_realloc_m(void * p_chunk, uint16_t size);
// void * smp_realloc_leg(void * p_chunk, uint16_t size);

// void smp_free(void * p_chunk);

#endif