#include "static_mem_pool.h"

#define __NULL          ((void *)0)

#define __BEGIN_LOCAL   (0)
#define __BEGIN_GLOBAL  (smp + __BEGIN_LOCAL)
#define __END_LOCAL     (STATIC_MEM_POOL_SIZE - 1)
#define __END_GLOBAL    (smp + __END_LOCAL)

#define __U8_CAST(ptr) \
    ((uint8_t *)(ptr))
#define __CHUNK_CAST(ptr) \
    ((smp_chunk_header_t *)(ptr))
#define __VOID_CAST(ptr) \
    ((void *)(ptr))

#define __MIDCHUNK_SIZE(chunk_1, chunk_2) \
    (__U8_CAST(chunk_2) - (__U8_CAST(chunk_1) + sizeof(smp_chunk_header_t) + chunk_1->size))

#define __POSTCHUNK_SIZE(chunk) \
    (__END_GLOBAL - (__U8_CAST(chunk) + sizeof(smp_chunk_header_t) + chunk->size))

#define __GLOBAL_PTR(local_ptr) \
    (smp + local_ptr)

#define __LOCAL_PTR(global_ptr) \
    (__U8_CAST(global_ptr) - smp)

static uint8_t smp[STATIC_MEM_POOL_SIZE];

#pragma pack(push, 1)
typedef struct{
    void *      mutex;
    uint16_t    size;
    uint16_t    prev_chunk;
    uint16_t    next_chunk;
}smp_chunk_header_t;
#pragma pack(pop)

static inline
void * smp_chunk_add(smp_chunk_header_t * prev_chunk, smp_chunk_header_t * next_chunk,
                     uint8_t * global_pos, uint16_t size){
    smp_chunk_header_t * new_chunk = __CHUNK_CAST(global_pos);
    if ((prev_chunk != __CHUNK_CAST(__BEGIN_GLOBAL)) || ((prev_chunk == __CHUNK_CAST(__BEGIN_GLOBAL)) && (prev_chunk->size != 0))){
        printf("new next chunk set\r\n");
        prev_chunk->next_chunk = __LOCAL_PTR(new_chunk);
    }
    if (next_chunk != __CHUNK_CAST(__END_GLOBAL)){
        next_chunk->prev_chunk = __LOCAL_PTR(new_chunk);
    }
    new_chunk->next_chunk = __LOCAL_PTR(next_chunk);
    new_chunk->prev_chunk = __LOCAL_PTR(prev_chunk);
    new_chunk->size = size;
    new_chunk->mutex = __NULL;
    return __VOID_CAST(__U8_CAST(new_chunk) + sizeof(smp_chunk_header_t));
}

void smp_initialize(void){
    smp_chunk_header_t * initial_chunk = __CHUNK_CAST(smp);
    initial_chunk->mutex = __NULL;
    initial_chunk->next_chunk = __END_LOCAL;
    initial_chunk->prev_chunk = __BEGIN_LOCAL;
    initial_chunk->size = 0;
}

void * smp_malloc(uint16_t size){
    smp_chunk_header_t * active_chunk = __CHUNK_CAST(smp);
    if (active_chunk->size == 0){
        printf("First allocation\r\n");
        return smp_chunk_add(__CHUNK_CAST(__BEGIN_GLOBAL), __CHUNK_CAST(__END_GLOBAL), smp, size);
    }
    smp_chunk_header_t * next_chunk;
    while(active_chunk->next_chunk != __END_LOCAL){
        printf("Midchunk allocation\r\n");
        next_chunk = (smp_chunk_header_t *)((uint8_t *)active_chunk + active_chunk->next_chunk);
        if ((__MIDCHUNK_SIZE(active_chunk, next_chunk) - sizeof(smp_chunk_header_t) >= size)){
            return smp_chunk_add(active_chunk, next_chunk, __U8_CAST(active_chunk) + sizeof(smp_chunk_header_t) + active_chunk->size, size);
        }
        active_chunk = __CHUNK_CAST(__GLOBAL_PTR(active_chunk->next_chunk));
    }
    if ((__POSTCHUNK_SIZE(active_chunk) - sizeof(smp_chunk_header_t)) >= size){
        printf("Postchunk allocation\r\n");
        return smp_chunk_add(active_chunk, __CHUNK_CAST(__END_GLOBAL), __U8_CAST(active_chunk) + sizeof(smp_chunk_header_t) + active_chunk->size, size);
    }
    printf("No allocation\r\n");
    return __NULL;
}

uint32_t get_local(void * ptr){
    return __LOCAL_PTR(ptr);
}

void print_map(void){
    for (uint16_t iter = 0; iter < STATIC_MEM_POOL_SIZE; iter++){
        printf("0x%X ", smp[iter]);
    }
    printf("\r\n");
}