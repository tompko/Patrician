#include "evaluation.h"

#include "bitscans.h"
#include "board.h"
#include "eval/piece_tables.h"

int evaluate_piece(bitboard pieces, int* squareValues, int pieceValue);

int eval_board_eval(Board* board)
{
	return board->staticScore;
}

int eval_full_eval(Board* board)
{
	int score = 0;
	score += evaluate_piece(board->pieces[WHITE_PAWN],
	                        pieceSquareValues[WHITE_PAWN],
	                        pieceValues[WHITE_PAWN]);
	score += evaluate_piece(board->pieces[WHITE_KNIGHT],
	                        pieceSquareValues[WHITE_KNIGHT],
	                        pieceValues[WHITE_KNIGHT]);
	score += evaluate_piece(board->pieces[WHITE_BISHOP],
	                        pieceSquareValues[WHITE_BISHOP],
	                        pieceValues[WHITE_BISHOP]);
	score += evaluate_piece(board->pieces[WHITE_ROOK],
	                        pieceSquareValues[WHITE_ROOK],
	                        pieceValues[WHITE_ROOK]);
	score += evaluate_piece(board->pieces[WHITE_QUEEN],
	                        pieceSquareValues[WHITE_QUEEN],
	                        pieceValues[WHITE_QUEEN]);
	score += evaluate_piece(board->pieces[WHITE_KING],
	                        pieceSquareValues[WHITE_KING],
	                        pieceValues[WHITE_KING]);

	score -= evaluate_piece(board->pieces[BLACK_KING],
	                        pieceSquareValues[BLACK_KING],
	                        pieceValues[BLACK_KING]);
	score -= evaluate_piece(board->pieces[BLACK_QUEEN],
	                        pieceSquareValues[BLACK_QUEEN],
	                        pieceValues[BLACK_QUEEN]);
	score -= evaluate_piece(board->pieces[BLACK_ROOK],
	                        pieceSquareValues[BLACK_ROOK],
	                        pieceValues[BLACK_ROOK]);
	score -= evaluate_piece(board->pieces[BLACK_BISHOP],
	                        pieceSquareValues[BLACK_BISHOP],
	                        pieceValues[BLACK_BISHOP]);
	score -= evaluate_piece(board->pieces[BLACK_KNIGHT],
	                        pieceSquareValues[BLACK_KNIGHT],
	                        pieceValues[BLACK_KNIGHT]);
	score -= evaluate_piece(board->pieces[BLACK_PAWN],
	                        pieceSquareValues[BLACK_PAWN],
	                        pieceValues[BLACK_PAWN]);

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
