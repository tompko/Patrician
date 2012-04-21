#ifndef MOVE_H
#define MOVE_H

typedef struct
{
    unsigned int to: 6;
    unsigned int from: 6;
    unsigned int flags: 4;
    unsigned int capturedPiece: 4;
} Move;

#define PROMOTION_FLAG (0x8)
#define CAPTURE_FLAG (0x4)
#define SPECIAL1_FLAG (0x2)
#define SPECIAL0_FLAG (0x1)

#endif