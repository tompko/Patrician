#ifndef BITSCANS_H
#define BITSCANS_H

#include "defines.h"

#ifdef _MSC_VER
    #include <intrin.h>
    #ifdef _WIN64
        #pragma intrinsic(_BitScanForward64)
        #pragma intrinsic(_BitScanReverse64)
        #define USING_INTRINSICS
    #endif

INLINE unsigned long bit_scan_forward(unsigned long long mask)
{
	unsigned long index;
	_BitScanForward64(&index, mask);
	return index;
}

INLINE unsigned long bit_scan_reverse(unsigned long long mask)
{
	unsigned long index;
	_BitScanReverse64(&index, mask);
	return index;
}
#else

INLINE unsigned int bit_scan_forward(unsigned long long mask)
{
    return __builtin_ffsll(mask) - 1;
}

INLINE unsigned int bit_scan_reverse(unsigned long long mask)
{
    return 63 - __builtin_clzll(mask);
}
#endif

INLINE int population(unsigned long long mask)
{
	int count = 0;
	for (; mask; ++count)
	{
		mask &= mask - 1;
	}
	return count;
}

INLINE unsigned long long lsb(unsigned long long mask)
{
	return mask & (0 - mask);
}

INLINE unsigned long long clear_lsb(unsigned long long mask)
{
	return mask & (mask - 1);
}

#endif