#ifndef BITSCANS_H
#define BITSCANS_H

#ifdef _MSC_VER
    #include <intrin.h>
    #ifdef _WIN64
        #pragma intrinsic(_BitScanForward64)
        #pragma intrinsic(_BitScanReverse64)
        #define USING_INTRINSICS
    #endif

__inline unsigned int bit_scan_forward(unsigned long long mask)
{
	int index = -1;
	_BitScanForward64(&index, mask);
	return index;
}
#endif

__inline int population(unsigned long long mask)
{
	int count = 0;
	for (; mask; ++count)
	{
		mask &= mask - 1;
	}
	return count;
}

__inline unsigned long long lsb(unsigned long long mask)
{
	return mask & -mask;
}

__inline unsigned long long clear_lsb(unsigned long long mask)
{
	return mask & (mask - 1);
}

#endif