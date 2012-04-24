#ifndef DEFINES_H
#define DEFINES_H

#ifdef _MSC_VER

#define INLINE __inline
#define UNUSED(x) x

#else

#define INLINE static inline
#define UNUSED(x) 

#endif

#endif