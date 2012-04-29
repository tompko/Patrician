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

INLINE unsigned long long mirror_horizontal(unsigned long long mask)
{
	const unsigned long long k1 = 0x5555555555555555ull;
	const unsigned long long k2 = 0x3333333333333333ull;
	const unsigned long long k4 = 0x0f0f0f0f0f0f0f0full;
	mask = ((mask >> 1) & k1) +  2*(mask & k1);
	mask = ((mask >> 2) & k2) +  4*(mask & k2);
	mask = ((mask >> 4) & k4) + 16*(mask & k4);
	return mask;
}

#endif