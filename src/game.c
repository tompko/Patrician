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

int board_perft(Board* board, int level);

int is_valid_pseudo_move(Board* board, Move move);
void filter_pseudo_moves(Board* board, MoveNode* movenode);

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

	filter_pseudo_moves(board, ret);

	return ret;
}

int perft(Board* board, int level)
{
    return board_perft(board, level);
}

void divide(Board* board, int level)
{
	int i, totalMoves = 0;
	MoveNode* genMoves;

    if (level <= 0)
    {
    	return;
    }

    genMoves = generate_moves(board);

    for (i = 0; i < genMoves->numChildren; ++i)
    {
    	int numMoves = 0;
    	char moveString[8];
    	sprint_move(moveString, genMoves->children[i].move);
    	printf("%s ", moveString);

    	make_move(board, &genMoves->children[i].move);
    	numMoves = board_perft(board, level - 1);
    	unmake_move(board, genMoves->children[i].move);

    	totalMoves += numMoves;
    	printf("%i\n", numMoves);
    }

    free_move_node(genMoves);
    free(genMoves);

    printf("\nMoves: %i\n", genMoves->numChildren);
    printf("Nodes: %i\n", totalMoves);
}

int board_perft(Board* board, int level)
{
	int i, nperft = 0;
	MoveNode* genMoves;

	genMoves = generate_moves(board);

	if (level == 1)
	{
		int ret = genMoves->numChildren;
		free_move_node(genMoves);
		free(genMoves);
		return ret;
	}

	for (i = 0; i < genMoves->numChildren; ++i)
	{
		make_move(board, &genMoves->children[i].move);
		nperft += board_perft(board, level - 1);
		unmake_move(board, genMoves->children[i].move);
	}

	free_move_node(genMoves);
	free(genMoves);

	return nperft;
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

void filter_pseudo_moves(Board* board, MoveNode* movenode)
{
	int i = 0, j = 0, numValidMoves = movenode->numChildren;

	while(i < numValidMoves)
	{
		if (is_valid_pseudo_move(board, movenode->children[i].move))
		{
			++i;
		}
		else
		{
			free_move_node(&movenode->children[i]);
			for (j = i + 1; j < numValidMoves; ++j)
			{
				movenode->children[j-1].move = movenode->children[j].move;
				movenode->children[j-1].children = movenode->children[j].children;
				movenode->children[j-1].numChildren = movenode->children[j].numChildren;
				movenode->children[j-1].maxChildren = movenode->children[j].maxChildren;
			}
			--numValidMoves;
		}
	}

	movenode->numChildren = numValidMoves;
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
		!black_attacks_square(board, E1) &&
		!black_attacks_square(board, F1) && !black_attacks_square(board, G1))
	{
		Move move;
		move.from = E1;
		move.to = G1;
		move.flags = SPECIAL1_FLAG;
		move.piece = WHITE_KING;
		move.side = WHITE;
		add_move(movenode, move);
	}
	if (board->castling & (1ull << WHITE_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!black_attacks_square(board, E1) &&
		!black_attacks_square(board, C1) && !black_attacks_square(board, D1))
	{
		Move move;
		move.from = E1;
		move.to = C1;
		move.flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
		move.piece = WHITE_KING;
		move.side = WHITE;
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
		!white_attacks_square(board, E8) &&
		!white_attacks_square(board, F8) && !white_attacks_square(board, G8))
	{
		Move move;
		move.from = E8;
		move.to = G8;
		move.flags = SPECIAL1_FLAG;
		move.piece = BLACK_KING;
		move.side = BLACK;
		add_move(movenode, move);
	}
	if (board->castling & (1ull << BLACK_QUEENSIDE) &&
		(board->empty & queenSideMask) == queenSideMask &&
		!white_attacks_square(board, E8) &&
		!white_attacks_square(board, C8) && !white_attacks_square(board, D8))
	{
		Move move;
		move.from = E8;
		move.to = C8;
		move.flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
		move.piece = BLACK_KING;
		move.side = BLACK;
		add_move(movenode, move);
	}
}

void generate_white_pawns(Board* board, MoveNode* movenode)
{
	Move move;
	bitboard promotionRank = ranks[6];
	bitboard epAttackRank = ranks[5];
	bitboard startRank = ranks[1];
	bitboard promoPawns = board->pieces[WHITE_PAWN] & promotionRank;
	bitboard nonPromoPawns = board->pieces[WHITE_PAWN] & ~promotionRank;

	bitboard singlepromo = (promoPawns << 8) & board->empty;
	bitboard singlepush = (nonPromoPawns << 8) & board->empty;
	bitboard doublepush = ((nonPromoPawns & startRank) << 8) & board->empty;
	doublepush = (doublepush << 8) & board->empty;

	move.flags = 0;
	move.piece = WHITE_PAWN;
	move.side = WHITE;

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

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[WHITE][pawnSquare] & board->sides[BLACK];
		bitboard epAttacks = pawnAttacks[WHITE][pawnSquare] & board->enPassant & epAttackRank;

		while(attacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(attacks);
			move.flags = CAPTURE_FLAG;
			add_move(movenode, move);
			attacks = clear_lsb(attacks);
		}
		while(epAttacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(epAttacks);
			move.flags = CAPTURE_FLAG | SPECIAL0_FLAG;
			add_move(movenode, move);
			epAttacks = clear_lsb(epAttacks);	
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int pawnSquare = bit_scan_forward(singlepromo);
		move.from = pawnSquare - 8;
		move.to = pawnSquare;
		move.flags = PROMOTION_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | SPECIAL0_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | SPECIAL1_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | SPECIAL1_FLAG | SPECIAL0_FLAG;
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
	bitboard promotionRank = ranks[1];
	bitboard epAttackRank = ranks[2];
	bitboard startRank = ranks[6];
	bitboard promoPawns = board->pieces[BLACK_PAWN] & promotionRank;
	bitboard nonPromoPawns = board->pieces[BLACK_PAWN] & ~promotionRank;

	bitboard singlepromo = (promoPawns >> 8) & board->empty;
	bitboard singlepush = (nonPromoPawns >> 8) & board->empty;
	bitboard doublepush = ((nonPromoPawns & startRank) >> 8) & board->empty;
	doublepush = (doublepush >> 8) & board->empty;

	move.flags = 0;
	move.piece = BLACK_PAWN;
	move.side = BLACK;

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

	while(nonPromoPawns)
	{
		int pawnSquare = bit_scan_forward(nonPromoPawns);
		bitboard attacks = pawnAttacks[BLACK][pawnSquare] & board->sides[WHITE];
		bitboard epAttacks = pawnAttacks[BLACK][pawnSquare] & board->enPassant & epAttackRank;
		while(attacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(attacks);
			move.flags = CAPTURE_FLAG;
			add_move(movenode, move);
			attacks = clear_lsb(attacks);
		}
		while(epAttacks)
		{
			move.from = pawnSquare;
			move.to = bit_scan_forward(epAttacks);
			move.flags = CAPTURE_FLAG | SPECIAL0_FLAG;
			add_move(movenode, move);
			epAttacks = clear_lsb(epAttacks);
		}
		nonPromoPawns = clear_lsb(nonPromoPawns);
	}

	while(singlepromo)
	{
		int pawnSquare = bit_scan_forward(singlepromo);
		move.from = pawnSquare + 8;
		move.to = pawnSquare;
		move.flags = PROMOTION_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | SPECIAL0_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | SPECIAL1_FLAG;
		add_move(movenode, move);
		move.flags = PROMOTION_FLAG | SPECIAL1_FLAG | SPECIAL0_FLAG;
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
			move.side = mySide;
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
			move.side = mySide;
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
			move.side = mySide;
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
			move.side = mySide;
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
			move.side = mySide;
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
			move.side = mySide;
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
			move.side = mySide;
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
			move.side = mySide;
			add_move(movenode, move);

			captures = clear_lsb(captures);
		}
		allQueens = clear_lsb(allQueens);
	}
}

void generate_king(Board* board, MoveNode* movenode,
					int piece, int mySide, int theirSide)
{
	Move move;
	int kingSquare = bit_scan_forward(board->pieces[piece]);
	bitboard allMoves = kingMoves[kingSquare] & ~board->sides[mySide];
	bitboard captures = allMoves & board->sides[theirSide];
	allMoves &= ~captures;

	move.from = kingSquare;
	move.flags = 0;
	move.piece = piece;
	move.side = mySide;

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
