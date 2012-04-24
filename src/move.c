#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "move.h"
#include "board.h"

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
        buf[i] = tolower(moveStr[i]);
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
