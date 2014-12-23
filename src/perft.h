#ifndef PERFT_H
#define PERFT_H

#include <stdio.h>

struct EPDFile;
struct Board;

int perft_test_perft(struct EPDFile* epd);
unsigned int perft_perft(struct Board* board, unsigned int depth);
void perft_divide(FILE* stream, struct Board* board, unsigned int depth);

#endif

