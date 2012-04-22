#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "game.h"
#include "bitscans.h"

#include "moves/knight_moves.h"
#include "moves/ray_attacks.h"
#include "moves/king_moves.h"

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

Move make_move_str(Board* board, const char* moveStr)
{
	Move move;
	bitboard fromBB, toBB;
	int fromPiece = -1;
	int toPiece = -1;
	int i;

	move.from = tolower(moveStr[0]) - 'a';
	move.from += (tolower(moveStr[1]) - '1')*8;
	move.to = tolower(moveStr[2]) - 'a';
	move.to += (tolower(moveStr[3]) - '1')*8;
	fromBB = 1ull << move.from;
	toBB = 1ull << move.to;

	for (i = 0; i < NUM_PIECES; ++i)
	{
		if (board->pieces[i] & fromBB)
		{
			fromPiece = i;
		}
		if (board->pieces[i] & toBB)
		{
			toPiece = i;
		}
	}

	move.piece = fromPiece;
	move.flags =0;
	if (toPiece >= 0)
	{
		move.capturedPiece = toPiece;
		move.flags |= CAPTURE_FLAG;
	}

	make_move(board, &move);

	return move;
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

int is_move(const char* moveStr)
{
	char buf[4];
	int i;

	if (strlen(moveStr) != 4)
	{
		return 0;
	}

	for (i = 0; i < 4; ++i)
	{
		buf[i] = tolower(moveStr[i]);
	}

	if ('a' > buf[0] || 'h' < buf[0])
	{
		return 0;
	}
	if ('a' > buf[2] || 'h' < buf[2])
	{
		return 0;
	}
	if ('1' > buf[1] || '8' < buf[1])
	{
		return 0;
	}
	if ('1' > buf[3] || '8' < buf[3])
	{
		return 0;
	}

	return 1;
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
	int totalMoves = 0;

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

    	make_move(&game->board, &game->moves.children[i].move);
    	numMoves = board_perft(&game->board, &game->moves.children[i], level - 1);
    	unmake_move(&game->board, game->moves.children[i].move);

    	sprint_move(moveString, game->moves.children[i].move);
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
	generate_white_pawns(board, movenode);
	generate_knights(board, movenode, WHITE_KNIGHT, WHITE, BLACK);
	generate_rooks(board, movenode, WHITE_ROOK, WHITE, BLACK);
	generate_bishops(board, movenode, WHITE_BISHOP, WHITE, BLACK);
	generate_queens(board, movenode, WHITE_QUEEN, WHITE, BLACK);
	generate_king(board, movenode, WHITE_KING, WHITE, BLACK);
}

void generate_black_moves(Board* board, MoveNode* movenode)
{
	generate_black_pawns(board, movenode);
	generate_knights(board, movenode, BLACK_KNIGHT, BLACK, WHITE);
	generate_rooks(board, movenode, BLACK_ROOK, BLACK, WHITE);
	generate_bishops(board, movenode, BLACK_BISHOP, BLACK, WHITE);
	generate_queens(board, movenode, BLACK_QUEEN, BLACK, WHITE);
	generate_king(board, movenode, BLACK_KING, BLACK, WHITE);
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

void generate_black_pawns(Board* board, MoveNode* movenode)
{
	bitboard allPawns = board->pieces[BLACK_PAWN];

	while(allPawns)
	{
		Move move;
		int pawnSquare = bit_scan_forward(allPawns);
		bitboard pawnMask = lsb(allPawns);

		move.from = pawnSquare;
		move.flags = 0;
		move.piece = BLACK_PAWN;

		if (!(board->occupied & (pawnMask >> 8)))
		{
			move.to = pawnSquare - 8;
			add_move(movenode, move);
		}

		if ((48 <= pawnSquare) && (pawnSquare < 56) &&
			!(board->occupied & (pawnMask >> 8)) &&
			!(board->occupied & (pawnMask >> 16)))
		{
			move.to = pawnSquare - 16;
			add_move(movenode, move);
		}

		if ((pawnMask & 0x7f7f7f7f7f7f7f7f) &&
			(board->sides[WHITE] & (pawnMask >> 7)))
		{
			move.to = pawnSquare - 7;
			move.flags = CAPTURE_FLAG;
			add_move(movenode, move);
		}

		if ((pawnMask & 0xfefefefefefefefe) &&
			(board->sides[WHITE] & (pawnMask >> 9)))
		{
			move.to = pawnSquare - 9;
			move.flags = CAPTURE_FLAG;
			add_move(movenode, move);
		}

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

void generate_rooks(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	bitboard allRooks = board->pieces[piece];

	while(allRooks)
	{
		int rookSquare = bit_scan_forward(allRooks);
		bitboard allMoves = rook_attacks(board->occupied, rookSquare);
		allMoves &= ~board->sides[mySide];

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
		bitboard allMoves = bishop_attacks(board->occupied, bishopSquare);
		allMoves &= ~board->sides[mySide];

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
		bitboard allMoves = queen_attacks(board->occupied, queenSquare);
		allMoves &= ~board->sides[mySide];

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
		allQueens = clear_lsb(allQueens);
	}
}

void generate_king(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	int kingSquare = bit_scan_forward(board->pieces[piece]);
	bitboard allMoves = kingMoves[kingSquare] & ~board->sides[mySide];
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
}
