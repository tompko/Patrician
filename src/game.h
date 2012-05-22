#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "move.h"

MoveNode* generate_moves(Board* board);
void make_move(Board* board, Move* move);
void unmake_move(Board* board, Move move);

/* Perft debugging */
int perft(Board* board, int level);
void divide(Board* board, int level);

#endif
