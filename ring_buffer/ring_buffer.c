#include "ring_buffer.h"
#include <stdint.h>
#include <string.h>

#define __weak __attribute__((weak))

/**
 * @brief Ring buffer mutex create
 * @details Function, implemented by user for mutex allocation
 * @warning WEAK
 * @return Abstract (void *) to the created mutex
 */
__weak void * rb_mutex_create(void){
    return NULL;
}
/**
 * @brief Ring buffer mutex acquire
 * @details Function, implemented by user for mutex acquiring
 * @warning WEAK
 * @param[in] mutex Abstract (void *) to the mutex, created with rb_mutex_create
 * @param[in] timeout Optional timeout value for mutex acquizition await
 * @return Status of acquizition
*/
__weak rb_mutex_status_e rb_mutex_acquire(void * mutex, uint32_t timeout){
    return RB_MUTEX_ERROR;
}
/**
 * @brief Ring buffer mutex release
 * @details Function, implemented by user for mutex release
 * @warning WEAK
 * @param[in] mutex Abstract (void *) to the mutex, created with rb_mutex_create
 * @return Status of release
*/
__weak rb_mutex_status_e rb_mutex_release(void * mutex){
    return RB_MUTEX_ERROR;
}
/**
 * @brief Ring buffer mutex availability status
 * @details Finction, implemented by user for mutex availability check
 * @warning WEAK
 * @param[in] mutex Abstract (void *) to the mutex, created with rb_mutex_create
 * @return Status of the mutex
*/
__weak rb_mutex_status_e rb_mutex_is_free(void * mutex){
    return RB_MUTEX_ERROR;
}

/**
 * @brief Ring buffer initialization function
 * @warning Do not reinitialize ring buffers with this function
 * @details Fill in the fields of the ring buffer object
 * @param[in] rb Ring buffer pointer to already existing, not initialized ring buffer object
 * @param[in] flags Option flags of the ring buffer
*/
void        rb_init(ring_buffer_t * rb, rb_flags_t flags){
    rb->flags = (flags & (RB_FLAG_USE_MUTEX |
                          RB_FLAG_USE_BUFFER_MASK |
                          RB_FLAG_USE_OVERWRITE |
                          RB_FLAG_USE_WRITE_LEAST_AVAILABLE));
    if (rb->flags & RB_FLAG_USE_MUTEX){
        rb->mutex = rb_mutex_create();
    }
    else{
        rb->mutex = NULL;
    }
    rb->back = 0;
    rb->front = 0;
    rb->bytes_available = 0;
    if (rb->flags & RB_FLAG_USE_BUFFER_MASK){
        memset(rb->buffer, RING_BUFFER_MASK_VALUE, RING_BUFFER_SIZE);
    }
}

/**
 * @brief Ring buffer check for availability
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Checks if ring buffer mutex is acquired. If the ring buffer was not initialized with RB_FLAG_USE_MUTEX, returns RB_STATUS_OK
 * @param[in] rb Ring buffer pointer to already existing and initialized ring buffer object
 * @return Ring buffer availability status
*/
rb_status_e rb_is_available(ring_buffer_t * rb){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX))){
        return RB_STATUS_OK;
    }
    else{
        if (rb_mutex_is_free(rb->mutex) == RB_MUTEX_OK){
            return RB_STATUS_OK;
        }
        else{
            return RB_STATUS_BUSY;
        }
    }
}

/**
 * @brief Push data to the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB 
 * @details Pushes data to the ring buffer, protected with mutex
 * @param[in] rb Ring buffer handle
 * @param[in] source Data to push
 * @param[in] size Amount of data in bytes
 * @param[in] timeout Semaphore acquisition timeout
 * @return Data push status
 */
rb_status_e rb_mutex_push(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t timeout){
    if (rb->mutex == NULL)
        return RB_STATUS_WRONG_TYPE;
    if ((!size) || (size > RING_BUFFER_SIZE))
        return RB_STATUS_OUT_OF_RANGE;
    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;
    if ((size > (RING_BUFFER_SIZE - rb->bytes_available)) &&
        (!(rb->flags & RB_FLAG_USE_WRITE_LEAST_AVAILABLE)) &&
        (!(rb->flags & RB_FLAG_USE_OVERWRITE))){
            rb_mutex_release(rb->mutex);
            return RB_STATUS_NOT_ENOUGH_SPACE;
    }

    if (size == 1){
        rb->buffer[rb->back] = source[0];
        rb->back = (((rb->back + 1) == RING_BUFFER_SIZE) ? (0) : (rb->back + 1));
        if (rb->bytes_available < RING_BUFFER_SIZE) rb->bytes_available++;
        else rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
    }
    else if ((RING_BUFFER_SIZE - rb->back) < size){
        uint32_t size_1 = RING_BUFFER_SIZE - rb->back;
        uint32_t size_2 = size - size_1;
        memcpy(rb->buffer + rb->back, source, size_1);
        memcpy(rb->buffer, source + size_1, size_2);
        rb->back = size_2;
        if (rb->front < size_2) rb->front = size_2;
        if ((rb->bytes_available + size) > RING_BUFFER_SIZE){
            rb->bytes_available = RING_BUFFER_SIZE;
        }
        else{
            rb->bytes_available += size;
        }
    }
    else{
        memcpy(rb->buffer + rb->back, source, size);
        if (rb->bytes_available + size > RING_BUFFER_SIZE){
            rb->back = (((rb->back + size) >= RING_BUFFER_SIZE) ? (rb->back + size - RING_BUFFER_SIZE) : (rb->back + size));
            rb->front = rb->back;
            rb->bytes_available = RING_BUFFER_SIZE;
        }
        else{
            rb->back = (((rb->back + size) >= RING_BUFFER_SIZE) ? (rb->back + size - RING_BUFFER_SIZE) : (rb->back + size));
            rb->bytes_available += size;
        }
    }

    rb_mutex_release(rb->mutex);
    return RB_STATUS_OK;
}

/**
 * @brief Push data to the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Pushed data to the ring buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @param[in] source Data to push
 * @param[in] size Amount of data in bytes
*/
rb_status_e rb_push(ring_buffer_t * rb, uint8_t * source, uint32_t size){
    if ((!size) || (size > RING_BUFFER_SIZE))
        return RB_STATUS_OUT_OF_RANGE;
    if ((size > (RING_BUFFER_SIZE - rb->bytes_available)) &&
        (!(rb->flags & RB_FLAG_USE_WRITE_LEAST_AVAILABLE)) &&
        (!(rb->flags & RB_FLAG_USE_OVERWRITE)))
            return RB_STATUS_NOT_ENOUGH_SPACE;

    if (size == 1){
        rb->buffer[rb->back] = source[0];
        rb->back = (((rb->back + 1) == RING_BUFFER_SIZE) ? (0) : (rb->back + 1));
        if (rb->bytes_available < RING_BUFFER_SIZE) rb->bytes_available++;
        else rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
    }
    else if ((RING_BUFFER_SIZE - rb->back) < size){
        uint32_t size_1 = RING_BUFFER_SIZE - rb->back;
        uint32_t size_2 = size - size_1;
        memcpy(rb->buffer + rb->back, source, size_1);
        memcpy(rb->buffer, source + size_1, size_2);
        rb->back = size_2;
        if (rb->front < size_2) rb->front = size_2;
        if ((rb->bytes_available + size) > RING_BUFFER_SIZE){
            rb->bytes_available = RING_BUFFER_SIZE;
        }
        else{
            rb->bytes_available += size;
        }
    }
    else{
        memcpy(rb->buffer + rb->back, source, size);
        if (rb->bytes_available + size > RING_BUFFER_SIZE){
            rb->back = (((rb->back + size) >= RING_BUFFER_SIZE) ? (rb->back + size - RING_BUFFER_SIZE) : (rb->back + size));
            rb->front = rb->back;
            rb->bytes_available = RING_BUFFER_SIZE;
        }
        else{
            rb->back = (((rb->back + size) >= RING_BUFFER_SIZE) ? (rb->back + size - RING_BUFFER_SIZE) : (rb->back + size));
            rb->bytes_available += size;
        }
    }

    return RB_STATUS_OK;
}

// rb_status_e rb_push_data(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t timeout){
//     if ((!size) || (size > RING_BUFFER_SIZE))
//         return RING_BUFFER_OUT_OF_RANGE;

// #ifdef RING_BUFFER_THREAD_SAFETY
//     if (rb_semaphore_acquire(rb, timeout) != RB_MUTEX_OK) return RING_BUFFER_BUSY;
// #endif
//     if (size == 1){
//         rb->buffer[rb->back] = source[0];
//         rb->back = (((rb->back + 1) == RING_BUFFER_SIZE) ? (0) : (rb->back + 1));
//         if (rb->bytes_available < RING_BUFFER_SIZE) rb->bytes_available++;
//         else rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
//     }
//     else if ((RING_BUFFER_SIZE - rb->back) < size){
//         uint32_t size_1 = RING_BUFFER_SIZE - rb->back;
//         uint32_t size_2 = size - size_1;
//         memcpy(rb->buffer + rb->back, source, size_1);
//         memcpy(rb->buffer, source + size_1, size_2);
//         rb->back = size_2;
//         if (rb->front < size_2) rb->front = size_2;
//         if ((rb->bytes_available + size) > RING_BUFFER_SIZE){
//             rb->bytes_available = RING_BUFFER_SIZE;
//         }
//         else{
//             rb->bytes_available += size;
//         }
//     }
//     else{
//         memcpy(rb->buffer + rb->back, source, size);
//         if (rb->bytes_available + size > RING_BUFFER_SIZE){
//             rb->back = (((rb->back + size) >= RING_BUFFER_SIZE) ? (rb->back + size - RING_BUFFER_SIZE) : (rb->back + size));
//             rb->front = rb->back;
//             rb->bytes_available = RING_BUFFER_SIZE;
//         }
//         else{
//             rb->back = (((rb->back + size) >= RING_BUFFER_SIZE) ? (rb->back + size - RING_BUFFER_SIZE) : (rb->back + size));
//             rb->bytes_available += size;
//         }
//     }
// #ifdef RING_BUFFER_THREAD_SAFETY
//     rb_semaphore_release(rb);
// #endif
//     return RING_BUFFER_OK;
// }

// #ifdef RING_BUFFER_THREAD_SAFETY
// rb_status_e rb_take_data(ring_buffer_t * rb, uint8_t * dest, uint32_t size, uint32_t timeout){
// #else
// rb_status_e rb_take_data(ring_buffer_t * rb, uint8_t * dest, uint32_t size){
// #endif

//     if ((!size) || (size > RING_BUFFER_SIZE) || (size > rb->bytes_available))
//         return RING_BUFFER_OUT_OF_RANGE;

// #ifdef RING_BUFFER_THREAD_SAFETY
//     if (rb_semaphore_acquire(rb, timeout) == RB_MUTEX_OK){
// #endif
//         if (size == 1){
//             dest[0] = rb->buffer[rb->front];
//             rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
//             rb->bytes_available--;
//         }
//         else if ((RING_BUFFER_SIZE - rb->front) < size){
//             uint32_t size_1 = RING_BUFFER_SIZE - rb->front;
//             uint32_t size_2 = size - size_1;
//             memcpy(dest, rb->buffer + rb->front, size_1);
//             memcpy(dest + size_1, rb->buffer, size_2);
//             rb->front = size_2;
//             rb->bytes_available -= size;
//         }
//         else{
//             memcpy(dest, rb->buffer + rb->front, size);
//             rb->front = (rb->front + size) % RING_BUFFER_SIZE;
//             rb->bytes_available -= size;
//         }
// #ifdef RING_BUFFER_THREAD_SAFETY
//         rb_semaphore_release(rb);
// #endif
//         return RING_BUFFER_OK;
// #ifdef RING_BUFFER_THREAD_SAFETY
//     }
    
//     return RING_BUFFER_BUSY;
// #endif
// }

// rb_status_e rb_popdata(ring_buffer_t * rb
// #ifdef RING_BUFFER_THREAD_SAFETY
//                         , uint32_t timeout){
// #else
//                         ){
// #endif

// #ifdef RING_BUFFER_THREAD_SAFETY
//     if (rb_semaphore_acquire(rb, timeout) == RB_MUTEX_OK){
// #endif
//         if (rb->bytes_available){
//             rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
//             rb->bytes_available--;
//         }
// #ifdef RING_BUFFER_THREAD_SAFETY
//         rb_semaphore_release(rb);
// #endif
//         return RING_BUFFER_OK;
// #ifdef RING_BUFFER_THREAD_SAFETY
//     }
//     return RING_BUFFER_BUSY;
// #endif
// }

// void        rb_flush(ring_buffer_t * rb
// #ifdef RING_BUFFER_THREAD_SAFETY
//                     , uint32_t timeout){
// #else
//                     ){
// #endif

// #ifdef RING_BUFFER_THREAD_SAFETY
//     if (rb_semaphore_acquire(rb, timeout) == RB_MUTEX_OK){
// #endif
// #ifdef RING_BUFFER_USE_MASK
//         memset(rb->buffer, RING_BUFFER_MASK_VALUE, RING_BUFFER_SIZE);
// #endif
//         rb->back = 0;
//         rb->front = 0;
//         rb->bytes_available = 0;

// #ifdef RING_BUFFER_THREAD_SAFETY
//         rb_semaphore_release(rb);
// #endif
//         return RING_BUFFER_OK;
// #ifdef RING_BUFFER_THREAD_SAFETY
//     }
//     return RING_BUFFER_BUSY;
// #endif
// }

// uint32_t rb_get_available(ring_buffer_t * rb
// #ifdef RING_BUFFER_THREAD_SAFETY
//                             , uint32_t timeout){
// #else
//                             ){
// #endif

// #ifdef RING_BUFFER_THREAD_SAFETY
//     if (rb_semaphore_acquire(rb, timeout) == RB_MUTEX_OK){
// #endif
//         uint32_t tmp = rb->bytes_available;
// #ifdef RING_BUFFER_THREAD_SAFETY
//         rb_semaphore_release(rb);
// #endif
//         return tmp;
// #ifdef RING_BUFFER_THREAD_SAFETY
//     }
//     return 0;
// #endif
// }