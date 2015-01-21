#include "move.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "game.h"
#include "board.h"
#include "bitscans.h"
#include "hashing/zobrist.h"
#include "eval/piece_tables.h"
#include "debug_log.h"

static int promotionPieces[] = {KNIGHT, BISHOP, ROOK, QUEEN};
static char promotionPiecesStr[] = {'N', 'B', 'R', 'Q'};

Move make_move_from_str(Board* board, const char* moveStr)
{
	unsigned int to = 0;
	unsigned int from = 0;
	unsigned int piece = 0;
	unsigned int capturedPiece = 0;
	unsigned int capture = 0;
	unsigned int promotion = 0;
	unsigned int special_0 = 0;
	unsigned int special_1 = 0;

	from = tolower(moveStr[0]) - 'a';
	from += (tolower(moveStr[1]) - '1')*8;
	to = tolower(moveStr[2]) - 'a';
	to += (tolower(moveStr[3]) - '1')*8;

	piece = board->mailbox[from];
	if (board->mailbox[to] != NUM_PIECES)
	{
		capture = 1;
		capturedPiece = board->mailbox[to];
	}

	if (piece == WHITE_PAWN &&
		((1ull << from) & ranks[1]) &&
		 ((1ull << to) & ranks[3]))
	{
		special_0 = 1;
	}
	if (piece == BLACK_PAWN &&
		((1ull << from) & ranks[6]) &&
		((1ull << to) & ranks[4]))
	{
		special_0 = 1;
	}
	if ((piece == WHITE_PAWN && (1ull << to) & ranks[7])
		|| (piece == BLACK_PAWN && (1ull << to) & ranks[0]))
	{
		promotion = 1;
		switch(moveStr[4])
		{
			case 'q':
			case 'Q':
				special_0 = 1;
				special_1 = 1;
				break;
			case 'r':
			case 'R':
				special_1 = 1;
				break;
			case 'b':
			case 'B':
				special_0 = 1;
				break;
		}
	}
	if (piece == WHITE_KING && from == E1)
	{
		if (to == G1)
		{
			special_1 = 1;
		}
		else if (to == C1)
		{
			special_0 = 1;
			special_1 = 1;
		}
	}
	if (piece == BLACK_KING && from == E8)
	{
		if (to == G8)
		{
			special_1 = 1;
		}
		else if (to == C8)
		{
			special_0 = 1;
			special_1 = 1;
		}
	}
	return move_pack_move(to,
                    from,
                    piece,
                    capturedPiece,
                    capture,
                    promotion,
                    special_0,
                    special_1);
}

Move make_move_from_san(struct Board* board, const char* moveStr)
{
	Move move;
	Move moves[256];
	int destFile = -1, sourceFile = -1, destRank = -1, sourceRank = -1;
	int to;
	int numMoves;
	int i;
	const char *sanMove = moveStr;
	unsigned int promotion = 0, special_0 = 0, special_1 = 0;

	while(*moveStr)
	{
		switch (*moveStr)
		{
			case 'N':
				promotion = 1;
				break;
			case 'B':
				promotion = 1;
				special_0 = 1;
				break;
			case 'R':
				promotion = 1;
				special_1 = 1;
				break;
			case 'Q':
				promotion = 1;
				special_0 = 1;
				special_1 = 1;
				break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
				if (destFile != -1)
				{
					sourceFile = destFile;
				}
				destFile = *moveStr - 'a';
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				if (destRank != -1)
				{
					sourceRank = destRank;
				}
				destRank = *moveStr - '1';
				break;
		}
		if (*moveStr == ' ')
		{
			break;
		}
		++moveStr;
	}

	to = (destRank * 8) + destFile;

	numMoves = generate_moves(board, moves);

	for (i = 0; i < numMoves; ++i)
	{
		if (move_get_to(moves[i]) != to)
		{
			continue;
		}

		if (move_get_promotion(moves[i]) &&
		    (!promotion ||
		     (move_get_promotion(moves[i]) != promotion) ||
		     (move_get_special_0(moves[i]) != special_0) ||
		     (move_get_special_1(moves[i]) != special_1)))
		{
			continue;
		}

		if ((sourceFile != -1) &&
		    (sourceFile != move_get_from(moves[i]) % 8))
		{
			continue;
		}

		if ((sourceRank != -1) &&
			(sourceRank != move_get_from(moves[i]) / 8))
		{
			continue;
		}

		return moves[i];
	}

	fprintf(stderr, "Error, unable to parse SAN move %s\n", sanMove);
	memset(&move, 0, sizeof(Move));
	return move;
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
		buf[i] = (char)tolower(moveStr[i]);
	}

	if (('a' > buf[0]) || ('h' < buf[0]) || ('a' > buf[2]) || ('h' < buf[2]) ||
		('1' > buf[1]) || ('8' < buf[1]) || ('1' > buf[3]) || ('8' < buf[3]))
	{
		return 0;
	}

	return 1;
}

void sprint_move(char* buffer, Move move)
{
	unsigned int to;
	unsigned int from;
	unsigned int piece;
	unsigned int capturedPiece;
	unsigned int capture;
	unsigned int promotion;
	unsigned int special_0;
	unsigned int special_1;

	move_unpack_move(move, &to, &from, &piece, &capturedPiece, &capture, &promotion, &special_0, &special_1);

	if (promotion)
	{
		char promoPiece = promotionPiecesStr[(special_0 << 1) | special_1];
		sprintf(buffer, "%s%s%c", strSquare[from], strSquare[to], promoPiece);
	}
	else
	{
		sprintf(buffer, "%s%s", strSquare[from], strSquare[to]);
	}
}

void log_move(Move move, const char* moveString, const char* moveType)
{
	unsigned int to;
	unsigned int from;
	unsigned int piece;
	unsigned int capturedPiece;
	unsigned int capture;
	unsigned int promotion;
	unsigned int special_0;
	unsigned int special_1;
	char* moveBuffer = (char*)malloc(sizeof(char)*1024);
	char* flagBuffer = (char*)malloc(sizeof(char)*256);

	move_unpack_move(move, &to, &from, &piece, &capturedPiece, &capture, &promotion, &special_0, &special_1);
	strcpy(flagBuffer, "[");
	if (promotion)
	{
		strcat(flagBuffer, "\\\"promotion\\\",");
	}
	if (capture)
	{
		strcat(flagBuffer, "\\\"capture\\\",");
	}
	if (special_1)
	{
		strcat(flagBuffer, "\\\"special1\\\",");
	}
	if (special_0)
	{
		strcat(flagBuffer, "\\\"special0\\\",");
	}
	if (flagBuffer[strlen(flagBuffer) - 1] == ',')
	{
		flagBuffer[strlen(flagBuffer) - 1] = 0;
	}
	strcat(flagBuffer, "]");

	sprintf(moveBuffer, "{\\\"to\\\": %i, \\\"from\\\": %i, "
		"\\\"piece\\\": %i, \\\"flags\\\": %s, "
		"\\\"capturedPiece\\\": %i, "
		"\\\"move_string\\\": \\\"%s\\\", \\\"move_type\\\": \\\"%s\\\"}", 
		to, from, piece, flagBuffer, capturedPiece,
		moveString, moveType);
	LOG("move", moveBuffer);

	free(flagBuffer);
	free(moveBuffer);
}

void make_move(Board* board, Move move)
{
	unsigned int to;
	unsigned int from;
	unsigned int piece;
	unsigned int capturedPiece;
	unsigned int capture;
	unsigned int promotion;
	unsigned int special_0;
	unsigned int special_1;
	bitboard fromBB, toBB, fromToBB;

	move_unpack_move(move, &from, &to, &piece, &capturedPiece, &capture, &promotion, &special_0, &special_1);

	fromBB = 1ull << from;
	toBB = 1ull << to;
	fromToBB = fromBB | toBB;

	assert(board->mailbox[from] == piece);
	assert((board->mailbox[to] == NUM_PIECES) ||
	       ((board->mailbox[to] == capturedPiece) &&
	        capture));

	push_state(board);
	board->zobrist ^= castlingKeys[board->castling];
	if (board->enPassant)
	{
		board->zobrist ^= enPassantKeys[bit_scan_forward(board->enPassant)];
	}
	board->enPassant = 0;
	board->mailbox[from] = NUM_PIECES;
	board->mailbox[to] = piece;

	board->staticScore -= pieceSquareValues[piece][from];
	board->staticScore += pieceSquareValues[piece][to];

	if (promotion && capture)
	{
		int promoPiece = promotionPieces[(special_0 << 1) | special_1] + board->sideToMove;

		board->pieces[piece] ^= fromBB;
		board->pieces[promoPiece] ^= toBB;
		board->pieces[capturedPiece] ^= toBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->sides[1 - board->sideToMove] ^= toBB;
		board->mailbox[to] = promoPiece;

		board->occupied ^= fromBB;
		board->empty ^= fromBB;

		if (capturedPiece == WHITE_ROOK)
		{
			if (to == A1)
			{
				board->castling &= ~(1ull << WHITE_QUEENSIDE);
			}
			else if (to == H1)
			{
				board->castling &= ~(1ull << WHITE_KINGSIDE);
			}
		}
		else if (capturedPiece == BLACK_ROOK)
		{
			if (to == A8)
			{
				board->castling &= ~(1ull << BLACK_QUEENSIDE);
			}
			else if (to == H8)
			{
				board->castling &= ~(1ull << BLACK_KINGSIDE);
			}   
		}

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[capturedPiece][to];
		board->zobrist ^= pieceKeys[promoPiece][to];

		board->staticScore -= pieceValues[piece];
		board->staticScore -= pieceSquareValues[piece][to];
		board->staticScore += pieceValues[promoPiece];
		board->staticScore += pieceSquareValues[promoPiece][to];
		board->staticScore += pieceValues[capturedPiece];
		board->staticScore += pieceSquareValues[capturedPiece][to];
	}
	else if (promotion)
	{
		int promoPiece = promotionPieces[(special_0 << 1) | special_1] + board->sideToMove;

		board->pieces[piece] ^= fromBB;
		board->pieces[promoPiece] ^= toBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->occupied ^= fromToBB;
		board->empty ^= fromToBB;
		board->mailbox[to] = promoPiece;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[promoPiece][to];

		board->staticScore -= pieceValues[piece];
		board->staticScore += pieceValues[promoPiece];
		board->staticScore -= pieceSquareValues[piece][to];
		board->staticScore += pieceSquareValues[promoPiece][to];
	}
	else if (capture && special_0)
	{
		bitboard captureBB = ((toBB & ranks[2]) << 8) | ((toBB & ranks[5]) >> 8);
		int captureSquare = bit_scan_forward(captureBB);

		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->sides[1 - board->sideToMove] ^= captureBB;
		board->occupied ^= fromToBB | captureBB;
		board->empty ^= fromToBB | captureBB;
		board->pieces[capturedPiece] ^= captureBB;
		board->mailbox[captureSquare] = NUM_PIECES;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= pieceKeys[capturedPiece][captureSquare];

		board->staticScore += pieceValues[capturedPiece];
		board->staticScore += pieceSquareValues[capturedPiece][captureSquare];
	}
	else if (capture)
	{
		board->pieces[piece] ^= fromToBB;
		board->pieces[capturedPiece] ^= toBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->sides[1 - board->sideToMove] ^= toBB;
		board->occupied ^= fromBB;
		board->empty ^= fromBB;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= pieceKeys[capturedPiece][to];

		board->staticScore += pieceValues[capturedPiece];
		board->staticScore += pieceSquareValues[capturedPiece][to];

		if (piece == WHITE_ROOK)
		{
			if (from == A1)
			{
				board->castling &= ~(1ull << WHITE_QUEENSIDE);
			}
			else if (from == H1)
			{
				board->castling &= ~(1ull << WHITE_KINGSIDE);
			}
		}
		else if (piece == BLACK_ROOK)
		{
			if (from == A8)
			{
				board->castling &= ~(1ull << BLACK_QUEENSIDE);
			}
			else if (from == H8)
			{
				board->castling &= ~(1ull << BLACK_KINGSIDE);
			}   
		}
		else if (piece == WHITE_KING)
		{
			board->castling &= (1ull << BLACK_KINGSIDE) | (1ull << BLACK_QUEENSIDE);
		}
		else if (piece == BLACK_KING)
		{
			board->castling &= (1ull << WHITE_KINGSIDE) | (1ull << WHITE_QUEENSIDE);
		}

		if (capturedPiece == WHITE_ROOK)
		{
			if (to == A1)
			{
				board->castling &= ~(1ull << WHITE_QUEENSIDE);
			}
			else if (to == H1)
			{
				board->castling &= ~(1ull << WHITE_KINGSIDE);
			}
		}
		else if (capturedPiece == BLACK_ROOK)
		{
			if (to == A8)
			{
				board->castling &= ~(1ull << BLACK_QUEENSIDE);
			}
			else if (to == H8)
			{
				board->castling &= ~(1ull << BLACK_KINGSIDE);
			}   
		}
	}
	else if (special_1)
	{
		int rookIndex = ROOK +  board->sideToMove;
		int rookFrom, rookTo;
		bitboard rookFromTo;

		if (special_0)
		{
			rookFromTo = ((1ull << A1) | (1ull << A8)) & backranks[board->sideToMove];
			rookFrom = bit_scan_forward(rookFromTo);
			rookTo = rookFrom + 3;
			rookFromTo |= rookFromTo << 3;
		}
		else
		{
			rookFromTo = ((1ull << H1) | (1ull << H8)) & backranks[board->sideToMove];
			rookFrom = bit_scan_forward(rookFromTo);
			rookTo = rookFrom - 2;
			rookFromTo |= rookFromTo >> 2;
		}

		board->pieces[piece] ^= fromToBB;
		board->pieces[rookIndex] ^= rookFromTo;
		board->sides[board->sideToMove] ^= fromToBB | rookFromTo;
		board->occupied ^= fromToBB | rookFromTo;
		board->empty ^= fromToBB | rookFromTo;
		board->mailbox[rookFrom] = NUM_PIECES;
		board->mailbox[rookTo] = rookIndex;

		if (board->sideToMove == WHITE)
		{
			board->castling &= (1ull << BLACK_KINGSIDE) | (1ull << BLACK_QUEENSIDE);
		}
		else
		{
			board->castling &= (1ull << WHITE_KINGSIDE) | (1ull << WHITE_QUEENSIDE);
		}

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= pieceKeys[rookIndex][bit_scan_forward(rookFromTo)];
		rookFromTo = clear_lsb(rookFromTo);
		board->zobrist ^= pieceKeys[rookIndex][bit_scan_forward(rookFromTo)];

		board->staticScore -= pieceSquareValues[rookIndex][rookFrom];
		board->staticScore += pieceSquareValues[rookIndex][rookTo];
	}
	else if (special_0)
	{
		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->occupied ^= fromToBB;
		board->empty ^= fromToBB;
		board->enPassant = squareFiles[to];

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= enPassantKeys[bit_scan_forward(board->enPassant)];
	}
	else
	{
		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->occupied ^= fromToBB;
		board->empty ^= fromToBB;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];

		if (piece == WHITE_ROOK)
		{
			if (from == A1)
			{
				board->castling &= ~(1ull << WHITE_QUEENSIDE);
			}
			else if (from == H1)
			{
				board->castling &= ~(1ull << WHITE_KINGSIDE);
			}
		}
		else if (piece == BLACK_ROOK)
		{
			if (from == A8)
			{
				board->castling &= ~(1ull << BLACK_QUEENSIDE);
			}
			else if (from == H8)
			{
				board->castling &= ~(1ull << BLACK_KINGSIDE);
			}
		}
		else if (piece == WHITE_KING)
		{
			board->castling &= (1ull << BLACK_KINGSIDE) | (1ull << BLACK_QUEENSIDE);
		}
		else if (piece == BLACK_KING)
		{
			board->castling &= (1ull << WHITE_KINGSIDE) | (1ull << WHITE_QUEENSIDE);
		}
	}

	board->sideToMove = 1 - board->sideToMove;
	board->zobrist ^= blackToMoveKey;
	board->zobrist ^= castlingKeys[board->castling];
	board->staticScore = -board->staticScore;

	verify_board(board);
}

void unmake_move(Board* board, Move move)
{
	unsigned int to;
	unsigned int from;
	unsigned int piece;
	unsigned int capturedPiece;
	unsigned int capture;
	unsigned int promotion;
	unsigned int special_0;
	unsigned int special_1;
	bitboard fromBB, toBB, fromToBB;

	move_unpack_move(move, &from, &to, &piece, &capturedPiece, &capture, &promotion, &special_0, &special_1);

	fromBB = 1ull << from;
	toBB = 1ull << to;
	fromToBB = fromBB | toBB;

	board->sideToMove = 1 - board->sideToMove;

	assert(board->mailbox[from] == NUM_PIECES);
	assert((board->mailbox[to] == piece) ||
	       ((board->mailbox[to] == promotionPieces[(special_0 << 1) | special_1] + board->sideToMove) && promotion));

	board->mailbox[from] = piece;
	board->mailbox[to] = NUM_PIECES;
	board->staticScore = -board->staticScore;

	board->staticScore -= pieceSquareValues[piece][to];
	board->staticScore += pieceSquareValues[piece][from];


	if (promotion && capture)
	{
		int promoPiece = promotionPieces[(special_0 << 1) | special_1] + board->sideToMove;

		board->pieces[piece] ^= fromBB;
		board->pieces[promoPiece] ^= toBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->sides[1 - board->sideToMove] ^= toBB;
		board->pieces[capturedPiece] ^= toBB;
		board->mailbox[to] = capturedPiece;

		board->occupied ^= fromBB;
		board->empty ^= fromBB;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[capturedPiece][to];
		board->zobrist ^= pieceKeys[promoPiece][to];

		board->staticScore += pieceValues[piece];
		board->staticScore += pieceSquareValues[piece][to];
		board->staticScore -= pieceValues[promoPiece];
		board->staticScore -= pieceSquareValues[promoPiece][to];
		board->staticScore -= pieceValues[capturedPiece];
		board->staticScore -= pieceSquareValues[capturedPiece][to];
	}
	else if (promotion)
	{
		int promoPiece = promotionPieces[(special_0 << 1) | special_1] + board->sideToMove;

		board->pieces[piece] ^= fromBB;
		board->pieces[promoPiece] ^= toBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->occupied ^= fromToBB;
		board->empty ^= fromToBB;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[promoPiece][to];

		board->staticScore += pieceValues[piece];
		board->staticScore -= pieceValues[promoPiece];
		board->staticScore += pieceSquareValues[piece][to];
		board->staticScore -= pieceSquareValues[promoPiece][to];
	}
	else if (capture && special_0)
	{
		bitboard captureBB = ((toBB & ranks[2]) << 8) | ((toBB & ranks[5]) >> 8);
		int captureSquare = bit_scan_forward(captureBB);

		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->sides[1 - board->sideToMove] ^= captureBB;
		board->occupied ^= fromToBB | captureBB;
		board->empty ^= fromToBB | captureBB;
		board->pieces[capturedPiece] ^= captureBB;
		board->mailbox[captureSquare] = capturedPiece;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= pieceKeys[capturedPiece][captureSquare];

		board->staticScore -= pieceValues[capturedPiece];
		board->staticScore -= pieceSquareValues[capturedPiece][captureSquare];
	}
	else if (capture)
	{
		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->sides[1 - board->sideToMove] ^= toBB;
		board->occupied ^= fromBB;
		board->empty ^= fromBB;
		board->pieces[capturedPiece] ^= toBB;
		board->mailbox[to] = capturedPiece;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= pieceKeys[capturedPiece][to];

		board->staticScore -= pieceValues[capturedPiece];
		board->staticScore -= pieceSquareValues[capturedPiece][to];
	}
	else if (special_1)
	{
		int rookIndex = ROOK + board->sideToMove;
		int rookFrom, rookTo;
		bitboard rookFromTo;

		if (special_0)
		{
			rookFromTo = ((1ull << D1) | (1ull << D8)) & backranks[board->sideToMove];
			rookTo = bit_scan_forward(rookFromTo);
			rookFrom = rookTo - 3;
			rookFromTo |= rookFromTo >> 3;
		}
		else
		{
			rookFromTo = ((1ull << F1) | (1ull << F8)) & backranks[board->sideToMove];
			rookTo = bit_scan_forward(rookFromTo);
			rookFrom = rookTo + 2;
			rookFromTo |= rookFromTo << 2;
		}

		board->pieces[piece] ^= fromToBB;
		board->pieces[rookIndex] ^= rookFromTo;
		board->sides[board->sideToMove] ^= fromToBB | rookFromTo;
		board->occupied ^= fromToBB | rookFromTo;
		board->empty ^= fromToBB | rookFromTo;
		board->mailbox[rookTo] = NUM_PIECES;
		board->mailbox[rookFrom] = rookIndex;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
		board->zobrist ^= pieceKeys[rookIndex][bit_scan_forward(rookFromTo)];
		rookFromTo = clear_lsb(rookFromTo);
		board->zobrist ^= pieceKeys[rookIndex][bit_scan_forward(rookFromTo)];

		board->staticScore += pieceSquareValues[rookIndex][rookFrom];
		board->staticScore -= pieceSquareValues[rookIndex][rookTo];
	}
	else if (special_0)
	{
		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->occupied ^= fromToBB;
		board->empty ^= fromToBB;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
	}
	else
	{
		board->pieces[piece] ^= fromToBB;
		board->sides[board->sideToMove] ^= fromToBB;
		board->occupied ^= fromToBB;
		board->empty ^= fromToBB;

		board->zobrist ^= pieceKeys[piece][from];
		board->zobrist ^= pieceKeys[piece][to];
	}

	board->zobrist ^= castlingKeys[board->castling];
	if (board->enPassant)
	{
		board->zobrist ^= enPassantKeys[bit_scan_forward(board->enPassant)];
	}
	pop_state(board);
	board->zobrist ^= castlingKeys[board->castling];
	if (board->enPassant)
	{
		board->zobrist ^= enPassantKeys[bit_scan_forward(board->enPassant)];
	}
	board->zobrist ^= blackToMoveKey;

	assert(board->mailbox[from] == piece);
	assert((board->mailbox[to] == NUM_PIECES) ||
	       ((board->mailbox[to] == capturedPiece) &&
	        capture));

	verify_board(board);
}

