#ifndef EVALUATION_H
#define EVALUATION_H

struct Board;

// Normal evaluation function, using the static material score on the board
int eval_board_eval(struct Board* board);

// Redo the full evaluation, including the static material score kept on the board
int eval_full_eval(struct Board* board);

#endif

