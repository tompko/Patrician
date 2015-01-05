#ifndef MOVE_H
#define MOVE_H

#include <assert.h>
#include <stdlib.h>

typedef struct Move
{
	unsigned int to: 6;
	unsigned int from: 6;
	unsigned int piece: 4;
	unsigned int flags: 4;
	unsigned int capturedPiece: 4;
	unsigned int side: 1;
} Move;

#define PROMOTION_FLAG (0x8)
#define CAPTURE_FLAG (0x4)
#define SPECIAL1_FLAG (0x2)
#define SPECIAL0_FLAG (0x1)

//The combination of the above flags denote special moves
// P  C  S1  S0
// 0  0  0   0   Quiet move
// 0  0  0   1   Pawn double push
// 0  0  1   0   Castle Kingside
// 0  0  1   1   Castle Queenside
// 0  1  0   0   Capture
// 0  1  0   1   Capture en passant
// 0  1  1   0   <not used>
// 0  1  1   1   <not used>
// 1  0  0   0   Pawn promotion to Knight
// 1  0  0   1   Pawn promotion to Bishop
// 1  0  1   0   Pawn promotion to Rook
// 1  0  1   1   Pawn promotion to Queen
// 1  1  0   0   Pawn promotion with capture to Knight
// 1  1  0   1   Pawn promotion with capture to Bishop
// 1  1  1   0   Pawn promotion with capture to Rook
// 1  1  1   1   Pawn promotion with capture to Queen

struct Board;

Move make_move_from_str(struct Board* board, const char* moveStr);
Move make_move_from_san(struct Board* board, const char* moveStr);
int is_move(const char* moveStr);
void sprint_move(char* buffer, Move move);
void log_move(Move move, const char* moveString, const char* moveType);

void make_move(struct Board* board, Move* move);
void unmake_move(struct Board* board, Move move);

#endif
