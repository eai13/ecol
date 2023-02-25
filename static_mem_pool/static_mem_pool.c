#include "static_mem_pool.h"

#define SMP_NULL          ((void *)0)

#define SMP_BEGIN_LOCAL   (0)
#define SMP_BEGIN_GLOBAL  (smp + SMP_BEGIN_LOCAL)
#define SMP_END_LOCAL     (STATIC_MEM_POOL_SIZE)
#define SMP_END_GLOBAL    (smp + SMP_END_LOCAL)
#define SMP_CHUNK_SIZE    (sizeof(smp_chunk_header_t))

#define SMP_CAST_U8(ptr) \
    ((uint8_t *)(ptr))
#define SMP_CAST_CHUNK_H(ptr) \
    ((smp_chunk_header_t *)(ptr))
#define SMP_CAST_VOID(ptr) \
    ((void *)(ptr))

#define SMP_MIDCHUNK_SIZE(chunk_1, chunk_2) \
    (SMP_CAST_U8(chunk_2) - (SMP_CAST_U8(chunk_1) + SMP_CHUNK_SIZE + chunk_1->size))

#define SMP_POSTCHUNK_SIZE(chunk) \
    (SMP_END_GLOBAL - (SMP_CAST_U8(chunk) + SMP_CHUNK_SIZE + chunk->size))

#define SMP_GLOBAL_PTR(local_ptr) \
    (smp + local_ptr)

#define SMP_LOCAL_PTR(global_ptr) \
    (SMP_CAST_U8(global_ptr) - smp)

static uint8_t smp[STATIC_MEM_POOL_SIZE];

#pragma pack(push, 1)
typedef struct{
    uint16_t    size;
    uint16_t    prev_chunk;
    uint16_t    next_chunk;
}smp_chunk_header_t;
#pragma pack(pop)

static inline
void * smp_chunk_add(smp_chunk_header_t * prev_chunk, smp_chunk_header_t * next_chunk,
                     uint8_t * global_pos, uint16_t size){
    smp_chunk_header_t * new_chunk = SMP_CAST_CHUNK_H(global_pos);
    if ((prev_chunk != SMP_CAST_CHUNK_H(SMP_BEGIN_GLOBAL)) || ((prev_chunk == SMP_CAST_CHUNK_H(SMP_BEGIN_GLOBAL)) && (prev_chunk->size != 0))){
        prev_chunk->next_chunk = SMP_LOCAL_PTR(new_chunk);
    }
    if (next_chunk != SMP_CAST_CHUNK_H(SMP_END_GLOBAL)){
        next_chunk->prev_chunk = SMP_LOCAL_PTR(new_chunk);
    }
    new_chunk->next_chunk = SMP_LOCAL_PTR(next_chunk);
    new_chunk->prev_chunk = SMP_LOCAL_PTR(prev_chunk);
    new_chunk->size = size;
    return SMP_CAST_VOID(SMP_CAST_U8(new_chunk) + SMP_CHUNK_SIZE);
}

void smp_initialize(void){
    smp_chunk_header_t * initial_chunk = SMP_CAST_CHUNK_H(smp);
    initial_chunk->next_chunk = SMP_END_LOCAL;
    initial_chunk->prev_chunk = SMP_BEGIN_LOCAL;
    initial_chunk->size = 0;
}

void * smp_malloc(uint16_t size){
    smp_chunk_header_t * active_chunk = SMP_CAST_CHUNK_H(smp);
    if (active_chunk->size == 0){
        printf("First allocation\r\n");
        return smp_chunk_add(SMP_CAST_CHUNK_H(SMP_BEGIN_GLOBAL), SMP_CAST_CHUNK_H(SMP_END_GLOBAL), smp, size);
    }
    smp_chunk_header_t * next_chunk;
    while(active_chunk->next_chunk != SMP_END_LOCAL){
        next_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
        if (SMP_MIDCHUNK_SIZE(active_chunk, next_chunk) >= (size + SMP_CHUNK_SIZE)){
            printf("Midchunk size %d\r\n", SMP_MIDCHUNK_SIZE(active_chunk, next_chunk));
            printf("Midchunk allocation\r\n");
            return smp_chunk_add(active_chunk, next_chunk, SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size, size);
        }
        active_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
    }
    if ((SMP_POSTCHUNK_SIZE(active_chunk) - SMP_CHUNK_SIZE) >= size){
        printf("Postchunk allocation\r\n");
        return smp_chunk_add(active_chunk, SMP_CAST_CHUNK_H(SMP_END_GLOBAL), SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size, size);
    }
    printf("No allocation\r\n");
    return SMP_NULL;
}

smp_free_status_e smp_free(void * ptr){
    if ((SMP_END_GLOBAL <= SMP_CAST_U8(ptr)) || (SMP_BEGIN_GLOBAL > SMP_CAST_U8(ptr))){
        printf("Free not in pool\r\n");
        return SMP_FREE_NOT_IN_POOL;
    }
    smp_chunk_header_t * active_chunk = SMP_CAST_CHUNK_H(smp);
    smp_chunk_header_t * next_chunk;
    smp_chunk_header_t * prev_chunk;
    while(active_chunk->next_chunk != SMP_END_LOCAL){
        next_chunk = SMP_GLOBAL_PTR(active_chunk->next_chunk);
        if ((SMP_CAST_U8(ptr) >= SMP_CAST_U8(active_chunk)) && (SMP_CAST_U8(ptr) < SMP_CAST_U8(next_chunk))){
            if ((SMP_CAST_U8(ptr) < (SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE))){
                return SMP_FREE_IS_RESOURCE_PTR;
            }
            if ((SMP_CAST_U8(ptr) >= (SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size))){
                return SMP_FREE_ALREADY_FREED;
            }
            if (active_chunk == SMP_BEGIN_GLOBAL){
                memset(SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE, 0x00, active_chunk->size);
                active_chunk->size = 0;
                return SMP_FREE_OK;
            }
            else{
                prev_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->prev_chunk));
                prev_chunk->next_chunk = active_chunk->next_chunk;
                next_chunk->prev_chunk = active_chunk->prev_chunk;
                memset(SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE, 0x00, active_chunk->size);
                active_chunk->size = 0;
                active_chunk->next_chunk = 0;
                active_chunk->prev_chunk = 0;
                return SMP_FREE_OK;
            }
        }
        active_chunk = next_chunk;
    }
    if (SMP_CAST_U8(ptr) < (SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE)){
        return SMP_FREE_IS_RESOURCE_PTR;
    }
    if (SMP_CAST_U8(ptr) >= (SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size)){
        return SMP_FREE_ALREADY_FREED;
    }
    if (active_chunk == SMP_BEGIN_GLOBAL){
        memset(SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE, 0x00, active_chunk->size);
        active_chunk->size = 0;
        return SMP_FREE_OK;
    }
    else{
        prev_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->size));
        prev_chunk = active_chunk->next_chunk;
        memset(SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE, 0x00, active_chunk->size);
        active_chunk->size = 0;
        active_chunk->next_chunk = 0;
        active_chunk->prev_chunk = 0;
        return SMP_FREE_OK;
    }
}











uint32_t get_local(void * ptr){
    return SMP_LOCAL_PTR(ptr);
}

void print_map(void){
    for (uint16_t iter = 0; iter < STATIC_MEM_POOL_SIZE; iter++){
        printf("0x%X ", smp[iter]);
    }
    printf("\r\n");
}