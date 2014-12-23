#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "game.h"
#include "bitscans.h"

#include "moves/knight_moves.h"
#include "moves/ray_attacks.h"
#include "moves/king_moves.h"
#include "moves/pawn_attacks.h"

#include "hashing/transposition_table.h"

int is_valid_pseudo_move(Board* board, Move move);
unsigned int filter_pseudo_moves(Board* board, Move* moves, unsigned int numMoves);

unsigned int generate_white_moves(Board* board, Move* moves);
unsigned int generate_black_moves(Board* board, Move* moves);

unsigned int generate_white_pawns(Board* board, Move* moves);
unsigned int generate_black_pawns(Board* board, Move* moves);

unsigned int generate_knights(Board* board, Move* moves,
					  int piece, int mySide, int theirSide);
unsigned int generate_rooks(Board* board, Move* moves,
					int piece, int mySide, int theirSide);
unsigned int generate_bishops(Board* board, Move* moves,
					int piece, int mySide, int theirSide);
unsigned int generate_queens(Board* board, Move* moves,
					int piece, int mySide, int theirSide);
unsigned int generate_king(Board* board, Move* moves,
					int piece, int mySide, int theirSide);

int white_attacks_square(Board* board, int square);
int black_attacks_square(Board* board, int square);

unsigned int generate_moves(Board* board, Move* moves)
{
	int num_moves = 0;
	if (board->sideToMove == WHITE)
	{
		num_moves = generate_white_moves(board, moves);
	}
	else
	{
		num_moves = generate_black_moves(board, moves);
	}

	num_moves = filter_pseudo_moves(board, moves, num_moves);

	return num_moves;
}

int is_valid_pseudo_move(Board* board, Move move)
{
	int valid = 1;

	make_move(board, &move);
	if ((board->sideToMove == BLACK &&
		black_attacks_square(board, bit_scan_forward(board->pieces[WHITE_KING]))) ||
		(board->sideToMove == WHITE &&
		white_attacks_square(board, bit_scan_forward(board->pieces[BLACK_KING]))))
	{
		valid = 0;
	}
	unmake_move(board, move);

	return valid;
}

unsigned int filter_pseudo_moves(Board* board, Move* moves, unsigned int numMoves)
{
	int i = 0, j = 0;

	while(i < numMoves)
	{
		if (is_valid_pseudo_move(board, moves[i]))
		{
			++i;
		}
		else
		{
			for (j = i + 1; j < numMoves; ++j)
			{
				moves[j-1] = moves[j];
			}
			--numMoves;
		}
	}
	return numMoves;
}

unsigned int generate_white_moves(Board* board, Move* moves)
{
	bitboard kingSideMask = (1ull << F1 | 1ull << G1);
	bitboard queenSideMask = (1ull << B1 | 1ull << C1 | 1ull << D1);
	int numMoves = 0;

	numMoves += generate_white_pawns(board, moves + numMoves);
	numMoves += generate_knights(board, moves + numMoves, WHITE_KNIGHT, WHITE, BLACK);
	numMoves += generate_rooks(board, moves + numMoves, WHITE_ROOK, WHITE, BLACK);
	numMoves += generate_bishops(board, moves + numMoves, WHITE_BISHOP, WHITE, BLACK);
	numMoves += generate_queens(board, moves + numMoves, WHITE_QUEEN, WHITE, BLACK);
	numMoves += generate_king(board, moves + numMoves, WHITE_KING, WHITE, BLACK);

	// Castling
	if (board->castling & (1ull << WHITE_KINGSIDE) &&
		(board->empty & kingSideMask) == kingSideMask &&
		!black_attacks_square(board, E1) &&
		!black_attacks_square(board, F1) && !black_attacks_square(board, G1))
	{
		moves[numMoves].from = E1;
		moves[numMoves].to = G1;
		moves[numMoves].flags = SPECIAL1_FLAG;
		moves[numMoves].piece = WHITE_KING;
		moves[numMoves].side = WHITE;
		++numMoves;
	}
	if (board->castling & (1ull << WHITE_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!black_attacks_square(board, E1) &&
		!black_attacks_square(board, C1) && !black_attacks_square(board, D1))
	{
		moves[numMoves].from = E1;
		moves[numMoves].to = C1;
		moves[numMoves].flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
		moves[numMoves].piece = WHITE_KING;
		moves[numMoves].side = WHITE;
		++numMoves;
	}

	return numMoves;
}

unsigned int generate_black_moves(Board* board, Move* moves)
{
	bitboard kingSideMask = (1ull << F8 | 1ull << G8);
	bitboard queenSideMask = (1ull << B8 | 1ull << C8 | 1ull << D8);
	int numMoves = 0;

	numMoves += generate_black_pawns(board, moves + numMoves);
	numMoves += generate_knights(board, moves + numMoves, BLACK_KNIGHT, BLACK, WHITE);
	numMoves += generate_rooks(board, moves + numMoves, BLACK_ROOK, BLACK, WHITE);
	numMoves += generate_bishops(board, moves + numMoves, BLACK_BISHOP, BLACK, WHITE);
	numMoves += generate_queens(board, moves + numMoves, BLACK_QUEEN, BLACK, WHITE);
	numMoves += generate_king(board, moves + numMoves, BLACK_KING, BLACK, WHITE);

	// Castling
	if (board->castling & (1ull << BLACK_KINGSIDE) &&
		(board->empty & kingSideMask) == kingSideMask &&
		!white_attacks_square(board, E8) &&
		!white_attacks_square(board, F8) && !white_attacks_square(board, G8))
	{
		moves[numMoves].from = E8;
		moves[numMoves].to = G8;
		moves[numMoves].flags = SPECIAL1_FLAG;
		moves[numMoves].piece = BLACK_KING;
		moves[numMoves].side = BLACK;
		++numMoves;
	}
	if (board->castling & (1ull << BLACK_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!white_attacks_square(board, E8) &&
		!white_attacks_square(board, C8) && !white_attacks_square(board, D8))
	{
		moves[numMoves].from = E8;
		moves[numMoves].to = C8;
		moves[numMoves].flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
		moves[numMoves].piece = BLACK_KING;
		moves[numMoves].side = BLACK;
		++numMoves;
	}
	return numMoves;
}

unsigned int generate_white_pawns(Board* board, Move* moves)
{
	bitboard promotionRank = ranks[6];
	bitboard epAttackRank = ranks[5];
	bitboard startRank = ranks[1];
	bitboard promoPawns = board->pieces[WHITE_PAWN] & promotionRank;
	bitboard nonPromoPawns = board->pieces[WHITE_PAWN] & ~promotionRank;

	bitboard singlepromo = (promoPawns << 8) & board->empty;
	bitboard singlepush = (nonPromoPawns << 8) & board->empty;
	bitboard doublepush = ((nonPromoPawns & startRank) << 8) & board->empty;
	doublepush = (doublepush << 8) & board->empty;
	unsigned int numMoves = 0;

	while(singlepush)
	{
		int pawnSquare = bit_scan_forward(singlepush);
		moves[numMoves].flags = 0;
		moves[numMoves].piece = WHITE_PAWN;
		moves[numMoves].side = WHITE;
		moves[numMoves].to = pawnSquare;
		moves[numMoves].from = pawnSquare - 8;
		++numMoves;
		singlepush = clear_lsb(singlepush);
	}


	while(doublepush)
	{
		int pawnSquare = bit_scan_forward(doublepush);
		moves[numMoves].flags = SPECIAL0_FLAG;
		moves[numMoves].piece = WHITE_PAWN;
		moves[numMoves].side = WHITE;
		moves[numMoves].to = pawnSquare;
		moves[numMoves].from = pawnSquare - 16;
		++numMoves;
		doublepush = clear_lsb(doublepush);
	}

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		bitboard epAttacks = pawnAttacks[WHITE][pawnSquare] & board->enPassant & epAttackRank;

		while(attacks)
		{
			moves[numMoves].piece = WHITE_PAWN;
			moves[numMoves].side = WHITE;
			moves[numMoves].from = pawnSquare;
			moves[numMoves].to = bit_scan_forward(attacks);
			moves[numMoves].flags = CAPTURE_FLAG;
			++numMoves;
			attacks = clear_lsb(attacks);
		}
		while(epAttacks)
		{
			moves[numMoves].piece = WHITE_PAWN;
			moves[numMoves].side = WHITE;
			moves[numMoves].from = pawnSquare;
			moves[numMoves].to = bit_scan_forward(epAttacks);
			moves[numMoves].flags = CAPTURE_FLAG | SPECIAL0_FLAG;
			++numMoves;
			epAttacks = clear_lsb(epAttacks);	
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int i;
		int pawnSquare = bit_scan_forward(singlepromo);
		int promotionFlags[] = {PROMOTION_FLAG,
					PROMOTION_FLAG | SPECIAL0_FLAG,
					PROMOTION_FLAG | SPECIAL1_FLAG,
					PROMOTION_FLAG | SPECIAL0_FLAG | SPECIAL1_FLAG};
		for (i = 0; i < 4; ++i)
		{
			moves[numMoves].piece = WHITE_PAWN;
			moves[numMoves].side = WHITE;
			moves[numMoves].from = pawnSquare - 8;
			moves[numMoves].to = pawnSquare;
			moves[numMoves].flags = promotionFlags[i];
			++numMoves;
		}
		singlepromo = clear_lsb(singlepromo);
	}

	while(promoPawns)
	{
		int i;
		int pawnSquare = bit_scan_forward(promoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		int promotionFlags[] = {PROMOTION_FLAG | CAPTURE_FLAG,
					PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG,
					PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG,
					PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG | SPECIAL1_FLAG};
		while(attacks)
		{
			for (i = 0; i < 4; ++i)
			{
				moves[numMoves].piece = WHITE_PAWN;
				moves[numMoves].side = WHITE;
				moves[numMoves].from = pawnSquare;
				moves[numMoves].to = bit_scan_forward(attacks);
				moves[numMoves].flags = promotionFlags[i];
				++numMoves;
			}
			attacks = clear_lsb(attacks);
		}
		promoPawns = clear_lsb(promoPawns);
	}

	return numMoves;
}

unsigned int generate_black_pawns(Board* board, Move* moves)
{
	bitboard promotionRank = ranks[1];
	bitboard epAttackRank = ranks[2];
	bitboard startRank = ranks[6];
	bitboard promoPawns = board->pieces[BLACK_PAWN] & promotionRank;
	bitboard nonPromoPawns = board->pieces[BLACK_PAWN] & ~promotionRank;

	bitboard singlepromo = (promoPawns >> 8) & board->empty;
	bitboard singlepush = (nonPromoPawns >> 8) & board->empty;
	bitboard doublepush = ((nonPromoPawns & startRank) >> 8) & board->empty;
	doublepush = (doublepush >> 8) & board->empty;
	unsigned int numMoves = 0;

	while(singlepush)
	{
		int pawnSquare = bit_scan_forward(singlepush);
		moves[numMoves].flags = 0;
		moves[numMoves].piece = BLACK_PAWN;
		moves[numMoves].side = BLACK;
		moves[numMoves].to = pawnSquare;
		moves[numMoves].from = pawnSquare + 8;
		++numMoves;
		singlepush = clear_lsb(singlepush);
	}

	while(doublepush)
	{
		int pawnSquare = bit_scan_forward(doublepush);
		moves[numMoves].flags = SPECIAL0_FLAG;
		moves[numMoves].piece = BLACK_PAWN;
		moves[numMoves].side = BLACK;
		moves[numMoves].to = pawnSquare;
		moves[numMoves].from = pawnSquare + 16;
		++numMoves;
		doublepush = clear_lsb(doublepush);
	}

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		bitboard epAttacks = pawnAttacks[BLACK][pawnSquare] & board->enPassant & epAttackRank;
		while(attacks)
		{
			moves[numMoves].piece = BLACK_PAWN;
			moves[numMoves].side = BLACK;
			moves[numMoves].from = pawnSquare;
			moves[numMoves].to = bit_scan_forward(attacks);
			moves[numMoves].flags = CAPTURE_FLAG;
			++numMoves;
			attacks = clear_lsb(attacks);
		}
		while(epAttacks)
		{
			moves[numMoves].piece = BLACK_PAWN;
			moves[numMoves].side = BLACK;
			moves[numMoves].from = pawnSquare;
			moves[numMoves].to = bit_scan_forward(epAttacks);
			moves[numMoves].flags = CAPTURE_FLAG | SPECIAL0_FLAG;
			++numMoves;
			epAttacks = clear_lsb(epAttacks);
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int i;
		int pawnSquare = bit_scan_forward(singlepromo);
		int promotionFlags[] = {PROMOTION_FLAG,
					PROMOTION_FLAG | SPECIAL0_FLAG,
					PROMOTION_FLAG | SPECIAL1_FLAG,
					PROMOTION_FLAG | SPECIAL0_FLAG | SPECIAL1_FLAG};
		for (i = 0; i < 4; ++i)
		{
			moves[numMoves].flags = 0;
			moves[numMoves].piece = BLACK_PAWN;
			moves[numMoves].side = BLACK;
			moves[numMoves].from = pawnSquare + 8;
			moves[numMoves].to = pawnSquare;
			moves[numMoves].flags = promotionFlags[i];
			++numMoves;
		}
		singlepromo = clear_lsb(singlepromo);
	}

	while(promoPawns)
	{
		int i;
		int pawnSquare = bit_scan_forward(promoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		int promotionFlags[] = {PROMOTION_FLAG | CAPTURE_FLAG,
					PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG,
					PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG,
					PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG | SPECIAL1_FLAG};
		while(attacks)
		{
			for (i = 0; i < 4; ++i)
			{
				moves[numMoves].flags = 0;
				moves[numMoves].piece = BLACK_PAWN;
				moves[numMoves].side = BLACK;
				moves[numMoves].from = pawnSquare;
				moves[numMoves].to = bit_scan_forward(attacks);
				moves[numMoves].flags = promotionFlags[i];
				++numMoves;
			}
			attacks = clear_lsb(attacks);
		}
		promoPawns = clear_lsb(promoPawns);
	}

	return numMoves;
}

unsigned int generate_knights(Board* board, Move* moves,
					  int piece, int mySide, int theirSide)
{
	bitboard allKnights = board->pieces[piece];
	unsigned int numMoves = 0;

	while(allKnights)
	{
		int knightSquare = bit_scan_forward(allKnights);
		bitboard allMoves = knightMoves[knightSquare] & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;
		while(allMoves)
		{
			moves[numMoves].from = knightSquare;
			moves[numMoves].to = bit_scan_forward(allMoves);
			moves[numMoves].flags = 0;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			moves[numMoves].from = knightSquare;
			moves[numMoves].to = bit_scan_forward(captures);
			moves[numMoves].flags = CAPTURE_FLAG;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			captures = clear_lsb(captures);
		}
		allKnights = clear_lsb(allKnights);
	}

	return numMoves;
}

unsigned int generate_rooks(Board* board, Move* moves,
					int piece, int mySide, int theirSide)
{
	bitboard allRooks = board->pieces[piece];
	unsigned int numMoves = 0;

	while(allRooks)
	{
		int rookSquare = bit_scan_forward(allRooks);
		bitboard allMoves = rook_attacks(board->occupied, rookSquare) & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;

		while(allMoves)
		{
			moves[numMoves].from = rookSquare;
			moves[numMoves].to = bit_scan_forward(allMoves);
			moves[numMoves].flags = 0;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			moves[numMoves].from = rookSquare;
			moves[numMoves].to = bit_scan_forward(captures);
			moves[numMoves].flags = CAPTURE_FLAG;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			captures = clear_lsb(captures);
		}
		allRooks = clear_lsb(allRooks);
	}

	return numMoves;
}

unsigned int generate_bishops(Board* board, Move* moves,
					int piece, int mySide, int theirSide)
{
	bitboard allBishops = board->pieces[piece];
	unsigned int numMoves = 0;

	while(allBishops)
	{
		int bishopSquare = bit_scan_forward(allBishops);
		bitboard allMoves = bishop_attacks(board->occupied, bishopSquare) & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;

		while(allMoves)
		{
			moves[numMoves].from = bishopSquare;
			moves[numMoves].to = bit_scan_forward(allMoves);
			moves[numMoves].flags = 0;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			moves[numMoves].from = bishopSquare;
			moves[numMoves].to = bit_scan_forward(captures);
			moves[numMoves].flags = CAPTURE_FLAG;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			captures = clear_lsb(captures);
		}
		allBishops = clear_lsb(allBishops);
	}

	return numMoves;
}

unsigned int generate_queens(Board* board, Move* moves,
					int piece, int mySide, int theirSide)
{
	bitboard allQueens = board->pieces[piece];
	unsigned int numMoves = 0;

	while(allQueens)
	{
		int queenSquare = bit_scan_forward(allQueens);
		bitboard allMoves = queen_attacks(board->occupied, queenSquare) & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;

		while(allMoves)
		{
			moves[numMoves].from = queenSquare;
			moves[numMoves].to = bit_scan_forward(allMoves);
			moves[numMoves].flags = 0;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			moves[numMoves].from = queenSquare;
			moves[numMoves].to = bit_scan_forward(captures);
			moves[numMoves].flags = CAPTURE_FLAG;
			moves[numMoves].piece = piece;
			moves[numMoves].side = mySide;
			++numMoves;

			captures = clear_lsb(captures);
		}
		allQueens = clear_lsb(allQueens);
	}

	return numMoves;
}

unsigned int generate_king(Board* board, Move* moves,
					int piece, int mySide, int theirSide)
{
	int kingSquare = bit_scan_forward(board->pieces[piece]);
	bitboard allMoves = kingMoves[kingSquare] & ~board->sides[mySide];
	bitboard captures = allMoves & board->sides[theirSide];
	allMoves &= ~captures;
	unsigned int numMoves = 0;


	while(allMoves)
	{
		moves[numMoves].from = kingSquare;
		moves[numMoves].flags = 0;
		moves[numMoves].piece = piece;
		moves[numMoves].side = mySide;
		moves[numMoves].to = bit_scan_forward(allMoves);
		++numMoves;

		allMoves = clear_lsb(allMoves);
	}
	while(captures)
	{
		moves[numMoves].from = kingSquare;
		moves[numMoves].flags = CAPTURE_FLAG;
		moves[numMoves].piece = piece;
		moves[numMoves].side = mySide;
		moves[numMoves].to = bit_scan_forward(captures);
		++numMoves;

		captures = clear_lsb(captures);
	}

	return numMoves;
}

int white_attacks_square(Board* board, int square)
{
	bitboard rookQueens = board->pieces[WHITE_ROOK] | board->pieces[WHITE_QUEEN];
	bitboard bishQueens = board->pieces[WHITE_BISHOP] | board->pieces[WHITE_QUEEN];

	return (pawnAttacks[BLACK][square] & board->pieces[WHITE_PAWN] ||
		knightMoves[square] & board->pieces[WHITE_KNIGHT] ||
		bishop_attacks(board->occupied, square) & bishQueens ||
		rook_attacks(board->occupied, square) & rookQueens ||
		kingMoves[square] & board->pieces[WHITE_KING]);
}

int black_attacks_square(Board* board, int square)
{
	bitboard rookQueens = board->pieces[BLACK_ROOK] | board->pieces[BLACK_QUEEN];
	bitboard bishQueens = board->pieces[BLACK_BISHOP] | board->pieces[BLACK_QUEEN];

	return (pawnAttacks[WHITE][square] & board->pieces[BLACK_PAWN] ||
		knightMoves[square] & board->pieces[BLACK_KNIGHT] ||
		bishop_attacks(board->occupied, square) & bishQueens ||
		rook_attacks(board->occupied, square) & rookQueens ||
		kingMoves[square] & board->pieces[BLACK_KING]);

}
