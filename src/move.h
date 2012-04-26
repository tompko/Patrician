#ifndef MOVE_H
#define MOVE_H

#include "defines.h"
#include <assert.h>

typedef struct
{
    unsigned int to: 6;
    unsigned int from: 6;
    unsigned int piece: 4;
    unsigned int flags: 4;
    unsigned int capturedPiece: 4;
    unsigned int side: 1;
} Move;

typedef struct MoveNode
{
	Move move;
	struct MoveNode * children;
	int numChildren;
	int maxChildren;
} MoveNode;

#define PROMOTION_FLAG (0x8)
#define CAPTURE_FLAG (0x4)
#define SPECIAL1_FLAG (0x2)
#define SPECIAL0_FLAG (0x1)

#define DEFAULT_NUM_CHILDREN (30)

struct Board;

Move make_move_from_str(struct Board* board, const char* moveStr);
int is_move(const char* moveStr);
void sprint_move(char* buffer, Move move);

void make_move(struct Board* board, Move* move);
void unmake_move(struct Board* board, Move move);

INLINE void init_move_node(MoveNode* movenode)
{
    int i = 0;

	movenode->children = (MoveNode*)malloc(sizeof(MoveNode)*DEFAULT_NUM_CHILDREN);
	movenode->numChildren = 0;
	movenode->maxChildren = DEFAULT_NUM_CHILDREN;

    for (i = 0; i < DEFAULT_NUM_CHILDREN; ++i)
    {
        movenode->children[i].children = NULL;
        movenode->children[i].numChildren = 0;
        movenode->children[i].maxChildren = 0;
    }
}

INLINE void free_move_node(MoveNode* movenode)
{
    int i;
    for (i = 0; i < movenode->numChildren; ++i)
    {
        free_move_node(&movenode->children[i]);
    }

    free(movenode->children);
    movenode->children = NULL;
    movenode->numChildren = 0;
    movenode->maxChildren = 0;
}

INLINE void add_move(MoveNode* movenode, Move move)
{
	if (movenode->numChildren == movenode->maxChildren)
	{
        int i, start;
		movenode->children = (MoveNode*)realloc(movenode->children, sizeof(MoveNode)*movenode->maxChildren*2);
        start = movenode->maxChildren;
		movenode->maxChildren *= 2;

        for (i = start; i < movenode->maxChildren; ++i)
        {
            movenode->children[i].children = NULL;
            movenode->children[i].numChildren = 0;
            movenode->children[i].maxChildren = 0;
        }
	}

	movenode->children[movenode->numChildren].move = move;
	movenode->children[movenode->numChildren].children = NULL;
	movenode->children[movenode->numChildren].numChildren = 0;
	movenode->children[movenode->numChildren].maxChildren = 0;
	++movenode->numChildren;
}

#endif