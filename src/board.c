#include "board.h"

#include <string.h>
#include <stdlib.h>

int set_from_FEN(Board* board, const char* FEN)
{
	int rank = 7, file = 0;
    memset(board, 0, sizeof(Board));

	while(*FEN != ' ')
	{
		switch(*FEN)
		{
            case 'r':
            {
                board->pieces[BLACK_ROOK] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'n':
            {
                board->pieces[BLACK_KNIGHT] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'b':
            {
                board->pieces[BLACK_BISHOP] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'q':
            {
                board->pieces[BLACK_QUEEN] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'k':
            {
                board->pieces[BLACK_KING] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'p':
            {
                board->pieces[BLACK_PAWN] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'R':
            {
                board->pieces[WHITE_ROOK] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'N':
            {
                board->pieces[WHITE_KNIGHT] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'B':
            {
                board->pieces[WHITE_BISHOP] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'Q':
            {
                board->pieces[WHITE_QUEEN] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'K':
            {
                board->pieces[WHITE_KING] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case 'P':
            {
                board->pieces[WHITE_PAWN] |= 1ULL << (rank*8 + file);
                ++file;
                break;
            }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            {
                file += *FEN - '0';
                break;
            }
            case '/':
            {
                rank -= 1;
                file = 0;
                break;
            }
            default:
            {
                return 0;
            }
		}
        ++FEN;
	}
    ++FEN;

    if (*FEN == 'w')
    {
        board->whiteMove = 1;
    }
    else if (*FEN == 'b')
    {
        board->whiteMove = 0;
    }
    else
    {
        return 0;
    }

    FEN += 2;

    while(*FEN != ' ')
    {
        switch(*FEN)
        {
            case 'k':
            {
                board->castling |= 1u << BLACK_KINGSIDE;
                break;
            }
            case 'q':
            {
                board->castling |= 1u << BLACK_QUEENSIDE;
                break;
            }
            case 'K':
            {
                board->castling |= 1u << WHITE_KINGSIDE;
                break;
            }
            case 'Q':
            {
                board->castling |= 1u << WHITE_QUEENSIDE;
                break;
            }
            default:
            {
                return 1;
            }
        }
        ++FEN;
    }

    ++FEN;
    if(*FEN == '-')
    {
        ++FEN;
    }
    else
    {
        return 1;
    }
    ++FEN;

    board->halfmove = atoi(FEN);

    while(*FEN++ != ' ');

    board->move = atoi(FEN);

    return 1;
}
