#include "moves/ray_attacks.h"

#include <stdio.h>
#include <stdlib.h>

#include "bitscans.h"
 
const SMagic rookMagic[64] = {
	{attackTable + 0x0, 0x101010101017e, 0xa8002c000108020, 12},
	{attackTable + 0x1000, 0x202020202027c, 0x6c00049b0002001, 11},
	{attackTable + 0x1800, 0x404040404047a, 0x100200010090040, 11},
	{attackTable + 0x2000, 0x8080808080876, 0x2480041000800801, 11},
	{attackTable + 0x2800, 0x1010101010106e, 0x280028004000800, 11},
	{attackTable + 0x3000, 0x2020202020205e, 0x900410008040022, 11},
	{attackTable + 0x3800, 0x4040404040403e, 0x280020001001080, 11},
	{attackTable + 0x4000, 0x8080808080807e, 0x2880002041000080, 12},
	{attackTable + 0x5000, 0x1010101017e00, 0xa000800080400034, 11},
	{attackTable + 0x5800, 0x2020202027c00, 0x4808020004000, 10},
	{attackTable + 0x5c00, 0x4040404047a00, 0x2290802004801000, 10},
	{attackTable + 0x6000, 0x8080808087600, 0x411000d00100020, 10},
	{attackTable + 0x6400, 0x10101010106e00, 0x402800800040080, 10},
	{attackTable + 0x6800, 0x20202020205e00, 0xb000401004208, 10},
	{attackTable + 0x6c00, 0x40404040403e00, 0x2409000100040200, 10},
	{attackTable + 0x7000, 0x80808080807e00, 0x1002100004082, 11},
	{attackTable + 0x7800, 0x10101017e0100, 0x22878001e24000, 11},
	{attackTable + 0x8000, 0x20202027c0200, 0x1090810021004010, 10},
	{attackTable + 0x8400, 0x40404047a0400, 0x801030040200012, 10},
	{attackTable + 0x8800, 0x8080808760800, 0x500808008001000, 10},
	{attackTable + 0x8c00, 0x101010106e1000, 0xa08018014000880, 10},
	{attackTable + 0x9000, 0x202020205e2000, 0x8000808004000200, 10},
	{attackTable + 0x9400, 0x404040403e4000, 0x201008080010200, 10},
	{attackTable + 0x9800, 0x808080807e8000, 0x801020000441091, 11},
	{attackTable + 0xa000, 0x101017e010100, 0x800080204005, 11},
	{attackTable + 0xa800, 0x202027c020200, 0x1040200040100048, 10},
	{attackTable + 0xac00, 0x404047a040400, 0x120200402082, 10},
	{attackTable + 0xb000, 0x8080876080800, 0xd14880480100080, 10},
	{attackTable + 0xb400, 0x1010106e101000, 0x12040280080080, 10},
	{attackTable + 0xb800, 0x2020205e202000, 0x100040080020080, 10},
	{attackTable + 0xbc00, 0x4040403e404000, 0x9020010080800200, 10},
	{attackTable + 0xc000, 0x8080807e808000, 0x813241200148449, 11},
	{attackTable + 0xc800, 0x1017e01010100, 0x491604001800080, 11},
	{attackTable + 0xd000, 0x2027c02020200, 0x100401000402001, 10},
	{attackTable + 0xd400, 0x4047a04040400, 0x4820010021001040, 10},
	{attackTable + 0xd800, 0x8087608080800, 0x400402202000812, 10},
	{attackTable + 0xdc00, 0x10106e10101000, 0x209009005000802, 10},
	{attackTable + 0xe000, 0x20205e20202000, 0x810800601800400, 10},
	{attackTable + 0xe400, 0x40403e40404000, 0x4301083214000150, 10},
	{attackTable + 0xe800, 0x80807e80808000, 0x204026458e001401, 11},
	{attackTable + 0xf000, 0x17e0101010100, 0x40204000808000, 11},
	{attackTable + 0xf800, 0x27c0202020200, 0x8001008040010020, 10},
	{attackTable + 0xfc00, 0x47a0404040400, 0x8410820820420010, 10},
	{attackTable + 0x10000, 0x8760808080800, 0x1003001000090020, 10},
	{attackTable + 0x10400, 0x106e1010101000, 0x804040008008080, 10},
	{attackTable + 0x10800, 0x205e2020202000, 0x12000810020004, 10},
	{attackTable + 0x10c00, 0x403e4040404000, 0x1000100200040208, 10},
	{attackTable + 0x11000, 0x807e8080808000, 0x430000a044020001, 11},
	{attackTable + 0x11800, 0x7e010101010100, 0x280009023410300, 11},
	{attackTable + 0x12000, 0x7c020202020200, 0xe0100040002240, 10},
	{attackTable + 0x12400, 0x7a040404040400, 0x200100401700, 10},
	{attackTable + 0x12800, 0x76080808080800, 0x2244100408008080, 10},
	{attackTable + 0x12c00, 0x6e101010101000, 0x8000400801980, 10},
	{attackTable + 0x13000, 0x5e202020202000, 0x2000810040200, 10},
	{attackTable + 0x13400, 0x3e404040404000, 0x8010100228810400, 10},
	{attackTable + 0x13800, 0x7e808080808000, 0x2000009044210200, 11},
	{attackTable + 0x14000, 0x7e01010101010100, 0x4080008040102101, 12},
	{attackTable + 0x15000, 0x7c02020202020200, 0x40002080411d01, 11},
	{attackTable + 0x15800, 0x7a04040404040400, 0x2005524060000901, 11},
	{attackTable + 0x16000, 0x7608080808080800, 0x502001008400422, 11},
	{attackTable + 0x16800, 0x6e10101010101000, 0x489a000810200402, 11},
	{attackTable + 0x17000, 0x5e20202020202000, 0x1004400080a13, 11},
	{attackTable + 0x17800, 0x3e40404040404000, 0x4000011008020084, 11},
	{attackTable + 0x18000, 0x7e80808080808000, 0x26002114058042, 12},
};

const SMagic bishopMagic[64] = {
	{attackTable + 0x19000, 0x40201008040200, 0x89a1121896040240, 6},
	{attackTable + 0x19040, 0x402010080400, 0x2004844802002010, 5},
	{attackTable + 0x19060, 0x4020100a00, 0x2068080051921000, 5},
	{attackTable + 0x19080, 0x40221400, 0x62880a0220200808, 5},
	{attackTable + 0x190a0, 0x2442800, 0x4042004000000, 5},
	{attackTable + 0x190c0, 0x204085000, 0x100822020200011, 5},
	{attackTable + 0x190e0, 0x20408102000, 0xc00444222012000a, 5},
	{attackTable + 0x19100, 0x2040810204000, 0x28808801216001, 6},
	{attackTable + 0x19140, 0x20100804020000, 0x400492088408100, 5},
	{attackTable + 0x19160, 0x40201008040000, 0x201c401040c0084, 5},
	{attackTable + 0x19180, 0x4020100a0000, 0x840800910a0010, 5},
	{attackTable + 0x191a0, 0x4022140000, 0x82080240060, 5},
	{attackTable + 0x191c0, 0x244280000, 0x2000840504006000, 5},
	{attackTable + 0x191e0, 0x20408500000, 0x30010c4108405004, 5},
	{attackTable + 0x19200, 0x2040810200000, 0x1008005410080802, 5},
	{attackTable + 0x19220, 0x4081020400000, 0x8144042209100900, 5},
	{attackTable + 0x19240, 0x10080402000200, 0x208081020014400, 5},
	{attackTable + 0x19260, 0x20100804000400, 0x4800201208ca00, 5},
	{attackTable + 0x19280, 0x4020100a000a00, 0xf18140408012008, 7},
	{attackTable + 0x19300, 0x402214001400, 0x1004002802102001, 7},
	{attackTable + 0x19380, 0x24428002800, 0x841000820080811, 7},
	{attackTable + 0x19400, 0x2040850005000, 0x40200200a42008, 7},
	{attackTable + 0x19480, 0x4081020002000, 0x800054042000, 5},
	{attackTable + 0x194a0, 0x8102040004000, 0x88010400410c9000, 5},
	{attackTable + 0x194c0, 0x8040200020400, 0x520040470104290, 5},
	{attackTable + 0x194e0, 0x10080400040800, 0x1004040051500081, 5},
	{attackTable + 0x19500, 0x20100a000a1000, 0x2002081833080021, 7},
	{attackTable + 0x19580, 0x40221400142200, 0x400c00c010142, 9},
	{attackTable + 0x19780, 0x2442800284400, 0x941408200c002000, 9},
	{attackTable + 0x19980, 0x4085000500800, 0x658810000806011, 7},
	{attackTable + 0x19a00, 0x8102000201000, 0x188071040440a00, 5},
	{attackTable + 0x19a20, 0x10204000402000, 0x4800404002011c00, 5},
	{attackTable + 0x19a40, 0x4020002040800, 0x104442040404200, 5},
	{attackTable + 0x19a60, 0x8040004081000, 0x511080202091021, 5},
	{attackTable + 0x19a80, 0x100a000a102000, 0x4022401120400, 7},
	{attackTable + 0x19b00, 0x22140014224000, 0x80c0040400080120, 9},
	{attackTable + 0x19d00, 0x44280028440200, 0x8040010040820802, 9},
	{attackTable + 0x19f00, 0x8500050080400, 0x480810700020090, 7},
	{attackTable + 0x19f80, 0x10200020100800, 0x102008e00040242, 5},
	{attackTable + 0x19fa0, 0x20400040201000, 0x809005202050100, 5},
	{attackTable + 0x19fc0, 0x2000204081000, 0x8002024220104080, 5},
	{attackTable + 0x19fe0, 0x4000408102000, 0x431008804142000, 5},
	{attackTable + 0x1a000, 0xa000a10204000, 0x19001802081400, 7},
	{attackTable + 0x1a080, 0x14001422400000, 0x200014208040080, 7},
	{attackTable + 0x1a100, 0x28002844020000, 0x3308082008200100, 7},
	{attackTable + 0x1a180, 0x50005008040200, 0x41010500040c020, 7},
	{attackTable + 0x1a200, 0x20002010080400, 0x4012020c04210308, 5},
	{attackTable + 0x1a220, 0x40004020100800, 0x208220a202004080, 5},
	{attackTable + 0x1a240, 0x20408102000, 0x111040120082000, 5},
	{attackTable + 0x1a260, 0x40810204000, 0x6803040141280a00, 5},
	{attackTable + 0x1a280, 0xa1020400000, 0x2101004202410000, 5},
	{attackTable + 0x1a2a0, 0x142240000000, 0x8200000041108022, 5},
	{attackTable + 0x1a2c0, 0x284402000000, 0x21082088000, 5},
	{attackTable + 0x1a2e0, 0x500804020000, 0x2410204010040, 5},
	{attackTable + 0x1a300, 0x201008040200, 0x40100400809000, 5},
	{attackTable + 0x1a320, 0x402010080400, 0x822088220820214, 5},
	{attackTable + 0x1a340, 0x2040810204000, 0x40808090012004, 6},
	{attackTable + 0x1a380, 0x4081020400000, 0x910224040218c9, 5},
	{attackTable + 0x1a3a0, 0xa102040000000, 0x402814422015008, 5},
	{attackTable + 0x1a3c0, 0x14224000000000, 0x90014004842410, 5},
	{attackTable + 0x1a3e0, 0x28440200000000, 0x1000042304105, 5},
	{attackTable + 0x1a400, 0x50080402000000, 0x10008830412a00, 5},
	{attackTable + 0x1a420, 0x20100804020000, 0x2520081090008908, 5},
	{attackTable + 0x1a440, 0x40201008040200, 0x40102000a0a60140, 6},
};

bitboard index_to_bitboard(int index, bitboard m) {
	int i;
	bitboard j, result = 0ULL;

	for(i = 0; m; i++)
	{
		j = lsb(m);
		if(index & (1 << i))
		{
			result |= j;
		}
		m = clear_lsb(m);
	}

	return result;
}
 
bitboard rook_mask(int square)
{
	bitboard result = 0ULL;
	int rank = square / 8, file = square % 8, r, f;

	for(r = rank + 1; r <= 6; r++)
	{
		result |= (1ULL << (file + r*8));
	}
	for(r = rank - 1; r >= 1; r--)
	{
		result |= (1ULL << (file + r*8));
	}
	for(f = file + 1; f <= 6; f++)
	{
		result |= (1ULL << (f + rank * 8));
	}
	for(f = file - 1; f >= 1; f--)
	{
		result |= (1ULL << (f + rank * 8));
	}

	return result;
}
 
bitboard bishop_mask(int square)
{
	bitboard result = 0ULL;
	int rank = square / 8, file = square % 8, r, f;

	for(r=rank + 1, f=file + 1; r <= 6 && f <= 6; r++, f++)
	{
		result |= (1ULL << (f + r*8));
	}
	for(r=rank + 1, f=file - 1; r <= 6 && f >= 1; r++, f--)
	{
		result |= (1ULL << (f + r*8));
	}
	for(r=rank - 1, f=file + 1; r >= 1 && f <= 6; r--, f++)
	{
		result |= (1ULL << (f + r*8));
	}
	for(r=rank - 1, f=file - 1; r >= 1 && f >= 1; r--, f--)
	{
		result |= (1ULL << (f + r*8));
	}

	return result;
}
 
bitboard rook_attack(int square, bitboard block)
{
	bitboard result = 0ULL;
	int rank = square / 8, file = square % 8, r, f;

	for(r = rank + 1; r <= 7; r++)
	{
		result |= (1ULL << (file + r * 8));
		if(block & (1ULL << (file + r * 8)))
		{
			break;
		}
	}
	for(r = rank - 1; r >= 0; r--)
	{
		result |= (1ULL << (file + r * 8));
		if(block & (1ULL << (file + r * 8)))
		{
			break;
		}
	}
	for(f = file + 1; f <= 7; f++)
	{
		result |= (1ULL << (f + rank * 8));
		if(block & (1ULL << (f + rank * 8)))
		{
			break;
		}
	}
	for(f = file - 1; f >= 0; f--)
	{
		result |= (1ULL << (f + rank * 8));
		if(block & (1ULL << (f + rank * 8)))
		{
			break;
		}
	}

	return result;
}
 
bitboard bishop_attack(int square, bitboard block)
{
	bitboard result = 0ULL;
	int rank = square / 8, file = square % 8, r, f;

	for(r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++)
	{
		result |= (1ULL << (f + r * 8));
		if(block & (1ULL << (f + r * 8)))
		{
			break;
		}
	}
	for(r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--)
	{
		result |= (1ULL << (f + r * 8));
		if(block & (1ULL << (f + r * 8)))
		{
			break;
		}
	}
	for(r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++)
	{
		result |= (1ULL << (f + r * 8));
		if(block & (1ULL << (f + r * 8)))
		{
			break;
		}
	}
	for(r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
	{
		result |= (1ULL << (f + r * 8));
		if(block & (1ULL << (f + r * 8)))
		{
			break;
		}
	}

	return result;
}
 
void init_magic(void)
{
	int i, square;
	bitboard mask, occupied, j;
	

	for (i = 0; i < ATTACK_TABLE_SIZE; ++i)
	{
		attackTable[i] = 0;
	}

	for (square = 0; square < 64; ++square)
	{
		mask = rook_mask(square);
		for (i = 0; i < (1 << rookMagic[square].shift); ++i)
		{
			occupied = index_to_bitboard(i, mask);
			j = occupied;
			j &= rookMagic[square].mask;
			j *= rookMagic[square].magic;
			j >>= 64 - rookMagic[square].shift;
			rookMagic[square].ptr[j] = rook_attack(square, occupied);
		}
	}
	for (square = 0; square < 64; ++square)
	{
		mask = bishop_mask(square);
		for (i = 0; i < (1 << rookMagic[square].shift); ++i)
		{
			occupied = index_to_bitboard(i, mask);
			j = occupied;
			j &= bishopMagic[square].mask;
			j *= bishopMagic[square].magic;
			j >>= 64 - bishopMagic[square].shift;
			bishopMagic[square].ptr[j] = bishop_attack(square, occupied);
		}
	}
}

