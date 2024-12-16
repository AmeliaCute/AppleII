#ifndef STANDARD_TYPE_DEFINITION
#define STANDARD_TYPE_DEFINITION

/** 
 * @brief Standard type definition for uint*_t type 
 */

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef uint32_t        uintptr_t;
typedef unsigned long   uint64_t;
typedef unsigned long   size_t;

/**
 * @brief Type definition for boolean
 *
 * @note This type is not standard in C++
 */
typedef unsigned int bool;

#define TRUE    1
#define true    1

#define FALSE   0
#define false   0

/**
 * @brief Type definition for NULL 
 */

#define NULL    0
#define nil     0

#endif //! STANDARD_TYPE_DEFINITION