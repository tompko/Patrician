#include <stdlib.h>

#include "game.h"
#include "bitscans.h"

#include "moves\knight_moves.h"

int board_perft(Board* board, MoveNode* moves, int level);

void generate_white_moves(Board* board, MoveNode* movenode);
void generate_black_moves(Board* board, MoveNode* movenode);

void generate_white_pawns(Board* board, MoveNode* movenode);
void generate_white_knights(Board* board, MoveNode* movenode);

void generate_black_pawns(Board* board, MoveNode* movenode);
void generate_black_knights(Board* board, MoveNode* movenode);

int set_game_from_FEN(Game* game, const char* FEN)
{
    int ret = set_from_FEN(&game->board, FEN);
    game->moves.children = NULL;

    return ret;
}

MoveNode* generate_moves(Board* board)
{
	MoveNode* ret = (MoveNode*)malloc(sizeof(MoveNode));

	init_move_node(ret);

	if (board->whiteMove)
	{
		generate_white_moves(board, ret);
	}
	else
	{
		generate_black_moves(board, ret);
	}

	return ret;
}

void make_move(Board* board, Move move)
{
	bitboard fromBB = 1ull << move.from;
	bitboard toBB = 1ull << move.to;
	bitboard fromToBB = fromBB | toBB;

	board->pieces[move.piece] ^= fromToBB;
	if (move.piece < BLACK_PAWN)
	{
		board->white ^= fromToBB;
	}
	else
	{
		board->black ^= fromToBB;
	}
	board->occupied ^= fromToBB;
	board->empty ^= fromToBB;

	board->whiteMove = 1 - board->whiteMove;
}

void unmake_move(Board* board, Move move)
{
	bitboard fromBB = 1ull << move.from;
	bitboard toBB = 1ull << move.to;
	bitboard fromToBB = fromBB | toBB;

	board->pieces[move.piece] ^= fromToBB;
	if (move.piece < BLACK_PAWN)
	{
		board->white ^= fromToBB;
	}
	else
	{
		board->black ^= fromToBB;
	}
	board->occupied ^= fromToBB;
	board->empty ^= fromToBB;

	board->whiteMove = 1 - board->whiteMove;
}

int perft(Game* game, int level)
{
    return board_perft(&game->board, &game->moves, level);
}

void divide(Game* game, int level)
{
    
}

int board_perft(Board* board, MoveNode* moves, int level)
{
	int i;
	int perft = 0;

	if (level == 0)
	{
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
		make_move(board, moves->children[i].move);
		perft += board_perft(board, &moves->children[i], level - 1);
		unmake_move(board, moves->children[i].move);
	}

	return perft;
}

void generate_white_moves(Board* board, MoveNode* movenode)
{
	generate_white_pawns(board, movenode);
	generate_white_knights(board, movenode);
}

void generate_black_moves(Board* board, MoveNode* movenode)
{
	generate_black_pawns(board, movenode);
	generate_black_knights(board, movenode);
}

void generate_white_pawns(Board* board, MoveNode* movenode)
{
	bitboard allPawns = board->pieces[WHITE_PAWN];

	while(allPawns)
	{
		Move move;

		int pawnSquare = bit_scan_forward(allPawns);

		move.from = pawnSquare;
		move.to = pawnSquare + 8;
		move.flags = 0;
		move.piece = WHITE_PAWN;
		add_move(movenode, move);

		move.to = pawnSquare + 16;
		add_move(movenode, move);

		allPawns = clear_lsb(allPawns);
	}
}

void generate_white_knights(Board* board, MoveNode* movenode)
{
	bitboard allKnights = board->pieces[WHITE_KNIGHT];

	while(allKnights)
	{
		int knightSquare = bit_scan_forward(allKnights);
		bitboard allMoves = knight_moves[knightSquare] & ~board->white;

		while(allMoves)
		{
			Move move;

			move.from = knightSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = WHITE_KNIGHT;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}

		allKnights = clear_lsb(allKnights);
	}
}


void generate_black_pawns(Board* board, MoveNode* movenode)
{
	bitboard allPawns = board->pieces[BLACK_PAWN];
	
	while(allPawns)
	{
		Move move;

		int pawnSquare = bit_scan_forward(allPawns);

		move.from = pawnSquare;
		move.to = pawnSquare - 8;
		move.flags = 0;
		move.piece = BLACK_PAWN;
		add_move(movenode, move);

		move.to = pawnSquare - 16;
		add_move(movenode, move);

		allPawns = clear_lsb(allPawns);
	}
}

void generate_black_knights(Board* board, MoveNode* movenode)
{
	bitboard allKnights = board->pieces[BLACK_KNIGHT];

	while(allKnights)
	{
		int knightSquare = bit_scan_forward(allKnights);
		bitboard allMoves = knight_moves[knightSquare] & ~board->black;

		while(allMoves)
		{
			Move move;

			move.from = knightSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = BLACK_KNIGHT;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}

		allKnights = clear_lsb(allKnights);
	}
}
