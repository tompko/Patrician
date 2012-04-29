#include "evaluation.h"

#include "bitscans.h"
#include "board.h"
#include "eval/pieceTables.h"

int evaluate_piece(bitboard pieces, int* squareValues, int pieceValue);

int evaluate(Board* board)
{
	int score = 0;
	score += evaluate_piece(board->pieces[WHITE_PAWN], whitePawnValues, PAWN_VALUE);
	score += evaluate_piece(board->pieces[WHITE_KNIGHT], knightValues, KNIGHT_VALUE);
	score += evaluate_piece(board->pieces[WHITE_BISHOP], bishopValues, BISHOP_VALUE);
	score += evaluate_piece(board->pieces[WHITE_ROOK], rookValues, ROOK_VALUE);
	score += evaluate_piece(board->pieces[WHITE_QUEEN], queenValues, QUEEN_VALUE);
	score += evaluate_piece(board->pieces[WHITE_KING], whiteKingMiddleValues, KING_VALUE);

	score -= evaluate_piece(board->pieces[BLACK_PAWN], blackPawnValues, PAWN_VALUE);
	score -= evaluate_piece(board->pieces[BLACK_KNIGHT], knightValues, KNIGHT_VALUE);
	score -= evaluate_piece(board->pieces[BLACK_BISHOP], bishopValues, BISHOP_VALUE);
	score -= evaluate_piece(board->pieces[BLACK_ROOK], rookValues, ROOK_VALUE);
	score -= evaluate_piece(board->pieces[BLACK_QUEEN], queenValues, QUEEN_VALUE);
	score -= evaluate_piece(board->pieces[BLACK_KING], blackKingMiddleValues, KING_VALUE);

	if(board->sideToMove == BLACK)
	{
		return -score;
	}
	return score;
}

int evaluate_piece(bitboard pieces, int* squareValues, int pieceValue)
{
	int score = 0;

	while(pieces)
	{
		int pieceSquare = bit_scan_forward(pieces);

		score += squareValues[pieceSquare] + pieceValue;

		pieces = clear_lsb(pieces);
	}

	return score;
}
