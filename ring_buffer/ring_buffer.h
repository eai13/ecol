#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdint.h>

#define RING_BUFFER_SIZE        500
#define RING_BUFFER_MASK_VALUE  0xA5

typedef enum{
    RB_STATUS_OK                = 0x00,
    RB_STATUS_OUT_OF_RANGE      = 0x01,
    RB_STATUS_NOT_INIT          = 0x02,
    RB_STATUS_BUSY              = 0x03,
    RB_STATUS_WRONG_TYPE        = 0x04,
    RB_STATUS_NOT_ENOUGH_SPACE  = 0x05
}rb_status_e;

typedef enum{
    RB_FLAG_USE_MUTEX                   = 0x01,
    RB_FLAG_USE_BUFFER_MASK             = 0x02,
    RB_FLAG_USE_OVERWRITE               = 0x04,
    RB_FLAG_USE_WRITE_LEAST_AVAILABLE   = 0x08
}rb_flags_t;

typedef enum{
    RB_MUTEX_OK     = 0x00,
    RB_MUTEX_ERROR  = 0x01
}rb_mutex_status_e;

typedef struct{
    rb_flags_t  flags;
    uint32_t    front;
    uint32_t    back;
    uint32_t    bytes_available;
    uint8_t     buffer[RING_BUFFER_SIZE];
    void *      mutex;
}ring_buffer_t;

void *              rb_mutex_create(void);
rb_mutex_status_e   rb_mutex_acquire(void * mutex, uint32_t timeout);
rb_mutex_status_e   rb_mutex_release(void * mutex);
rb_mutex_status_e   rb_mutex_is_free(void * mutex);

void        rb_init(ring_buffer_t * rb, rb_flags_t flags);
rb_status_e rb_is_available(ring_buffer_t * rb);
rb_status_e rb_mutex_push(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t timeout);
rb_status_e rb_push(ring_buffer_t * rb, uint8_t * source, uint32_t size);
// rb_status_e rb_push_data(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t timeout);
// rb_status_e rb_take_data(ring_buffer_t * rb, uint8_t * dest, uint32_t size, uint32_t timeout);
// void        rb_flush(ring_buffer_t * rb, uint32_t timeout);
// rb_status_e rb_popdata(ring_buffer_t * rb, uint32_t timeout);
// uint32_t    rb_get_available(ring_buffer_t * rb, uint32_t timeout);
// rb_status_e rb_is_available(ring_buffer_t * rb);
// #else
// void        rb_init(ring_buffer_t * rb);
// rb_status_e rb_push_data(ring_buffer_t * rb, uint8_t * source, uint32_t size);
// rb_status_e rb_take_data(ring_buffer_t * rb, uint8_t * dest, uint32_t size);
// void        rb_flush(ring_buffer_t * rb);
// rb_status_e rb_popdata(ring_buffer_t * rb);
// uint32_t    rb_get_available(ring_buffer_t * rb);
// rb_status_e rb_is_available(ring_buffer_t * rb);
// #endif

// #ifdef RING_BUFFER_THREAD_SAFETY
// rb_mutex_status_e rb_semaphore_acquire(ring_buffer_t * rb, uint32_t timeout);
// #else
// rb_mutex_status_e rb_semaphore_acquire(ring_buffer_t * rb);
// #endif
// rb_mutex_status_e rb_semaphore_release(ring_buffer_t * rb);
// rb_mutex_status_e rb_semaphore_is_free(ring_buffer_t * rb);

// void              rb_semaphore_released_callback(ring_buffer_t * rb);

#endif