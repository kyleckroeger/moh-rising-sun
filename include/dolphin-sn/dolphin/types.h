#ifndef _DOLPHIN_TYPES_H_
#define _DOLPHIN_TYPES_H_

// Scalar subset of include/dolphin-sdk/dolphin/types.h for SN game code.
// Excludes the reference header's CodeWarrior libc includes; typedefs are unchanged.
// Attribution: src/dolphin/NOTICE and docs/Rendering.md.

typedef signed   char          s8;
typedef unsigned char          u8;
typedef signed   short int     s16;
typedef unsigned short int     u16;
typedef signed   long          s32;
typedef unsigned long          u32;
typedef signed   long long int s64;
typedef unsigned long long int u64;

typedef float  f32;
typedef double f64;

typedef char *Ptr;

typedef int BOOL;

#define FALSE 0
#define TRUE 1

#endif
