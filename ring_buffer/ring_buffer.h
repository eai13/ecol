#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdint.h>

#define RING_BUFFER_SIZE        500
#define RING_BUFFER_MASK_VALUE  0xA5

/// @brief Available status of the functions
typedef enum{
    RB_STATUS_OK                    = 0x00, ///< OK status
    RB_STATUS_OUT_OF_RANGE          = 0x01, ///< Size of push or take is wrong (either > RING_BUFFER_SIZE, or == 0)
    RB_STATUS_NOT_INIT              = 0x02, ///< Ring buffer not initialized
    RB_STATUS_BUSY                  = 0x03, ///< Mutex of ring buffer is acquired and ring buffer is not currently available
    RB_STATUS_WRONG_TYPE            = 0x04, ///< Type flags do not allow to use the function with current ring buffer
    RB_STATUS_NOT_ENOUGH_SPACE      = 0x05, ///< Not enough space for pushing data into the ring buffer
    RB_STATUS_NOT_ENOUGH_ELEMENTS   = 0x06  ///< Not enough elements to take from the ring buffer
}rb_status_e;

/// @brief Available type flags for ring buffers. Flags are set only via rb_init
typedef enum{
    RB_FLAG_USE_MUTEX                   = 0x01, ///< Set this flag for mutex protection of ring buffer
    RB_FLAG_USE_BUFFER_MASK             = 0x02, ///< Set this flag to use buffer mask at free space of the ring buffer
    RB_FLAG_USE_OVERWRITE               = 0x04, ///< Set this flag, if it is required to overwrite existing data if the ring buffer is full
    RB_FLAG_USE_PUSH_LEAST_AVAILABLE    = 0x08, ///< Set this flag, if it is required to push the least available data in case when the RB_FLAG_USE_OVERWRITE was not set and it is OK to push only some part of data to the ring buffer
    RB_FLAG_USE_TAKE_LEAST_AVAILABLE    = 0x10  ///< Set this flag, if it is required to take the least available data (< required size) from the ring buffer
}rb_flags_t;

/// @brief Mutex acquisition/release states
typedef enum{
    RB_MUTEX_OK     = 0x00, ///< Mutex was acquired/released successfully
    RB_MUTEX_ERROR  = 0x01  ///< Mutex acquizition/release was corrupted
}rb_mutex_status_e;

typedef struct{
    rb_flags_t  flags;
    uint32_t    front;
    uint32_t    back;
    uint32_t    bytes_available;
    uint8_t     buffer[RING_BUFFER_SIZE];
    void *      mutex;
}ring_buffer_t;

/**
 * @brief Ring buffer mutex create
 * @details Function, implemented by user for mutex allocation
 * @warning WEAK
 * @return Abstract (void *) to the created mutex
 */
void *              rb_mutex_create(void);
/**
 * @brief Ring buffer mutex acquire
 * @details Function, implemented by user for mutex acquiring
 * @warning WEAK
 * @param[in] mutex Abstract (void *) to the mutex, created with rb_mutex_create
 * @param[in] timeout Optional timeout value for mutex acquizition await
 * @return Status of acquizition
*/
rb_mutex_status_e   rb_mutex_acquire(void * mutex, uint32_t timeout);
/**
 * @brief Ring buffer mutex release
 * @details Function, implemented by user for mutex release
 * @warning WEAK
 * @param[in] mutex Abstract (void *) to the mutex, created with rb_mutex_create
 * @return Status of release
*/
rb_mutex_status_e   rb_mutex_release(void * mutex);
/**
 * @brief Ring buffer mutex availability status
 * @details Finction, implemented by user for mutex availability check
 * @warning WEAK
 * @param[in] mutex Abstract (void *) to the mutex, created with rb_mutex_create
 * @return Status of the mutex
*/
rb_mutex_status_e   rb_mutex_is_free(void * mutex);

/**
 * @brief Ring buffer initialization function
 * @warning Do not reinitialize ring buffers with this function
 * @details Fill in the fields of the ring buffer object
 * @param[in] rb Ring buffer pointer to already existing, not initialized ring buffer object
 * @param[in] flags Option flags of the ring buffer
*/
void        rb_init(ring_buffer_t * rb, rb_flags_t flags);
/**
 * @brief Ring buffer check for availability
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Checks if ring buffer mutex is acquired. If the ring buffer was not initialized with RB_FLAG_USE_MUTEX, returns RB_STATUS_OK
 * @param[in] rb Ring buffer pointer to already existing and initialized ring buffer object
 * @return Ring buffer availability status
*/
rb_status_e rb_is_available(ring_buffer_t * rb);

/**
 * @brief Push data to the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Pushes data to the ring buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @param[in] source Data to push
 * @param[in] size Amount of data in bytes
 * @param[out] r_data_written Pointer to where to write real data amount pushed. May be set to NULL, if not needed
 * @return Data push status
*/
rb_status_e rb_push(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t * r_data_written);
/**
 * @brief Push data to the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB 
 * @details Pushes data to the ring buffer, protected with mutex
 * @param[in] rb Ring buffer handle
 * @param[in] source Data to push
 * @param[in] size Amount of data in bytes
 * @param[in] timeout Mutex acquisition timeout
 * @param[out] r_data_written Pointer to where to write real data amount pushed. May be set to NULL, if not needed
 * @return Data push status
 */
rb_status_e rb_push_m(ring_buffer_t * rb, uint8_t * source, uint32_t size, uint32_t timeout, uint32_t * r_data_written);

/**
 * @brief Take data from the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Takes data from the ring buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @param[out] dest Pointer to take data to
 * @param[in] size Amount of data to take
 * @param[out] r_data_taken Pointer to where to write real data amount taken. May be set to NULL, if not needed
 * @return Data take status
*/
rb_status_e rb_take(ring_buffer_t * rb, uint8_t * dest, uint32_t size, uint32_t * r_data_taken);
/**
 * @brief Take data from the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Takes data from the ring buffer, protected with mutex
 * @param[in] rb Ring buffer handle
 * @param[out] dest Pointer to take data to
 * @param[in] size Amount of data to take
 * @param[in] timeout Mutex acquizition timeout
 * @param[out] r_data_taken Pointer to where to write real data amount taken. May be set to NULL, if not needed
 * @return Data take status
*/
rb_status_e rb_take_m(ring_buffer_t * rb, uint8_t * dest, uint32_t size, uint32_t timeout, uint32_t * r_data_taken);

/**
 * @brief Flush buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Clears the buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @return Flush status
*/
rb_status_e rb_flush(ring_buffer_t * rb);
/**
 * @brief Flush buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Clears the buffer, protected with mutex
 * @param[in] rb Ring buffer handle
 * @param[in] timeout Mutex acquizition timeout
 * @return Flush status
*/
rb_status_e rb_flush_m(ring_buffer_t * rb, uint32_t timeout);

/**
 * @brief Get amount of bytes available
 * @details Get amount of bytes available in the ring buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @param[out] r_bytes_available Pointer to the variable to write available bytes amount to
 * @param[in] timeout Mutex acquizition timeout
 * @return Status
*/
rb_status_e rb_data_available(ring_buffer_t * rb, uint32_t * r_bytes_available);

/**
 * @brief Get amount of bytes available
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Get amount of bytes available in the ring buffer, mutex protected
 * @param[in] rb Ring buffer handle
 * @param[out] r_bytes_available Pointer to the variable to write available bytes amount to
 * @param[in] timeout Mutex acquizition timeout
 * @return Status
*/
rb_status_e rb_data_available_m(ring_buffer_t * rb, uint32_t * r_bytes_available, uint32_t timeout);

/**
 * @brief Push single element into the ring buffer
 * @warning Do not use this on uninitialized ring buffers, leads to UB
 * @details Push single element into the ring buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @param[in] data Data element to write
 * @return Push status
*/
rb_status_e rb_push_single(ring_buffer_t * rb, uint8_t data);

/**
 * @brief Push single element into the ring buffer
 * @warning Do not use this on uninitialized ring buffer, leads to UB
 * @details Push single element into the ring buffer, protected with mutex
 * @param[in] rb Ring buffer handle
 * @param[in] data Data element to write
 * @param[in] timeout Mutex acquizition timeout
 * @return Push status
*/
rb_status_e rb_push_single_m(ring_buffer_t * rb, uint8_t data, uint32_t timeout);

/**
 * @brief Take single element from the ring buffer
 * @warning Do not use this on uninitialized ring buffer, leads to UB
 * @details Take single element from the ring buffer, not protected with mutex. May also be used with mutex-protected ring buffers
 * @param[in] rb Ring buffer handle
 * @param[out] r_data Pointer where to place the obtained data
 * @return Take status
*/
rb_status_e rb_take_single(ring_buffer_t * rb, uint8_t * r_data);

/**
 * @brief Take single element from the ring buffer
 * @warning Do not use this on uninitialized ring buffer, leads to UB
 * @details Take single element from the ring buffer, protected with mutex
 * @param[in] rb Ring buffer handle
 * @param[out] r_data Pointer where to place the obtained data
 * @param[in] timeout Mutex acquizition timeout
 * @return Take status
*/
rb_status_e rb_take_single_m(ring_buffer_t * rb, uint8_t * r_data, uint32_t timeout);

#endif