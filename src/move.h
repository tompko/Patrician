#ifndef MOVE_H
#define MOVE_H

#include <assert.h>
#include <stdlib.h>

#include "board.h"

#define ONE_BIT_MASK (0x1)
#define FOUR_BIT_MASK (0xf)
#define SIX_BIT_MASK (0x3f)

#define MOVE_TO_SHIFT (0)
#define MOVE_FROM_SHIFT (6)
#define MOVE_SPECIAL0_FLAG_SHIFT (12)
#define MOVE_SPECIAL1_FLAG_SHIFT (13)
#define MOVE_PIECE_SHIFT (14)
#define MOVE_PROMOTION_FLAG_SHIFT (18)
#define MOVE_CAPTURE_PIECE_SHIFT (19)
#define MOVE_CAPTURE_FLAG_SHIFT (23)

// Moves have the following structure:
// capture: 1
// targetPiece: 4 - captured piece
// promotion: 1
// piece: 4 - stored as (NUM_PIECES - piece)
// flags: 2
// from: 6
// to: 6
// This is so that sorting as integers will sort "better" moves first
typedef unsigned long Move;

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

void make_move(struct Board* board, Move move);
void unmake_move(struct Board* board, Move move);

// capture: 1
// targetPiece: 4 - captured piece
// promotion: 1
// piece: 4 - stored as (NUM_PIECES - piece)
// flags: 2
// from: 6
// to: 6
static inline Move move_pack_move(unsigned int from,
                                  unsigned int to,
                                  unsigned int piece,
                                  unsigned int capturedPiece,
                                  unsigned int capture,
                                  unsigned int promotion,
                                  unsigned int special_0,
                                  unsigned int special_1)
{
	return (
		((to & SIX_BIT_MASK) << MOVE_TO_SHIFT) |
		((from & SIX_BIT_MASK) << MOVE_FROM_SHIFT) |
		(((NUM_PIECES - piece) & FOUR_BIT_MASK) << MOVE_PIECE_SHIFT) |
		((capturedPiece & FOUR_BIT_MASK) << MOVE_CAPTURE_PIECE_SHIFT) |
		((capture & ONE_BIT_MASK) << MOVE_CAPTURE_FLAG_SHIFT) |
		((promotion & ONE_BIT_MASK) << MOVE_PROMOTION_FLAG_SHIFT) |
		((special_0 & ONE_BIT_MASK) << MOVE_SPECIAL0_FLAG_SHIFT) |
		((special_1 & ONE_BIT_MASK) << MOVE_SPECIAL1_FLAG_SHIFT)
	);
}
static inline void move_unpack_move(Move move,
                                    unsigned int* from,
                                    unsigned int* to,
                                    unsigned int* piece,
                                    unsigned int* capturedPiece,
                                    unsigned int* capture,
                                    unsigned int* promotion,
                                    unsigned int* special_0,
                                    unsigned int* special_1)
{
	*to = (move >> MOVE_TO_SHIFT) & SIX_BIT_MASK;
	*from = (move >> MOVE_FROM_SHIFT) & SIX_BIT_MASK;
	*piece = NUM_PIECES - ((move >> MOVE_PIECE_SHIFT) & FOUR_BIT_MASK);
	*capturedPiece = (move >> MOVE_CAPTURE_PIECE_SHIFT) & FOUR_BIT_MASK;
	*capture = (move >> MOVE_CAPTURE_FLAG_SHIFT) & ONE_BIT_MASK;
	*promotion = (move >> MOVE_PROMOTION_FLAG_SHIFT) & ONE_BIT_MASK;
	*special_0 = (move >> MOVE_SPECIAL0_FLAG_SHIFT) & ONE_BIT_MASK;
	*special_1 = (move >> MOVE_SPECIAL1_FLAG_SHIFT) & ONE_BIT_MASK;
}

static inline unsigned int move_get_to(Move move)
{
	return (move >> MOVE_TO_SHIFT) & SIX_BIT_MASK;
}

static inline unsigned int move_get_from(Move move)
{
	return (move >> MOVE_FROM_SHIFT) & SIX_BIT_MASK;
}

static inline unsigned int move_get_piece(Move move)
{
	return NUM_PIECES - ((move >> MOVE_PIECE_SHIFT) & FOUR_BIT_MASK);
}

static inline unsigned int move_get_capturedPiece(Move move)
{
	return (move >> MOVE_CAPTURE_PIECE_SHIFT) & FOUR_BIT_MASK;
}

static inline unsigned int move_get_capture(Move move)
{
	return (move >> MOVE_CAPTURE_FLAG_SHIFT) & ONE_BIT_MASK;
}

static inline unsigned int move_get_promotion(Move move)
{
	return (move >> MOVE_PROMOTION_FLAG_SHIFT) & ONE_BIT_MASK;
}

static inline unsigned int move_get_special_0(Move move)
{
	return (move >> MOVE_SPECIAL0_FLAG_SHIFT) & ONE_BIT_MASK;
}

static inline unsigned int move_get_special_1(Move move)
{
	return (move >> MOVE_SPECIAL1_FLAG_SHIFT) & ONE_BIT_MASK;
}


#endif
