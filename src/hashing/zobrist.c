#include "zobrist.h"

#include <assert.h>

#include "hashing/mt64.h"
#include "board.h"
#include "bitscans.h"

static const unsigned long long mt64_seed = 0;

unsigned long long pieceKeys[12][64];
unsigned long long blackToMoveKey;
unsigned long long castlingKeys[16];
unsigned long long enPassantKeys[8];

void init_zobrist_keys(void)
{
	int i, j;

	init_genrand64(mt64_seed);

	for (i = 0; i < 12; ++i)
	{
		for (j = 0; j < 64; ++j)
		{
			pieceKeys[i][j] = genrand64_int64();
		}
	}

	blackToMoveKey = genrand64_int64();

	for (i = 0; i < 16; ++i)
	{
		castlingKeys[i] = genrand64_int64();
	}

	for (i = 0; i < 8; ++i)
	{
		enPassantKeys[i] = genrand64_int64();
	}
}

unsigned long long calculate_zobrist(struct Board * board)
{
	unsigned long long zobristHash = 0;
	int i;

	for(i = 0; i < 12; ++i)
	{
		bitboard piece = board->pieces[i];
		while (piece)
		{
			int index = bit_scan_forward(piece);
			zobristHash ^= pieceKeys[i][index];
			piece = clear_lsb(piece);
		}
	}

	if (board->sideToMove == BLACK)
	{
		zobristHash ^= blackToMoveKey;
	}

	assert(board->castling < 16);

	zobristHash ^= castlingKeys[board->castling];

	if (board->enPassant)
	{
		int index = bit_scan_forward(board->enPassant);
		zobristHash ^= enPassantKeys[index];
	}

	return zobristHash;
}
