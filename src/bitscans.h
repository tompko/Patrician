#ifndef BITSCANS_H
#define BITSCANS_H

#include "defines.h"

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

#ifdef _MSC_VER
    #include <intrin.h>
    #ifdef _WIN64
        #pragma intrinsic(_BitScanForward64)
        #pragma intrinsic(_BitScanReverse64)
        #define USING_INTRINSICS
    #endif

INLINE unsigned long bit_scan_forward(unsigned long long mask)
{
#ifdef USING_INTRINSICS
	unsigned long index;
	_BitScanForward64(&index, mask);
	return index;
#else
	return population((mask & (0-mask)) - 1);
#endif
}

INLINE unsigned long bit_scan_reverse(unsigned long long mask)
{
#ifdef USING_INTRINSICS
	unsigned long index;
	_BitScanReverse64(&index, mask);
	return index;
#else
	const int ms1bTable[256] = {0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
							4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
							5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
							5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
							6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
							6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
							6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
							6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
							7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,};
	int result = 0;
	if (mask > 0xFFFFFFFFull)
	{
		mask >>= 32;
		result = 32;
	}
	if (mask > 0xFFFFull)
	{
		mask >>= 16;
		result += 16;
	}
	if (mask > 0xFFull)
	{
		mask >>= 8;
		result += 8;
	}
	return result + ms1bTable[mask];
#endif
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

#endif
