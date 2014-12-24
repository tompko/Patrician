#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "move.h"

unsigned int generate_moves(Board* board, Move* moves);
void make_move(Board* board, Move* move);
void unmake_move(Board* board, Move move);

int white_attacks_square(Board* board, int square);
int black_attacks_square(Board* board, int square);

#endif
