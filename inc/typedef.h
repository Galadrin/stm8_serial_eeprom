//
// Created by leroy on 23/10/17.
//

#ifndef LT200_DATAPLUG_TYPEDEF_H
#define LT200_DATAPLUG_TYPEDEF_H

/* Signed.  */

typedef signed char             int8_t;
typedef short int               int16_t;
typedef long int                int32_t;

/* Unsigned.  */
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned long int       uint32_t;

/* Types for `void *' pointers.  */
typedef int                     intptr_t;
typedef unsigned int            uintptr_t;

/* Values to test for integral types holding `void *' pointer.  */
#define INTPTR_MIN             (-32767-1)
#define INTPTR_MAX             (32767)
#define UINTPTR_MAX            (65535)

/* Minimum for largest signed integral type.  */
#define INTMAX_MIN             (-__INT32_C(-2147483647L)-1)
/* Maximum for largest signed integral type.  */
#define INTMAX_MAX             (__INT32_C(2147483647L))

/* Maximum for largest unsigned integral type.  */
#define UINTMAX_MAX            (__UINT32_C(4294967295UL))


/* Limits of other integer types.  */

/* Limits of `ptrdiff_t' type.  */
#define PTRDIFF_MIN           (-32767-1)
#define PTRDIFF_MAX           (32767)

/* Limit of `size_t' type.  */
#define SIZE_MAX               (65535u)

#endif //LT200_DATAPLUG_TYPEDEF_H
