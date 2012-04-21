#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "move.h"

typedef struct
{
    Board board;
    Move* moves;
    unsigned int numMoves;
    unsigned int maxMoves;
} Game;

int set_game_from_FEN(Game* game, const char* FEN);

/* Perft debugging */
int perft(Game* game, int level);
void divide(Game* game, int level);

#endif