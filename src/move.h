#ifndef MOVE_H
#define MOVE_H

typedef struct
{
    unsigned int to: 6;
    unsigned int from: 6;
    unsigned int piece: 4;
    unsigned int flags: 4;
    unsigned int capturedPiece: 4;
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

__inline void init_move_node(MoveNode* movenode)
{
	int i;

	movenode->children = (MoveNode*)malloc(sizeof(MoveNode)*DEFAULT_NUM_CHILDREN);
	movenode->numChildren = 0;
	movenode->maxChildren = DEFAULT_NUM_CHILDREN;
}

__inline void add_move(MoveNode* movenode, Move move)
{
	if (movenode->numChildren == movenode->maxChildren)
	{
		movenode->children = (MoveNode*)realloc(movenode->children, sizeof(MoveNode)*movenode->maxChildren*2);
		movenode->maxChildren *= 2;
	}

	movenode->children[movenode->numChildren].move = move;
	movenode->children[movenode->numChildren].children = NULL;
	movenode->children[movenode->numChildren].numChildren = 0;
	movenode->children[movenode->numChildren].maxChildren = 0;
	++movenode->numChildren;
}

#endif