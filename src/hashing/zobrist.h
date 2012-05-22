#ifndef ZOBRIST_H
#define ZOBRIST_H

extern unsigned long long pieceKeys[12][64];
extern unsigned long long blackToMove;
extern unsigned long long castlingKeys[16];
extern unsigned long long enPassantKeys[8];

struct Board;

void init_zobrist_keys(void);
unsigned long long calculate_zobrist(struct Board * board);

#endif