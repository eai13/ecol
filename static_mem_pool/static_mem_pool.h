#ifndef _ECOL_STATIC_MEM_POOL_H_
#define _ECOL_STATIC_MEM_POOL_H_

#include <stdint.h>
#include <stdio.h>

#define STATIC_MEM_POOL_SIZE    56

void smp_initialize(void);

void * smp_malloc(uint16_t size);

uint32_t get_local(void * ptr);
void print_map(void);

// void * smp_malloc_m(uint16_t size);

// void * smp_realloc(void * p_chunk, uint16_t size);
// void * smp_realloc_m(void * p_chunk, uint16_t size);
// void * smp_realloc_leg(void * p_chunk, uint16_t size);

// void smp_free(void * p_chunk);

#endif