#include "search.h"

#include <malloc.h>

#include "game.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"

int alpha_beta(Board* board, MoveNode* movenode, int alpha, int beta, int depth);

Move root_search(Game* game)
{
	int i, maxScore;
	MoveNode* genMoves;
	Move move;

	free_move_node(&game->moves);
	
	genMoves = generate_moves(&game->board);
	game->moves.children = genMoves->children;
	game->moves.numChildren = genMoves->numChildren;
	game->moves.maxChildren = genMoves->maxChildren;
	free(genMoves);

	for (i = 0; i < game->moves.numChildren; ++i)
	{
		make_move(&game->board, &game->moves.children[i].move);
		game->moves.children[i].score = alpha_beta(&game->board, &game->moves.children[i], -1000000, 1000000, 7);
		unmake_move(&game->board, game->moves.children[i].move);
	}

	maxScore = game->moves.children[0].score;
	move = game->moves.children[0].move;

	for (i = 1; i < game->moves.numChildren; ++i)
	{
		if (game->moves.children[i].score > maxScore)
		{
			maxScore = game->moves.children[i].score;
			move = game->moves.children[i].move;
		}
	}

	return move;
}

int alpha_beta(Board* board, MoveNode* movenode, int alpha, int beta, int depth)
{
	int i;

	if(depth == 0)
	{
		return evaluate(board);
	}

	if (!movenode->children)
	{
		MoveNode * genMoves = generate_moves(board);
		movenode->children = genMoves->children;
		movenode->numChildren = genMoves->numChildren;
		movenode->maxChildren = genMoves->maxChildren;
		free(genMoves);
	}
		
	for (i = 0; i < movenode->numChildren; ++i)
	{
		make_move(board, &movenode->children[i].move);
		movenode->children[i].score = -alpha_beta(board, &movenode->children[i], -beta, -alpha, depth - 1 );
		unmake_move(board, movenode->children[i].move);

		if( movenode->children[i].score >= beta )
		{
			return beta;   //  fail hard beta-cutoff
		}
		if( movenode->children[i].score > alpha )
		{
			alpha = movenode->children[i].score; // alpha acts like max in MiniMax
		}
	}

	return alpha;
}
