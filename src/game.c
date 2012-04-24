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

int board_perft(Board* board, MoveNode* moves, int level);

void generate_white_moves(Board* board, MoveNode* movenode);
void generate_black_moves(Board* board, MoveNode* movenode);

void generate_white_pawns(Board* board, MoveNode* movenode);
void generate_black_pawns(Board* board, MoveNode* movenode);

void generate_knights(Board* board, MoveNode* movenode,
					  int piece, int mySide, int theirSide);
void generate_rooks(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide);
void generate_bishops(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide);
void generate_queens(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide);
void generate_king(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide);

int white_attacks_square(Board* board, int square);
int black_attacks_square(Board* board, int square);

int set_game_from_FEN(Game* game, const char* FEN)
{
    int ret = set_from_FEN(&game->board, FEN);
    return ret;
}

MoveNode* generate_moves(Board* board)
{
	MoveNode* ret = (MoveNode*)malloc(sizeof(MoveNode));
	init_move_node(ret);

	if (board->sideToMove == WHITE)
	{
		generate_white_moves(board, ret);
	}
	else
	{
		generate_black_moves(board, ret);
	}

	return ret;
}

void make_move(Board* board, Move* move)
{
	bitboard fromBB = 1ull << move->from;
	bitboard toBB = 1ull << move->to;
	bitboard fromToBB = fromBB | toBB;

	board->pieces[move->piece] ^= fromToBB;
	if (move->piece < BLACK_PAWN)
	{
		board->sides[WHITE] ^= fromToBB;
		if (move->flags & CAPTURE_FLAG)
		{
			int i;
			for (i = BLACK_PAWN; i <= BLACK_KING; ++i)
			{
				if (board->pieces[i] & toBB)
				{
					board->pieces[i] &= ~toBB;
					board->sides[BLACK] &= ~toBB;
					board->occupied ^= toBB;
					move->capturedPiece = i;
					break;
				}
			}
		}
	}
	else
	{
		board->sides[BLACK] ^= fromToBB;
		if (move->flags & CAPTURE_FLAG)
		{
			int i;
			for (i = WHITE_PAWN; i < WHITE_KING; ++i)
			{
				if (board->pieces[i] & toBB)
				{
					board->pieces[i] &= ~toBB;
					board->sides[WHITE] &= ~toBB;
					board->occupied ^= toBB;
					move->capturedPiece = i;
					break;
				}
			}
		}
	}
	board->occupied ^= fromToBB;
	board->empty ^= fromToBB;

	board->sideToMove = 1 - board->sideToMove;
}

void unmake_move(Board* board, Move move)
{
	bitboard fromBB = 1ull << move.from;
	bitboard toBB = 1ull << move.to;
	bitboard fromToBB = fromBB | toBB;

	board->pieces[move.piece] ^= fromToBB;
	if (move.piece < BLACK_PAWN)
	{
		board->sides[WHITE] ^= fromToBB;
		if (move.flags & CAPTURE_FLAG)
		{
			board->pieces[move.capturedPiece] |= toBB;
			board->sides[BLACK] |= toBB;
			board->occupied ^= toBB;
		}
	}
	else
	{
		board->sides[BLACK] ^= fromToBB;
		if (move.flags & CAPTURE_FLAG)
		{
			board->pieces[move.capturedPiece] |= toBB;
			board->sides[WHITE] |= toBB;
			board->occupied ^= toBB;
		}

	}
	board->occupied ^= fromToBB;
	board->empty ^= fromToBB;

	board->sideToMove = 1 - board->sideToMove;
}

int perft(Game* game, int level)
{
    return board_perft(&game->board, &game->moves, level);
}

void divide(Game* game, int level)
{
	int i, totalMoves = 0;

    if (level <= 0)
    {
    	return;
    }

    if (!game->moves.children)
    {
		MoveNode* genMoves = generate_moves(&game->board);
		game->moves.children = genMoves->children;
		game->moves.numChildren = genMoves->numChildren;
		game->moves.maxChildren = genMoves->maxChildren;
		free(genMoves);
    }

    for (i = 0; i < game->moves.numChildren; ++i)
    {
    	int numMoves = 0;
    	char moveString[8];
    	sprint_move(moveString, game->moves.children[i].move);

    	make_move(&game->board, &game->moves.children[i].move);
    	numMoves = board_perft(&game->board, &game->moves.children[i], level - 1);
    	unmake_move(&game->board, game->moves.children[i].move);

    	totalMoves += numMoves;
    	printf("%s: %i\n", moveString, numMoves);
    }

    printf("\nMoves: %i\n", game->moves.numChildren);
    printf("Nodes: %i\n", totalMoves);
}

int board_perft(Board* board, MoveNode* moves, int level)
{
	int i;
	int perft = 0;

	if (!moves->children)
	{
		MoveNode* genMoves = generate_moves(board);
		moves->children = genMoves->children;
		moves->numChildren = genMoves->numChildren;
		moves->maxChildren = genMoves->maxChildren;
		free(genMoves);
	}

	if (level <= 0)
	{
		for (i = 0; i < moves->numChildren; ++i)
		{
			bitboard toBB = 1ull << moves->children[i].move.to;
			if (board->sideToMove == WHITE && 
				(board->pieces[BLACK_KING] & toBB))
			{
				return 0;
			}
			else if (board->sideToMove == BLACK &&
				(board->pieces[WHITE_KING] & toBB))
			{
				return 0;
			}
		}
		return 1;
	}

	if (!moves->children)
	{
		MoveNode* genMoves = generate_moves(board);
		moves->children = genMoves->children;
		moves->numChildren = genMoves->numChildren;
		moves->maxChildren = genMoves->maxChildren;
		free(genMoves);
	}

	for (i = 0; i < moves->numChildren; ++i)
	{
		make_move(board, &moves->children[i].move);
		perft += board_perft(board, &moves->children[i], level - 1);
		unmake_move(board, moves->children[i].move);
	}

	return perft;
}

void generate_white_moves(Board* board, MoveNode* movenode)
{
	bitboard kingSideMask = (1ull << F1 | 1ull << G1);
	bitboard queenSideMask = (1ull << B1 | 1ull << C1 | 1ull << D1);

	generate_white_pawns(board, movenode);
	generate_knights(board, movenode, WHITE_KNIGHT, WHITE, BLACK);
	generate_rooks(board, movenode, WHITE_ROOK, WHITE, BLACK);
	generate_bishops(board, movenode, WHITE_BISHOP, WHITE, BLACK);
	generate_queens(board, movenode, WHITE_QUEEN, WHITE, BLACK);
	generate_king(board, movenode, WHITE_KING, WHITE, BLACK);

	// Castling
	if (board->castling & (1ull << WHITE_KINGSIDE) &&
		(board->empty & kingSideMask) == kingSideMask &&
		!black_attacks_square(board, F1) && !black_attacks_square(board, G1))
	{
		Move move;
		move.from = E1;
		move.to = G1;
		move.flags = SPECIAL1_FLAG;
		move.piece = WHITE_KING;
		add_move(movenode, move);
	}
	if (board->castling & (1ull << WHITE_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!black_attacks_square(board, C1) && !black_attacks_square(board, D1))
	{
		Move move;
		move.from = E1;
		move.to = C1;
		move.flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
		move.piece = WHITE_KING;
		add_move(movenode, move);
	}
}

void generate_black_moves(Board* board, MoveNode* movenode)
{
	bitboard kingSideMask = (1ull << F8 | 1ull << G8);
	bitboard queenSideMask = (1ull << B8 | 1ull << C8 | 1ull << D8);

	generate_black_pawns(board, movenode);
	generate_knights(board, movenode, BLACK_KNIGHT, BLACK, WHITE);
	generate_rooks(board, movenode, BLACK_ROOK, BLACK, WHITE);
	generate_bishops(board, movenode, BLACK_BISHOP, BLACK, WHITE);
	generate_queens(board, movenode, BLACK_QUEEN, BLACK, WHITE);
	generate_king(board, movenode, BLACK_KING, BLACK, WHITE);

	// Castling
	if (board->castling & (1ull << BLACK_KINGSIDE) &&
		(board->empty & kingSideMask) == kingSideMask &&
		!white_attacks_square(board, F8) && !white_attacks_square(board, G8))
	{
		Move move;
		move.from = E8;
		move.to = G8;
		move.flags = SPECIAL1_FLAG;
		move.piece = BLACK_KING;
		add_move(movenode, move);
	}
	if (board->castling & (1ull << BLACK_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!white_attacks_square(board, C8) && !white_attacks_square(board, D8))
	{
		Move move;
		move.from = E8;
		move.to = C8;
		move.flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
		move.piece = BLACK_KING;
		add_move(movenode, move);
	}
}

void generate_white_pawns(Board* board, MoveNode* movenode)
{
	Move move;
	bitboard promotionRank = (1ull << A7) | (1ull << B7) | (1ull << C7) | (1ull << D7) |
							(1ull << E7) | (1ull << F7) | (1ull << G7) | (1ull << H7);
	bitboard startRank = (1ull << A2) | (1ull << B2) | (1ull << C2) | (1ull << D2) |
						(1ull << E2) | (1ull << F2) | (1ull << G2) | (1ull << H2);
	bitboard promoPawns = board->pieces[WHITE_PAWN] & promotionRank;
	bitboard nonPromoPawns = board->pieces[WHITE_PAWN] & ~promotionRank;

	bitboard singlepromo = (promoPawns << 8) & board->empty;
	bitboard singlepush = (nonPromoPawns << 8) & board->empty;
	bitboard doublepush = ((nonPromoPawns & startRank) << 8) & board->empty;
	doublepush = (doublepush << 8) & board->empty;

	move.flags = 0;
	move.piece = WHITE_PAWN;

	while(singlepush)
	{
		int pawnSquare = bit_scan_forward(singlepush);
		move.to = pawnSquare;
		move.from = pawnSquare - 8;
		add_move(movenode, move);
		singlepush = clear_lsb(singlepush);
	}

	move.flags = SPECIAL0_FLAG;

	while(doublepush)
	{
		int pawnSquare = bit_scan_forward(doublepush);
		move.to = pawnSquare;
		move.from = pawnSquare - 16;
		add_move(movenode, move);
		doublepush = clear_lsb(doublepush);
	}

	move.flags = CAPTURE_FLAG;

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		while(attacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(attacks);
			add_move(movenode, move);
			attacks = clear_lsb(attacks);
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int pawnSquare = bit_scan_forward(singlepromo);
		move.from = pawnSquare - 8;
		move.to = pawnSquare;
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG | SPECIAL0_FLAG;
		add_move(movenode, move);
		singlepromo = clear_lsb(singlepromo);
	}

	while(promoPawns)
	{
		int pawnSquare = bit_scan_forward(promoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		while(attacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(attacks);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG;
			add_move(movenode, move);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG;
			add_move(movenode, move);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG;
			add_move(movenode, move);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG | SPECIAL0_FLAG;
			add_move(movenode, move);
			attacks = clear_lsb(attacks);
		}
		promoPawns = clear_lsb(promoPawns);
	}
}

void generate_black_pawns(Board* board, MoveNode* movenode)
{
	Move move;
	bitboard promotionRank = (1ull << A2) | (1ull << B2) | (1ull << C2) | (1ull << D2) |
						(1ull << E2) | (1ull << F2) | (1ull << G2) | (1ull << H2);
	bitboard startRank = (1ull << A7) | (1ull << B7) | (1ull << C7) | (1ull << D7) |
						(1ull << E7) | (1ull << F7) | (1ull << G7) | (1ull << H7);
	bitboard promoPawns = board->pieces[BLACK_PAWN] & promotionRank;
	bitboard nonPromoPawns = board->pieces[BLACK_PAWN] & ~promotionRank;

	bitboard singlepromo = (promoPawns >> 8) & board->empty;
	bitboard singlepush = (nonPromoPawns >> 8) & board->empty;
	bitboard doublepush = ((nonPromoPawns & startRank) >> 8) & board->empty;
	doublepush = (doublepush >> 8) & board->empty;

	move.flags = 0;
	move.piece = BLACK_PAWN;

	while(singlepush)
	{
		int pawnSquare = bit_scan_forward(singlepush);
		move.to = pawnSquare;
		move.from = pawnSquare + 8;
		add_move(movenode, move);
		singlepush = clear_lsb(singlepush);
	}

	move.flags = SPECIAL0_FLAG;

	while(doublepush)
	{
		int pawnSquare = bit_scan_forward(doublepush);
		move.to = pawnSquare;
		move.from = pawnSquare + 16;
		add_move(movenode, move);
		doublepush = clear_lsb(doublepush);
	}

	move.flags = CAPTURE_FLAG;

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		while(attacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(attacks);
			add_move(movenode, move);
			attacks = clear_lsb(attacks);
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int pawnSquare = bit_scan_forward(singlepromo);
		move.from = pawnSquare + 8;
		move.to = pawnSquare;
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG | SPECIAL0_FLAG;
		add_move(movenode, move);
		singlepromo = clear_lsb(singlepromo);
	}

	while(promoPawns)
	{
		int pawnSquare = bit_scan_forward(promoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		while(attacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(attacks);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG;
			add_move(movenode, move);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL0_FLAG;
			add_move(movenode, move);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG;
			add_move(movenode, move);
			move.flags = PROMOTION_FLAG | CAPTURE_FLAG | SPECIAL1_FLAG | SPECIAL0_FLAG;
			add_move(movenode, move);
			attacks = clear_lsb(attacks);
		}
		promoPawns = clear_lsb(promoPawns);
	}
}

void generate_knights(Board* board, MoveNode* movenode,
					  int piece, int mySide, int theirSide)
{
	bitboard allKnights = board->pieces[piece];

	while(allKnights)
	{
		int knightSquare = bit_scan_forward(allKnights);
		bitboard allMoves = knightMoves[knightSquare] & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;
		while(allMoves)
		{
			Move move;

			move.from = knightSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = piece;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			Move move;

			move.from = knightSquare;
			move.to = bit_scan_forward(captures);
			move.flags = CAPTURE_FLAG;
			move.piece = piece;
			add_move(movenode, move);

			captures = clear_lsb(captures);
		}
		allKnights = clear_lsb(allKnights);
	}	
}

void generate_rooks(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	bitboard allRooks = board->pieces[piece];

	while(allRooks)
	{
		int rookSquare = bit_scan_forward(allRooks);
		bitboard allMoves = rook_attacks(board->occupied, rookSquare) & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;

		while(allMoves)
		{
			Move move;

			move.from = rookSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = piece;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			Move move;

			move.from = rookSquare;
			move.to = bit_scan_forward(captures);
			move.flags = CAPTURE_FLAG;
			move.piece = piece;
			add_move(movenode, move);

			captures = clear_lsb(captures);
		}		
		allRooks = clear_lsb(allRooks);
	}
}

void generate_bishops(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	bitboard allBishops = board->pieces[piece];

	while(allBishops)
	{
		int bishopSquare = bit_scan_forward(allBishops);
		bitboard allMoves = bishop_attacks(board->occupied, bishopSquare) & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;

		while(allMoves)
		{
			Move move;

			move.from = bishopSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = piece;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			Move move;

			move.from = bishopSquare;
			move.to = bit_scan_forward(captures);
			move.flags = CAPTURE_FLAG;
			move.piece = piece;
			add_move(movenode, move);

			captures = clear_lsb(captures);
		}
		allBishops = clear_lsb(allBishops);
	}
}

void generate_queens(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	bitboard allQueens = board->pieces[piece];

	while(allQueens)
	{
		int queenSquare = bit_scan_forward(allQueens);
		bitboard allMoves = queen_attacks(board->occupied, queenSquare) & ~board->sides[mySide];
		bitboard captures = allMoves & board->sides[theirSide];
		allMoves &= ~captures;

		while(allMoves)
		{
			Move move;

			move.from = queenSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = piece;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}
		while(captures)
		{
			Move move;

			move.from = queenSquare;
			move.to = bit_scan_forward(captures);
			move.flags = CAPTURE_FLAG;
			move.piece = piece;
			add_move(movenode, move);

			captures = clear_lsb(captures);
		}
		allQueens = clear_lsb(allQueens);
	}
}

void generate_king(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	int kingSquare = bit_scan_forward(board->pieces[piece]);
	bitboard allMoves = kingMoves[kingSquare] & ~board->sides[mySide];
	bitboard captures = allMoves & board->sides[theirSide];
	allMoves &= ~captures;

	Move move;
	move.from = kingSquare;
	move.flags = 0;
	move.piece = piece;

	while(allMoves)
	{
		move.to = bit_scan_forward(allMoves);
		add_move(movenode, move);
		allMoves = clear_lsb(allMoves);
	}
	move.flags = CAPTURE_FLAG;
	while(captures)
	{
		move.to = bit_scan_forward(captures);
		add_move(movenode, move);
		captures = clear_lsb(captures);
	}
}

int white_attacks_square(Board* board, int square)
{
	return (pawnAttacks[BLACK][square] & board->pieces[WHITE_PAWN] ||
		knightMoves[square] & board->pieces[WHITE_KNIGHT] ||
		bishop_attacks(board->occupied, square) & board->pieces[WHITE_BISHOP] ||
		rook_attacks(board->occupied, square) & board->pieces[WHITE_ROOK] ||
		queen_attacks(board->occupied, square) & board->pieces[WHITE_QUEEN]);
}

int black_attacks_square(Board* board, int square)
{
	return (pawnAttacks[WHITE][square] & board->pieces[BLACK_PAWN] ||
		knightMoves[square] & board->pieces[BLACK_KNIGHT] ||
		bishop_attacks(board->occupied, square) & board->pieces[BLACK_BISHOP] ||
		rook_attacks(board->occupied, square) & board->pieces[BLACK_ROOK] ||
		queen_attacks(board->occupied, square) & board->pieces[BLACK_QUEEN]);

}
