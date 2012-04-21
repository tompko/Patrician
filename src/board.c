#include "board.h"

#include <string.h>
#include <stdlib.h>

char * strSquare[] = {
"A1","B1","C1","D1","E1","F1","G1","H1",
"A2","B2","C2","D2","E2","F2","G2","H2",
"A3","B3","C3","D3","E3","F3","G3","H3",
"A4","B4","C4","D4","E4","F4","G4","H4",
"A5","B5","C5","D5","E5","F5","G5","H5",
"A6","B6","C6","D6","E6","F6","G6","H6",
"A7","B7","C7","D7","E7","F7","G7","H7",
"A8","B8","C8","D8","E8","F8","G8","H8"
};

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

    board->white = board->pieces[WHITE_PAWN];
    board->white |= board->pieces[WHITE_KNIGHT];
    board->white |= board->pieces[WHITE_BISHOP];
    board->white |= board->pieces[WHITE_ROOK];
    board->white |= board->pieces[WHITE_QUEEN];
    board->white |= board->pieces[WHITE_KING];

    board->black = board->pieces[BLACK_PAWN];
    board->black |= board->pieces[BLACK_KNIGHT];
    board->black |= board->pieces[BLACK_BISHOP];
    board->black |= board->pieces[BLACK_ROOK];
    board->black |= board->pieces[BLACK_QUEEN];
    board->black |= board->pieces[BLACK_KING];

    board->occupied = board->white | board->black;
    board->empty = ~board->empty;

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
