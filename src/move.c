#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "move.h"
#include "board.h"
#include "bitscans.h"
#include "debug_log.h"

static int promotionPieces[] = {KNIGHT, BISHOP, ROOK, QUEEN};

Move make_move_from_str(Board* board, const char* moveStr)
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
    if (move.piece == WHITE_PAWN && 
        ((1ull << move.from) & ranks[1]) &&
         ((1ull << move.to) & ranks[3]))
    {
        move.flags |= SPECIAL0_FLAG;
    }
    if (move.piece == BLACK_PAWN &&
        ((1ull << move.from) & ranks[6]) &&
        ((1ull << move.to) & ranks[4]))
    {
        move.flags |= SPECIAL0_FLAG;
    }
    if (move.piece == WHITE_KING && move.from == E1)
    {
        if (move.to == G1)
        {
            move.flags = SPECIAL1_FLAG;
        }
        else if (move.to == C1)
        {
            move.flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
        }
    }
    if (move.piece == BLACK_KING && move.from == E8)
    {
        if (move.to == G8)
        {
            move.flags = SPECIAL1_FLAG;
        }
        else if (move.to == C8)
        {
            move.flags = SPECIAL1_FLAG | SPECIAL0_FLAG;
        }
    }
    move.side = move.piece % 2;
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
    sprintf(buffer, "%s%s", strSquare[move.from], strSquare[move.to]);
}

void log_move(Move move, const char* moveString, const char* moveType)
{
    char* moveBuffer = (char*)malloc(sizeof(char)*1024);
    char* flagBuffer = (char*)malloc(sizeof(char)*256);

    strcpy(flagBuffer, "[");
    if ((move.flags & PROMOTION_FLAG) == PROMOTION_FLAG)
    {
        strcat(flagBuffer, "\\\"promotion\\\",");
    }
    if ((move.flags & CAPTURE_FLAG) == CAPTURE_FLAG)
    {
        strcat(flagBuffer, "\\\"capture\\\",");
    }
    if ((move.flags & SPECIAL1_FLAG) == SPECIAL1_FLAG)
    {
        strcat(flagBuffer, "\\\"special1\\\",");
    }
    if ((move.flags & SPECIAL0_FLAG) == SPECIAL0_FLAG)
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
        "\\\"capturedPiece\\\": %i, \\\"side\\\": \\\"%s\\\", "
        "\\\"move_string\\\": \\\"%s\\\", \\\"move_type\\\": \\\"%s\\\"}", 
        move.to, move.from, move.piece, flagBuffer, move.capturedPiece,
        move.side == WHITE ? "white" : "black", moveString, moveType);
    LOG("move", moveBuffer);

    free(flagBuffer);
    free(moveBuffer);
}

void make_move(Board* board, Move* move)
{
    bitboard fromBB = 1ull << move->from;
    bitboard toBB = 1ull << move->to;
    bitboard fromToBB = fromBB | toBB;

    push_state(board);
    board->enPassant = 0;

    if (move->flags == 0)
    {
        board->pieces[move->piece] ^= fromToBB;
        board->sides[move->side] ^= fromToBB;
        board->occupied ^= fromToBB;
        board->empty ^= fromToBB;

        if (move->piece == WHITE_ROOK)
        {
            if (move->from == A1)
            {
                board->castling &= ~(1ull << WHITE_QUEENSIDE);
            }
            else if (move->from == H1)
            {
                board->castling &= ~(1ull << WHITE_KINGSIDE);
            }
        }
        else if (move->piece == BLACK_ROOK)
        {
            if (move->from == A8)
            {
                board->castling &= ~(1ull << BLACK_QUEENSIDE);
            }
            else if (move->from == H8)
            {
                board->castling &= ~(1ull << BLACK_KINGSIDE);
            }   
        }
        else if (move->piece == WHITE_KING)
        {
            board->castling &= (1ull << BLACK_KINGSIDE) | (1ull << BLACK_QUEENSIDE);
        }
        else if (move->piece == BLACK_KING)
        {
            board->castling &= (1ull << WHITE_KINGSIDE) | (1ull << WHITE_QUEENSIDE);
        }
    }
    else if ((move->flags & (PROMOTION_FLAG | CAPTURE_FLAG)) == (PROMOTION_FLAG | CAPTURE_FLAG))
    {
        int i;
        int promoPiece = promotionPieces[move->flags & 3] + move->side;

        board->pieces[move->piece] ^= fromBB;
        board->pieces[promoPiece] ^= toBB;
        board->sides[move->side] ^= fromToBB;
        board->sides[1-move->side] ^= toBB;

        for (i = 0; i < 6; ++i)
        {
            int pieceIndex = i*2 + (1-move->side);
            if (board->pieces[pieceIndex] & toBB)
            {
                board->pieces[pieceIndex] ^= toBB;
                move->capturedPiece = pieceIndex;
            }
        }

        board->occupied ^= fromBB;
        board->empty ^= fromBB;

        if (move->capturedPiece == WHITE_ROOK)
        {
            if (move->to == A1)
            {
                board->castling &= ~(1ull << WHITE_QUEENSIDE);
            }
            else if (move->to == H1)
            {
                board->castling &= ~(1ull << WHITE_KINGSIDE);
            }
        }
        else if (move->capturedPiece == BLACK_ROOK)
        {
            if (move->to == A8)
            {
                board->castling &= ~(1ull << BLACK_QUEENSIDE);
            }
            else if (move->to == H8)
            {
                board->castling &= ~(1ull << BLACK_KINGSIDE);
            }   
        }
    }
    else if (move->flags & PROMOTION_FLAG)
    {
        int promoPiece = promotionPieces[move->flags & 3] + move->side;

        board->pieces[move->piece] ^= fromBB;
        board->pieces[promoPiece] ^= toBB;
        board->sides[move->side] ^= fromToBB;
        board->occupied ^= fromToBB;
        board->empty ^= fromToBB;
    }
    else if ((move->flags & (CAPTURE_FLAG | SPECIAL0_FLAG)) == (CAPTURE_FLAG | SPECIAL0_FLAG))
    {
        bitboard captureSquare = ((toBB & ranks[2]) << 8) | ((toBB & ranks[5]) >> 8);

        board->pieces[move->piece] ^= fromToBB;
        board->sides[move->side] ^= fromToBB;
        board->sides[1 - move->side] ^= captureSquare;
        board->occupied ^= fromToBB | captureSquare;
        board->empty ^= fromToBB | captureSquare;
        move->capturedPiece = PAWN + (1 - move->side);
        board->pieces[move->capturedPiece] ^= captureSquare;
    }
    else if (move->flags & CAPTURE_FLAG)
    {
        int i;

        board->pieces[move->piece] ^= fromToBB;
        board->sides[move->side] ^= fromToBB;
        board->sides[1-move->side] ^= toBB;
        board->occupied ^= fromBB;
        board->empty ^= fromBB;
        for (i = 0; i < 6; ++i)
        {
            int pieceIndex = i*2 + (1-move->side);
            if (board->pieces[pieceIndex] & toBB)
            {
                board->pieces[pieceIndex] ^= toBB;
                move->capturedPiece = pieceIndex;
            }
        }

		if (move->piece == WHITE_ROOK)
		{
			if (move->from == A1)
			{
				board->castling &= ~(1ull << WHITE_QUEENSIDE);
			}
			else if (move->from == H1)
			{
				board->castling &= ~(1ull << WHITE_KINGSIDE);
			}
		}
		else if (move->piece == BLACK_ROOK)
		{
			if (move->from == A8)
			{
				board->castling &= ~(1ull << BLACK_QUEENSIDE);
			}
			else if (move->from == H8)
			{
				board->castling &= ~(1ull << BLACK_KINGSIDE);
			}   
		}
		else if (move->piece == WHITE_KING)
		{
			board->castling &= (1ull << BLACK_KINGSIDE) | (1ull << BLACK_QUEENSIDE);
		}
		else if (move->piece == BLACK_KING)
		{
			board->castling &= (1ull << WHITE_KINGSIDE) | (1ull << WHITE_QUEENSIDE);
		}

        if (move->capturedPiece == WHITE_ROOK)
        {
            if (move->to == A1)
            {
                board->castling &= ~(1ull << WHITE_QUEENSIDE);
            }
            else if (move->to == H1)
            {
                board->castling &= ~(1ull << WHITE_KINGSIDE);
            }
        }
        else if (move->capturedPiece == BLACK_ROOK)
        {
            if (move->to == A8)
            {
                board->castling &= ~(1ull << BLACK_QUEENSIDE);
            }
            else if (move->to == H8)
            {
                board->castling &= ~(1ull << BLACK_KINGSIDE);
            }   
        }
    }
    else if (move->flags & SPECIAL1_FLAG)
    {
        int rookIndex = ROOK + move->side;
        bitboard rookFromTo;
        if (move->flags & SPECIAL0_FLAG)
        {
            rookFromTo = ((1ull << A1) | (1ull << A8)) & backranks[move->side];
            rookFromTo |= rookFromTo << 3;
        }
        else
        {
            rookFromTo = ((1ull << H1) | (1ull << H8)) & backranks[move->side];
            rookFromTo |= rookFromTo >> 2;
        }
        board->pieces[move->piece] ^= fromToBB;
        board->pieces[rookIndex] ^= rookFromTo;
        board->sides[move->side] ^= fromToBB | rookFromTo;
        board->occupied ^= fromToBB | rookFromTo;
        board->empty ^= fromToBB | rookFromTo;

		if (move->side == WHITE)
		{
			board->castling &= (1ull << BLACK_KINGSIDE) | (1ull << BLACK_QUEENSIDE);
		}
		else
		{
			board->castling &= (1ull << WHITE_KINGSIDE) | (1ull << WHITE_QUEENSIDE);
		}
    }
    else if (move->flags & SPECIAL0_FLAG)
    {
        board->pieces[move->piece] ^= fromToBB;
        board->sides[move->side] ^= fromToBB;
        board->occupied ^= fromToBB;
        board->empty ^= fromToBB;
        board->enPassant = squareFiles[move->to];
    }

    board->sideToMove = 1 - board->sideToMove;
}

void unmake_move(Board* board, Move move)
{
    bitboard fromBB = 1ull << move.from;
    bitboard toBB = 1ull << move.to;
    bitboard fromToBB = fromBB | toBB;

    if (move.flags == 0)
    {
        board->pieces[move.piece] ^= fromToBB;
        board->sides[move.side] ^= fromToBB;
        board->occupied ^= fromToBB;
        board->empty ^= fromToBB;
    }
    else if ((move.flags & (PROMOTION_FLAG | CAPTURE_FLAG)) == (PROMOTION_FLAG | CAPTURE_FLAG))
    {
        int promoPiece = promotionPieces[move.flags & 3] + move.side;
        board->pieces[move.piece] ^= fromBB;
        board->pieces[promoPiece] ^= toBB;
        board->sides[move.side] ^= fromToBB;
        board->sides[1-move.side] ^= toBB;
        board->pieces[move.capturedPiece] ^= toBB;

        board->occupied ^= fromBB;
        board->empty ^= fromBB;
    }
    else if (move.flags & PROMOTION_FLAG)
    {
        int promoPiece = promotionPieces[move.flags & 3] + move.side;

        board->pieces[move.piece] ^= fromBB;
        board->pieces[promoPiece] ^= toBB;
        board->sides[move.side] ^= fromToBB;
        board->occupied ^= fromToBB;
        board->empty ^= fromToBB;
    }
    else if ((move.flags & (CAPTURE_FLAG | SPECIAL0_FLAG)) == (CAPTURE_FLAG | SPECIAL0_FLAG))
    {
        bitboard captureSquare = ((toBB & ranks[2]) << 8) | ((toBB & ranks[5]) >> 8);

        board->pieces[move.piece] ^= fromToBB;
        board->sides[move.side] ^= fromToBB;
        board->sides[1 - move.side] ^= captureSquare;
        board->occupied ^= fromToBB | captureSquare;
        board->empty ^= fromToBB | captureSquare;
        board->pieces[move.capturedPiece] ^= captureSquare;
    }
    else if (move.flags & CAPTURE_FLAG)
    {
        board->pieces[move.piece] ^= fromToBB;
        board->sides[move.side] ^= fromToBB;
        board->sides[1 - move.side] ^= toBB;
        board->occupied ^= fromBB;
        board->empty ^= fromBB;
        board->pieces[move.capturedPiece] ^= toBB;
    }
    else if (move.flags & SPECIAL1_FLAG)
    {
        int rookIndex = ROOK + move.side;
        bitboard rookFromTo;

        if (move.flags & SPECIAL0_FLAG)
        {
            rookFromTo = ((1ull << D1) | (1ull << D8)) & backranks[move.side];
            rookFromTo |= rookFromTo >> 3;
        }
        else
        {
            rookFromTo = ((1ull << F1) | (1ull << F8)) & backranks[move.side];
            rookFromTo |= rookFromTo << 2;
        }

        board->pieces[move.piece] ^= fromToBB;
        board->pieces[rookIndex] ^= rookFromTo;
        board->sides[move.side] ^= fromToBB | rookFromTo;
        board->occupied ^= fromToBB | rookFromTo;
        board->empty ^= fromToBB | rookFromTo;
    }
    else if (move.flags & SPECIAL0_FLAG)
    {
        board->pieces[move.piece] ^= fromToBB;
        board->sides[move.side] ^= fromToBB;
        board->occupied ^= fromToBB;
        board->empty ^= fromToBB;
    }

    pop_state(board);
    board->sideToMove = 1 - board->sideToMove;
}
