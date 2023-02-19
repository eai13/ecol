#include "ring_buffer.h"
#include <stdint.h>
#include <string.h>

#define __weak __attribute__((weak))


__weak void * rb_mutex_create(void){
    return NULL;
}

__weak rb_mutex_status_e rb_mutex_acquire(void * mutex, uint32_t timeout){
    return RB_MUTEX_ERROR;
}

__weak rb_mutex_status_e rb_mutex_release(void * mutex){
    return RB_MUTEX_ERROR;
}

__weak rb_mutex_status_e rb_mutex_is_free(void * mutex){
    return RB_MUTEX_ERROR;
}


void        rb_init(ring_buffer_t * rb, rb_flags_t flags){
    rb->flags = (flags & (RB_FLAG_USE_MUTEX |
                          RB_FLAG_USE_BUFFER_MASK |
                          RB_FLAG_USE_OVERWRITE |
                          RB_FLAG_USE_PUSH_LEAST_AVAILABLE |
                          RB_FLAG_USE_TAKE_LEAST_AVAILABLE));
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


rb_status_e rb_push(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t * r_data_written){
    if ((!size) || (size > RING_BUFFER_SIZE))
        return RB_STATUS_OUT_OF_RANGE;
    uint32_t verified_size;
    if (size > (RING_BUFFER_SIZE - rb->bytes_available)){
        if (rb->flags & RB_FLAG_USE_OVERWRITE){
            verified_size = size;
        }
        else{
            if (rb->flags & RB_FLAG_USE_WRITE_LEAST_AVAILABLE){
                verified_size = RING_BUFFER_SIZE - rb->bytes_available;
                if (!verified_size) return RB_STATUS_NOT_ENOUGH_SPACE;
            }
            else{
                return RB_STATUS_NOT_ENOUGH_SPACE;
            }
        }
    }

    if (r_data_written != NULL){
        (*r_data_written) = verified_size;
    }

    if (verified_size == 1){
        rb->buffer[rb->back] = source[0];
        rb->back = (((rb->back + 1) == RING_BUFFER_SIZE) ? (0) : (rb->back + 1));
        if (rb->bytes_available < RING_BUFFER_SIZE) rb->bytes_available++;
        else rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
    }
    else if ((RING_BUFFER_SIZE - rb->back) < verified_size){
        uint32_t size_1 = RING_BUFFER_SIZE - rb->back;
        uint32_t size_2 = verified_size - size_1;
        memcpy(rb->buffer + rb->back, source, size_1);
        memcpy(rb->buffer, source + size_1, size_2);
        rb->back = size_2;
        if (rb->front < size_2) rb->front = size_2;
        if ((rb->bytes_available + verified_size) > RING_BUFFER_SIZE){
            rb->bytes_available = RING_BUFFER_SIZE;
        }
        else{
            rb->bytes_available += verified_size;
        }
    }
    else{
        memcpy(rb->buffer + rb->back, source, verified_size);
        if (rb->bytes_available + verified_size > RING_BUFFER_SIZE){
            rb->back = (((rb->back + verified_size) >= RING_BUFFER_SIZE) ? (rb->back + verified_size - RING_BUFFER_SIZE) : (rb->back + verified_size));
            rb->front = rb->back;
            rb->bytes_available = RING_BUFFER_SIZE;
        }
        else{
            rb->back = (((rb->back + verified_size) >= RING_BUFFER_SIZE) ? (rb->back + verified_size - RING_BUFFER_SIZE) : (rb->back + verified_size));
            rb->bytes_available += verified_size;
        }
    }

    return RB_STATUS_OK;
}

rb_status_e rb_push_m(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t timeout, uint32_t * r_data_written){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX)))
        return RB_STATUS_WRONG_TYPE;
    
    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;

    rb_status_e status;
    status = rb_push(rb, source, size, r_data_written);

    rb_mutex_release(rb->mutex);
    return status;
}


rb_status_e rb_take(ring_buffer_t * rb, uint8_t * dest, uint32_t size, uint32_t * r_data_taken){
    if (!size)
        return RB_STATUS_OUT_OF_RANGE;

    uint32_t verified_size;
    if (size > rb->bytes_available){
        if (rb->flags & RB_FLAG_USE_TAKE_LEAST_AVAILABLE){
            verified_size = rb->bytes_available;
            if (!verified_size){
                return RB_STATUS_NOT_ENOUGH_ELEMENTS;
            }
        }
        else{
            return RB_STATUS_NOT_ENOUGH_SPACE;
        }
    }

    if (r_data_taken != NULL){
        (*r_data_taken) = verified_size;
    }

    if (verified_size == 1){
        dest[0] = rb->buffer[rb->front];
        if (rb->flags & RB_FLAG_USE_BUFFER_MASK){
            rb->buffer[rb->front] = RING_BUFFER_MASK_VALUE;
        }
        rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
        rb->bytes_available--;
    }
    else if ((RING_BUFFER_SIZE - rb->front) < verified_size){
        uint32_t size_1 = RING_BUFFER_SIZE - rb->front;
        uint32_t size_2 = verified_size - size_1;
        memcpy(dest, rb->buffer + rb->front, size_1);
        memcpy(dest + size_1, rb->buffer, size_2);
        if (rb->flags & RB_FLAG_USE_BUFFER_MASK){
            memset(rb->buffer + rb->front, RING_BUFFER_MASK_VALUE, size_1);
            memset(rb->buffer, RING_BUFFER_MASK_VALUE, size_2);
        }
        rb->front = size_2;
        rb->bytes_available -= verified_size;
    }
    else{
        memcpy(dest, rb->buffer + rb->front, verified_size);
        if (rb->flags & RB_FLAG_USE_BUFFER_MASK){
            memset(rb->buffer + rb->front, RING_BUFFER_MASK_VALUE, verified_size);
        }
        rb->front = (rb->front + verified_size) % RING_BUFFER_SIZE;
        rb->bytes_available -= verified_size;
    }

    return RB_STATUS_OK;
}

rb_status_e rb_take_m(ring_buffer_t * rb, uint8_t * dest, uint32_t size, uint32_t timeout, uint32_t * r_data_taken){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX)))
        return RB_STATUS_WRONG_TYPE;
    
    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;

    rb_status_e status;
    status = rb_take(rb, dest, size, r_data_taken);

    rb_mutex_release(rb->mutex);
    return status;
}


rb_status_e rb_flush(ring_buffer_t * rb){
    if (rb->flags & RB_FLAG_USE_BUFFER_MASK)
        memset(rb->buffer, RING_BUFFER_MASK_VALUE, RING_BUFFER_SIZE);
    
    rb->back = 0;
    rb->front = 0;
    rb->bytes_available = 0;

    return RB_STATUS_OK;
}

rb_status_e rb_flush_m(ring_buffer_t * rb, uint32_t timeout){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX)))
        return RB_STATUS_WRONG_TYPE;
    
    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;

    rb_status_e status;
    status = rb_flush(rb);

    rb_mutex_release(rb->mutex);

    return status;
}


rb_status_e rb_data_available(ring_buffer_t * rb, uint32_t * r_bytes_available){
    (*r_bytes_available) = rb->bytes_available;
    return RB_STATUS_OK;
}

rb_status_e rb_data_available_m(ring_buffer_t * rb, uint32_t * r_bytes_available, uint32_t timeout){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX)))
        return RB_STATUS_WRONG_TYPE;

    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;

    rb_status_e status;
    status = rb_data_available(rb, r_bytes_available);

    rb_mutex_release(rb->mutex);

    return status;
}


rb_status_e rb_push_single(ring_buffer_t * rb, uint8_t data){
    if ((rb->bytes_available == RING_BUFFER_SIZE) && (!(rb->flags & RB_FLAG_USE_OVERWRITE))){
        return RB_STATUS_NOT_ENOUGH_SPACE;
    }

    rb->buffer[rb->back] = data;
    rb->back = (((rb->back + 1) == RING_BUFFER_SIZE) ? (0) : (rb->back));
    if (rb->bytes_available < RING_BUFFER_SIZE) rb->bytes_available++;
    else rb->front = (rb->front + 1) % RING_BUFFER_SIZE;

    return RB_STATUS_OK;
}

rb_status_e rb_push_single_m(ring_buffer_t * rb, uint8_t data, uint32_t timeout){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX)))
        return RB_STATUS_WRONG_TYPE;
    
    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;

    rb_status_e status;
    status = rb_push_single(rb, data);

    rb_mutex_release(rb->mutex);

    return status;
}

rb_status_e rb_take_single(ring_buffer_t * rb, uint8_t * r_data){
    if (!(rb->bytes_available))
        return RB_STATUS_NOT_ENOUGH_ELEMENTS;

    (*data) = rb->buffer[rb->front];
    if (rb->flags & RB_FLAG_USE_BUFFER_MASK){
        rb->buffer[rb->front] = RING_BUFFER_MASK_VALUE;
    }
    rb->front = (rb->front + 1) % RING_BUFFER_SIZE;
    rb->bytes_available--;

    return RB_STATUS_OK;
}

rb_status_e rb_take_single_m(ring_buffer_t * rb, uint8_t * r_data, uint32_t timeout){
    if ((rb->mutex == NULL) || (!(rb->flags & RB_FLAG_USE_MUTEX)))
        return RB_STATUS_WRONG_TYPE;
    
    if (rb_mutex_acquire(rb->mutex, timeout) == RB_MUTEX_ERROR)
        return RB_STATUS_BUSY;

    rb_status_e status;
    status = rb_take_single(rb, data);

    rb_mutex_release(rb->mutex);

    return status;
}