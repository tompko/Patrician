#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "bitscans.h"

#include "moves/knight_moves.h"
#include "moves/ray_attacks.h"
#include "moves/king_moves.h"

int board_perft(Board* board, MoveNode* moves, int level);

void generate_white_moves(Board* board, MoveNode* movenode);
void generate_black_moves(Board* board, MoveNode* movenode);

void generate_white_pawns(Board* board, MoveNode* movenode);
void generate_white_rooks(Board* board, MoveNode* movenode);
void generate_white_bishops(Board* board, MoveNode* movenode);
void generate_white_queens(Board* board, MoveNode* movenode);
void generate_white_king(Board* board, MoveNode* movenode);

void generate_black_pawns(Board* board, MoveNode* movenode);

void generate_knights(Board* board, MoveNode* movenode,
					  int piece, int mySide, int theirSide);
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

void make_move(Board* board, Move move)
{
	bitboard fromBB = 1ull << move.from;
	bitboard toBB = 1ull << move.to;
	bitboard fromToBB = fromBB | toBB;

	board->pieces[move.piece] ^= fromToBB;
	if (move.piece < BLACK_PAWN)
	{
		board->sides[WHITE] ^= fromToBB;
	}
	else
	{
		board->sides[BLACK] ^= fromToBB;
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
	}
	else
	{
		board->sides[BLACK] ^= fromToBB;
	}
	board->occupied ^= fromToBB;
	board->empty ^= fromToBB;

	board->sideToMove = 1 - board->sideToMove;
}

void sprint_move(char* buffer, Move move)
{
	sprintf(buffer, "%s%s", strSquare[move.from], strSquare[move.to]);
}

int perft(Game* game, int level)
{
    return board_perft(&game->board, &game->moves, level);
}

void divide(Game* game, int level)
{
	int i;

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

    	make_move(&game->board, game->moves.children[i].move);
    	numMoves = board_perft(&game->board, &game->moves.children[i], level - 1);
    	unmake_move(&game->board, game->moves.children[i].move);

    	sprint_move(moveString, game->moves.children[i].move);
    	printf("%s: %i\n", moveString, numMoves);
    }
}

int board_perft(Board* board, MoveNode* moves, int level)
{
	int i;
	int perft = 0;

	if (level <= 0)
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
	generate_knights(board, movenode, WHITE_KNIGHT, WHITE, BLACK);
	generate_white_rooks(board, movenode);
	generate_white_bishops(board, movenode);
	generate_white_queens(board, movenode);
	generate_white_king(board, movenode);
}

void generate_black_moves(Board* board, MoveNode* movenode)
{
	generate_black_pawns(board, movenode);
	generate_knights(board, movenode, BLACK_KNIGHT, BLACK, WHITE);
}

void generate_white_pawns(Board* board, MoveNode* movenode)
{
	bitboard allPawns = board->pieces[WHITE_PAWN];

	while(allPawns)
	{
		Move move;
		int pawnSquare = bit_scan_forward(allPawns);
		bitboard pawnMask = lsb(allPawns);

		move.from = pawnSquare;
		move.flags = 0;
		move.piece = WHITE_PAWN;

		if (!(board->occupied & (pawnMask << 8)))
		{
			move.to = pawnSquare + 8;
			add_move(movenode, move);
		}

		if ((8 <= pawnSquare) && (pawnSquare < 16) &&
			!(board->occupied & (pawnMask << 8)) &&
			!(board->occupied & (pawnMask << 16)))
		{
			move.to = pawnSquare + 16;
			add_move(movenode, move);
		}

		if ((pawnMask & 0xfefefefefefefefe) &&
			(board->sides[BLACK] & (pawnMask << 7)))
		{
			move.to = pawnSquare + 7;
			move.flags = CAPTURE_FLAG;
			add_move(movenode, move);
		}

		if ((pawnMask & 0x7f7f7f7f7f7f7f7f) &&
			(board->sides[BLACK] & (pawnMask << 9)))
		{
			move.to = pawnSquare + 9;
			move.flags = CAPTURE_FLAG;
			add_move(movenode, move);
		}

		allPawns = clear_lsb(allPawns);
	}
}

void generate_white_rooks(Board* board, MoveNode* movenode)
{
	bitboard allRooks = board->pieces[WHITE_ROOK];

	while(allRooks)
	{
		int rookSquare = bit_scan_forward(allRooks);
		bitboard allMoves = rook_attacks(board->occupied, rookSquare);
		allMoves &= ~board->sides[WHITE];

		while(allMoves)
		{
			Move move;

			move.from = rookSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = WHITE_ROOK;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}

		allRooks = clear_lsb(allRooks);
	}
}

void generate_white_bishops(Board* board, MoveNode* movenode)
{
	bitboard allBishops = board->pieces[WHITE_BISHOP];

	while(allBishops)
	{
		int bishopSquare = bit_scan_forward(allBishops);
		bitboard allMoves = bishop_attacks(board->occupied, bishopSquare);
		allMoves &= ~board->sides[WHITE];

		while(allMoves)
		{
			Move move;

			move.from = bishopSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = WHITE_BISHOP;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}

		allBishops = clear_lsb(allBishops);
	}
}

void generate_white_queens(Board* board, MoveNode* movenode)
{
	bitboard allQueens = board->pieces[WHITE_QUEEN];

	while(allQueens)
	{
		int queenSquare = bit_scan_forward(allQueens);
		bitboard allMoves = queen_attacks(board->occupied, queenSquare);
		allMoves &= ~board->sides[WHITE];

		while(allMoves)
		{
			Move move;

			move.from = queenSquare;
			move.to = bit_scan_forward(allMoves);
			move.flags = 0;
			move.piece = WHITE_QUEEN;
			add_move(movenode, move);

			allMoves = clear_lsb(allMoves);
		}

		allQueens = clear_lsb(allQueens);
	}
}

void generate_white_king(Board* board, MoveNode* movenode)
{
	int kingSquare = bit_scan_forward(board->pieces[WHITE_KING]);
	bitboard allMoves = kingMoves[kingSquare] & ~board->sides[WHITE];
	Move move;
	move.from = kingSquare;
	move.flags = 0;
	move.piece = WHITE_KING;

	while(allMoves)
	{
		move.to = bit_scan_forward(allMoves);
		add_move(movenode, move);
		allMoves = clear_lsb(allMoves);
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

void generate_knights(Board* board, MoveNode* movenode,
					  int piece, int mySide, int theirSide)
{
	bitboard allKnights = board->pieces[piece];

	while(allKnights)
	{
		int knightSquare = bit_scan_forward(allKnights);
		bitboard allMoves = knightMoves[knightSquare] & ~board->sides[mySide];

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
		allKnights = clear_lsb(allKnights);
	}	
}
