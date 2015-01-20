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

	return num_moves;
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
		moves[numMoves++] = move_pack_move(E1,
		                                   G1,
		                                   WHITE_KING,
		                                   0,
		                                   0,
		                                   0,
		                                   0,
		                                   1);
	}
	if (board->castling & (1ull << WHITE_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!black_attacks_square(board, E1) &&
		!black_attacks_square(board, C1) && !black_attacks_square(board, D1))
	{
		moves[numMoves++] = move_pack_move(E1,
		                                   C1,
		                                   WHITE_KING,
		                                   0,
		                                   0,
		                                   0,
		                                   1,
		                                   1);
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
		moves[numMoves++] = move_pack_move(E8,
		                                   G8,
		                                   BLACK_KING,
		                                   0,
		                                   0,
		                                   0,
		                                   0,
		                                   1);
	}
	if (board->castling & (1ull << BLACK_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!white_attacks_square(board, E8) &&
		!white_attacks_square(board, C8) && !white_attacks_square(board, D8))
	{
		moves[numMoves++] = move_pack_move(E8,
		                                   C8,
		                                   BLACK_KING,
		                                   0,
		                                   0,
		                                   0,
		                                   1,
		                                   1);
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
		moves[numMoves++] = move_pack_move(pawnSquare - 8,
		                                   pawnSquare,
		                                   WHITE_PAWN,
		                                   0,
		                                   0,
		                                   0,
		                                   0,
		                                   0);
		singlepush = clear_lsb(singlepush);
	}


	while(doublepush)
	{
		int pawnSquare = bit_scan_forward(doublepush);
		moves[numMoves++] = move_pack_move(pawnSquare - 16,
		                                   pawnSquare,
		                                   WHITE_PAWN,
		                                   0,
		                                   0,
		                                   0,
		                                   1,
		                                   0);
		doublepush = clear_lsb(doublepush);
	}

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		bitboard epAttacks = pawnAttacks[WHITE][pawnSquare] & board->enPassant & epAttackRank;

		while(attacks)
		{
			int to = bit_scan_forward(attacks);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   WHITE_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   0,
			                                   0,
			                                   0);
			attacks = clear_lsb(attacks);
		}
		while(epAttacks)
		{
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   bit_scan_forward(epAttacks),
			                                   WHITE_PAWN,
			                                   BLACK_PAWN,
			                                   1,
			                                   0,
			                                   1,
			                                   0);
			epAttacks = clear_lsb(epAttacks);	
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int pawnSquare = bit_scan_forward(singlepromo);
		moves[numMoves++] = move_pack_move(pawnSquare - 8,
		                                   pawnSquare,
		                                   WHITE_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   0,
		                                   0);
		moves[numMoves++] = move_pack_move(pawnSquare - 8,
		                                   pawnSquare,
		                                   WHITE_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   0,
		                                   1);
		moves[numMoves++] = move_pack_move(pawnSquare - 8,
		                                   pawnSquare,
		                                   WHITE_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   1,
		                                   0);
		moves[numMoves++] = move_pack_move(pawnSquare - 8,
		                                   pawnSquare,
		                                   WHITE_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   1,
		                                   1);
		singlepromo = clear_lsb(singlepromo);
	}

	while(promoPawns)
	{
		int pawnSquare = bit_scan_forward(promoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		while(attacks)
		{
			int to = bit_scan_forward(attacks);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   WHITE_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   0,
			                                   0);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   WHITE_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   0,
			                                   1);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   WHITE_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   1,
			                                   0);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   WHITE_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   1,
			                                   1);
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
		moves[numMoves++] = move_pack_move(pawnSquare + 8,
		                                   pawnSquare,
		                                   BLACK_PAWN,
		                                   0,
		                                   0,
		                                   0,
		                                   0,
		                                   0);
		singlepush = clear_lsb(singlepush);
	}

	while(doublepush)
	{
		int pawnSquare = bit_scan_forward(doublepush);
		moves[numMoves++] = move_pack_move(pawnSquare + 16,
		                                   pawnSquare,
		                                   BLACK_PAWN,
		                                   0,
		                                   0,
		                                   0,
		                                   1,
		                                   0);
		doublepush = clear_lsb(doublepush);
	}

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		bitboard epAttacks = pawnAttacks[BLACK][pawnSquare] & board->enPassant & epAttackRank;
		while(attacks)
		{
			int to = bit_scan_forward(attacks);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   BLACK_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   0,
			                                   0,
			                                   0);
			attacks = clear_lsb(attacks);
		}
		while(epAttacks)
		{
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   bit_scan_forward(epAttacks),
			                                   BLACK_PAWN,
			                                   WHITE_PAWN,
			                                   1,
			                                   0,
			                                   1,
			                                   0);
			epAttacks = clear_lsb(epAttacks);
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int pawnSquare = bit_scan_forward(singlepromo);
		moves[numMoves++] = move_pack_move(pawnSquare + 8,
		                                   pawnSquare,
		                                   BLACK_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   0,
		                                   0);
		moves[numMoves++] = move_pack_move(pawnSquare + 8,
		                                   pawnSquare,
		                                   BLACK_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   0,
		                                   1);
		moves[numMoves++] = move_pack_move(pawnSquare + 8,
		                                   pawnSquare,
		                                   BLACK_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   1,
		                                   0);
		moves[numMoves++] = move_pack_move(pawnSquare + 8,
		                                   pawnSquare,
		                                   BLACK_PAWN,
		                                   0,
		                                   0,
		                                   1,
		                                   1,
		                                   1);
		singlepromo = clear_lsb(singlepromo);
	}

	while(promoPawns)
	{
		int pawnSquare = bit_scan_forward(promoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		while(attacks)
		{
			int to = bit_scan_forward(attacks);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   BLACK_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   0,
			                                   0);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   BLACK_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   0,
			                                   1);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   BLACK_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   1,
			                                   0);
			moves[numMoves++] = move_pack_move(pawnSquare,
			                                   to,
			                                   BLACK_PAWN,
			                                   board->mailbox[to],
			                                   1,
			                                   1,
			                                   1,
			                                   1);
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
			moves[numMoves++] = move_pack_move(knightSquare,
			                                   bit_scan_forward(allMoves),
			                                   piece,
			                                   0,
			                                   0,
			                                   0,
			                                   0,
			                                   0);
			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			unsigned int to = bit_scan_forward(captures);
			moves[numMoves++] = move_pack_move(knightSquare,
			                                   to,
			                                   piece,
			                                   board->mailbox[to],
			                                   1,
			                                   0,
			                                   0,
			                                   0);
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
			moves[numMoves++] = move_pack_move(rookSquare,
			                                   bit_scan_forward(allMoves),
			                                   piece,
			                                   0,
			                                   0,
			                                   0,
			                                   0,
			                                   0);
			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			unsigned int to = bit_scan_forward(captures);
			moves[numMoves++] = move_pack_move(rookSquare,
			                                   to,
			                                   piece,
			                                   board->mailbox[to],
			                                   1,
			                                   0,
			                                   0,
			                                   0);
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
			moves[numMoves++] = move_pack_move(bishopSquare,
			                                   bit_scan_forward(allMoves),
			                                   piece,
			                                   0,
			                                   0,
			                                   0,
			                                   0,
			                                   0);
			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			unsigned int to = bit_scan_forward(captures);
			moves[numMoves++] = move_pack_move(bishopSquare,
			                                   to,
			                                   piece,
			                                   board->mailbox[to],
			                                   1,
			                                   0,
			                                   0,
			                                   0);
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
			moves[numMoves++] = move_pack_move(queenSquare,
			                                   bit_scan_forward(allMoves),
			                                   piece,
			                                   0,
			                                   0,
			                                   0,
			                                   0,
			                                   0);
			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			unsigned int to = bit_scan_forward(captures);
			moves[numMoves++] = move_pack_move(queenSquare,
			                                   to,
			                                   piece,
			                                   board->mailbox[to],
			                                   1,
			                                   0,
			                                   0,
			                                   0);
			captures = clear_lsb(captures);
		}
		allQueens = clear_lsb(allQueens);
	}

	return numMoves;
}

unsigned int generate_king(Board* board, Move* moves,
					int piece, int mySide, int theirSide)
{
	unsigned int kingSquare = bit_scan_forward(board->pieces[piece]);
	bitboard allMoves = kingMoves[kingSquare] & ~board->sides[mySide];
	bitboard captures = allMoves & board->sides[theirSide];
	allMoves &= ~captures;
	unsigned int numMoves = 0;

	while(allMoves)
	{
		moves[numMoves++] = move_pack_move(kingSquare,
		                                   bit_scan_forward(allMoves),
		                                   piece,
		                                   0,
		                                   0,
		                                   0,
		                                   0,
		                                   0);
		allMoves = clear_lsb(allMoves);
	}
	while(captures)
	{
		unsigned int to = bit_scan_forward(captures);
		moves[numMoves++] = move_pack_move(kingSquare,
		                                   to,
		                                   piece,
		                                   board->mailbox[to],
		                                   1,
		                                   0,
		                                   0,
		                                   0);
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

