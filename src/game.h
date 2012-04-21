#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "move.h"

typedef struct
{
    Board board;
    MoveNode moves;
} Game;

int set_game_from_FEN(Game* game, const char* FEN);

MoveNode* generate_moves(Board* board);
void make_move(Board* board, Move move);
void unmake_move(Board* board, Move move);

/* Perft debugging */
int perft(Game* game, int level);
void divide(Game* game, int level);

#endif