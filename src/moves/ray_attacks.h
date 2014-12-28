#ifndef RAY_ATTACKS_H
#define RAY_ATTACKS_H

#include "../board.h"

#define ATTACK_TABLE_SIZE (0x80000)
bitboard attackTable[ATTACK_TABLE_SIZE];

typedef struct SMagic {
	bitboard* ptr;
	unsigned long long mask;
	unsigned long long magic;
	int shift;
} SMagic;

const SMagic rookMagic[64];
const SMagic bishopMagic[64];

void init_magic(void);

static inline bitboard rook_attacks(bitboard occupied, unsigned int square)
{
	occupied &= rookMagic[square].mask;
	occupied *= rookMagic[square].magic;
	occupied >>= 64 - rookMagic[square].shift;
	return rookMagic[square].ptr[occupied];
}
 
static inline bitboard bishop_attacks(bitboard occupied, unsigned int square)
{
	occupied &= bishopMagic[square].mask;
	occupied *= bishopMagic[square].magic;
	occupied >>= 64 - bishopMagic[square].shift;
	return bishopMagic[square].ptr[occupied];
}
 
static inline bitboard queen_attacks(bitboard occupied, unsigned int square)
{
  return rook_attacks(occupied, square) | bishop_attacks(occupied, square);
}

#endif

