#include "static_mem_pool.h"

/// @brief A null void pointer MACRO
#define SMP_NULL          ((void *)0)

/// @brief Local address of the start of SMP
#define SMP_BEGIN_LOCAL   ((smp_addr_t)0u)
/// @brief Global address of the start of SMP
#define SMP_BEGIN_GLOBAL  (smp + SMP_BEGIN_LOCAL)
/// @brief Local address of the end of SMP
#define SMP_END_LOCAL     ((smp_addr_t)STATIC_MEM_POOL_SIZE)
/// @brief Global address of the end of SMP
#define SMP_END_GLOBAL    (smp + SMP_END_LOCAL)
/// @brief Chunk header size in bytes
#define SMP_CHUNK_SIZE    ((smp_size_t)sizeof(smp_chunk_header_t))

/// @brief Cast to uint8_t *
#define SMP_CAST_U8(ptr) \
    ((uint8_t *)(ptr))
/// @brief Cast to smp_chunk_header_t *
#define SMP_CAST_CHUNK_H(ptr) \
    ((smp_chunk_header_t *)(ptr))
/// @brief Cast to void *
#define SMP_CAST_VOID(ptr) \
    ((void *)(ptr))

/// @brief MACRO for free space between two chunks
#define SMP_MIDCHUNK_SIZE(chunk_1, chunk_2) \
    (SMP_CAST_U8(chunk_2) - (SMP_CAST_U8(chunk_1) + SMP_CHUNK_SIZE + chunk_1->size))
/// @brief MACRO for free space after the last chunk in series
#define SMP_POSTCHUNK_SIZE(chunk) \
    (SMP_END_GLOBAL - (SMP_CAST_U8(chunk) + SMP_CHUNK_SIZE + chunk->size))

/// @brief Converts local pointer to the global one
#define SMP_GLOBAL_PTR(local_ptr) \
    (smp + local_ptr)
/// @brief Converts global pointer to the local one
#define SMP_LOCAL_PTR(global_ptr) \
    (SMP_CAST_U8(global_ptr) - smp)

/// @brief Static memory pool
static uint8_t smp[STATIC_MEM_POOL_SIZE];

#pragma pack(push, 1)
/// @brief Chunk header typedef
typedef struct{
    smp_size_t    size;       ///< Size of data, contained in the chunk
    smp_addr_t    prev_chunk; ///< Local address of the previous chunk
    smp_addr_t    next_chunk; ///< Local address of the next chunk
}smp_chunk_header_t;
#pragma pack(pop)

/***
 * @brief Utility local function, used for new chunk creation between two chunks
 * @param[in] prev_chunk Global pointer to the previous chunk of a new allocated chunk
 * @param[in] next_chunk Global pointer to the next chunk of a new allocated chunk
 * @param[in] global_pos Global pointer to the position of a new chunk in static pool
 * @param[in] size User-memory allocation size in the chunk (Chunk header size is not considered here)
 * @return Returns a void pointer to the start of the allocated memory
*/
static inline
void * smp_chunk_add(smp_chunk_header_t * prev_chunk, smp_chunk_header_t * next_chunk,
                     uint8_t * global_pos, smp_size_t size){
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

/***
 * @brief Static function for finding the chunk via the pointer
 * @param[in] ptr Some pointer
*/
static inline
smp_chunk_header_t * smp_findchunk(void * ptr){
    if ((SMP_END_GLOBAL <= SMP_CAST_U8(ptr)) || (SMP_BEGIN_GLOBAL > SMP_CAST_U8(ptr))){
        return SMP_CAST_CHUNK_H(SMP_NULL);
    }
    smp_chunk_header_t * active_chunk = SMP_CAST_CHUNK_H(smp);
    do{
        if ((SMP_CAST_U8(ptr) < (SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size)) &&
            (SMP_CAST_U8(ptr) >= (SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE)))
        {
            return active_chunk;
        }
        active_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
    }while(active_chunk != SMP_CAST_CHUNK_H(SMP_END_GLOBAL));
    return SMP_CAST_CHUNK_H(SMP_NULL);
}

/***
 * @brief Utility local function, used to count filled bytes (including headers) in static memory pool
 * @return Returns a number of filled bytes in the memory pool
*/
static inline
smp_size_t smp_count_filled_bytes(void){
    smp_size_t filled_bytes = (smp_size_t)0;
    smp_chunk_header_t * active_chunk = SMP_CAST_CHUNK_H(smp);
    do{
        if(active_chunk->size != (smp_size_t)0){
            filled_bytes += SMP_CHUNK_SIZE + active_chunk->size;
        }
        active_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
    } while(active_chunk != SMP_CAST_CHUNK_H(SMP_END_GLOBAL));
    return filled_bytes;
}

/***
 * @brief Static memory pool initialization function, call it before start working with static allocation
*/
void smp_initialize(void){
    smp_chunk_header_t * initial_chunk = SMP_CAST_CHUNK_H(smp);
    initial_chunk->next_chunk = SMP_END_LOCAL;
    initial_chunk->prev_chunk = SMP_BEGIN_LOCAL;
    initial_chunk->size = (smp_size_t)0;
}

/***
 * @brief Static memory pool memory chunk allocation
 * @param[in] size Amount of bytes to be allocated
 * @return Returns a void pointer to the start of the allocated memory
*/
void * smp_malloc(smp_size_t size){
    if(size == (smp_size_t)0) {
        return SMP_NULL;
    }
    smp_chunk_header_t * active_chunk = SMP_CAST_CHUNK_H(smp);
    if ((active_chunk->size == (smp_size_t)0) &&
        ((SMP_MIDCHUNK_SIZE(active_chunk, SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk)))) >= size)){
        return smp_chunk_add(SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->prev_chunk)), SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk)), smp, size);
    }
    smp_chunk_header_t * next_chunk;
    while(active_chunk->next_chunk != SMP_END_LOCAL){
        next_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
        if (SMP_MIDCHUNK_SIZE(active_chunk, next_chunk) >= (size + SMP_CHUNK_SIZE)){
            return smp_chunk_add(active_chunk, next_chunk, SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size, size);
        }
        active_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
    }
    if ((SMP_POSTCHUNK_SIZE(active_chunk) - SMP_CHUNK_SIZE) >= size){
        return smp_chunk_add(active_chunk, SMP_CAST_CHUNK_H(SMP_END_GLOBAL), SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE + active_chunk->size, size);
    }
    return SMP_NULL;
}

/***
 * @brief Static memory chunk free
 * @details Frees the allocated chunk of static pool, the incoming pointer may point not only to the start of the memory chunk,
 *          but to any of the allocated space, it will clear the whole chunk anyway
 * @param[in] ptr Pointer to the piece of data in the chunk to be freed
 * @return Returns the free status
*/
smp_free_status_e smp_free(void * ptr){
    smp_chunk_header_t * active_chunk = smp_findchunk(ptr);
    if (active_chunk == SMP_NULL) return SMP_FREE_NOT_IN_POOL;

    if (active_chunk == SMP_CAST_CHUNK_H(SMP_BEGIN_GLOBAL)){
        active_chunk->size = (smp_size_t)0;
        return SMP_FREE_OK;
    }
    else{
        smp_chunk_header_t * prev_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->prev_chunk));
        prev_chunk->next_chunk = active_chunk->next_chunk;
        if (active_chunk->next_chunk != SMP_END_LOCAL) {
            smp_chunk_header_t * next_chunk = SMP_CAST_CHUNK_H(SMP_GLOBAL_PTR(active_chunk->next_chunk));
            next_chunk->prev_chunk = active_chunk->prev_chunk;
        }
        active_chunk->size = (smp_size_t)0;
        active_chunk->next_chunk = SMP_BEGIN_LOCAL;
        active_chunk->prev_chunk = SMP_BEGIN_LOCAL;
        return SMP_FREE_OK;
    }
}

/***
 * @brief Static memory realloc
 * @details Resizes the allocated memory chunk. If the new_size is 0 or the ptr is not valid, returns SMP_NULL
 *          and does not free the ptr chunk. Otherwise resizes the ptr chunk if possible or frees the old one and
 *          mallocs the new one and copies the data from the old address to the new one
 * @param[in] ptr Pointer to the piece of data in the chunk to be reallocated
 * @return Returns SMP_NULL if failed and void * if succeeded
*/
void * smp_realloc(void * ptr, smp_size_t new_size){
    if (new_size == (smp_size_t)0) return SMP_NULL;
    smp_chunk_header_t * active_chunk = smp_findchunk(ptr);
    if (!active_chunk) return SMP_NULL;
    
    if ((SMP_GLOBAL_PTR(active_chunk->next_chunk) - SMP_CAST_U8(active_chunk) - SMP_CHUNK_SIZE) >= new_size){
        active_chunk->size = new_size;
        return ptr;
    }
    else{
        void * new_ptr = smp_malloc(new_size);
        if (new_ptr == SMP_NULL){
            return SMP_NULL;
        }
        memcpy(new_ptr, SMP_CAST_U8(active_chunk) + SMP_CHUNK_SIZE, active_chunk->size);
        smp_free(ptr);
        return new_ptr;
    }
}

/***
 * @brief Count filled bytes (including headers) in static memory pool
 * @return Returns a number of filled bytes in the memory pool
*/
smp_size_t smp_count_filled(void){
    return smp_count_filled_bytes();
}

/***
 * @brief Count free bytes (including headers) in static memory pool
 * @return Returns a number of free bytes in the memory pool
*/
smp_size_t smp_count_free(void){
    return ((smp_size_t)STATIC_MEM_POOL_SIZE - smp_count_filled_bytes());
}

/***
 * @brief Check if the ptr chunk has the needed amount of space
 * @param[in] ptr Some address in the memory
 * @param[in] size Required amount of space to be verified
 * @return SMP_ASSERT_TRUE - if the ptr belongs to some chunk and size parameter is less or equal to the chunk->size, SMP_ASSERT_FALSE - otherwise 
*/
smp_assert_e smp_assert_size(void * ptr, smp_size_t size){
    if ((size == (smp_size_t)0) || (ptr == SMP_NULL)) return SMP_ASSERT_FALSE;
    smp_chunk_header_t * active_chunk = smp_findchunk(ptr);
    if (active_chunk != SMP_NULL){
        if (active_chunk->size >= size){
            return SMP_ASSERT_TRUE;
        }
    }
    return SMP_ASSERT_FALSE;
}

/***
 * @brief Check if the ptr belongs to some chunk
 * @param[in] ptr Some address in the memory
 * @return SMP_ASSERT_TRUE - if the ptr belongs to some chunk, SMP_ASSERT_FALSE - otherwise
*/
smp_assert_e smp_assert_address(void * ptr){
    if (ptr == SMP_NULL) return SMP_ASSERT_FALSE;
    smp_chunk_header_t * active_chunk = smp_findchunk(ptr);
    if (active_chunk != SMP_NULL){
        return SMP_ASSERT_TRUE;
    }
    return SMP_ASSERT_FALSE;
}


// Utility functions, may delete
/**/ uint32_t get_local(void * ptr){
/**/     return SMP_LOCAL_PTR(ptr);
/**/ }
/**/ 
/**/ void print_map(void){
/**/     for (uint16_t iter = 0; iter < STATIC_MEM_POOL_SIZE; iter++){
/**/         printf("0x%X ", smp[iter]);
/**/     }
/**/     printf("\r\n");
/**/ }
//
