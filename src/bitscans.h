#ifndef BITSCANS_H
#define BITSCANS_H


static inline int population(unsigned long long mask)
{
	int count = 0;
	for (; mask; ++count)
	{
		mask &= mask - 1;
	}
	return count;
}

static inline unsigned long long lsb(unsigned long long mask)
{
	return mask & (0 - mask);
}

static inline unsigned long long clear_lsb(unsigned long long mask)
{
	return mask & (mask - 1);
}

static inline unsigned long long mirror_horizontal(unsigned long long mask)
{
	const unsigned long long k1 = 0x5555555555555555ull;
	const unsigned long long k2 = 0x3333333333333333ull;
	const unsigned long long k4 = 0x0f0f0f0f0f0f0f0full;
	mask = ((mask >> 1) & k1) +  2*(mask & k1);
	mask = ((mask >> 2) & k2) +  4*(mask & k2);
	mask = ((mask >> 4) & k4) + 16*(mask & k4);
	return mask;
}

static inline unsigned int bit_scan_forward(unsigned long long mask)
{
    return __builtin_ffsll(mask) - 1;
}

static inline unsigned int bit_scan_reverse(unsigned long long mask)
{
    return 63 - __builtin_clzll(mask);
}

#endif
