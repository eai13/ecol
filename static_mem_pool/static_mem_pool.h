#ifndef _ECOL_STATIC_MEM_POOL_H_
#define _ECOL_STATIC_MEM_POOL_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/// @brief SMP size in bytes
#define STATIC_MEM_POOL_SIZE    56

void smp_initialize(void);

/// @brief Free status enum
typedef enum{
    SMP_FREE_OK                 = ((uint8_t)(0x00)),    ///< Free is okay
    SMP_FREE_NOT_IN_POOL        = ((uint8_t)(0x01)),    ///< Pointer is not in the current pool
    SMP_FREE_ALREADY_FREED      = ((uint8_t)(0x02)),    ///< Place in SMP is free, nothing is allocated there
    SMP_FREE_IS_RESOURCE_PTR    = ((uint8_t)(0x03))     ///< Pointer points to the resource space (chunk header space)
}smp_free_status_e;

void *              smp_malloc(uint16_t size);
smp_free_status_e   smp_free(void * ptr);

// Utility functions, may delete
/* Prints global pointer in local memory pool */
/**/ uint32_t get_local(void * ptr);
/* Prints the whole map of static pool */
/**/ void print_map(void);
//

#endif