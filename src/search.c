#include "search.h"

#include <malloc.h>

#include "game.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"

int alpha_beta(Board* board, int alpha, int beta, int depth);

Move root_search(Board* board)
{
	int i, maxScore;
	MoveNode* genMoves;
	Move move;

	genMoves = generate_moves(board);

	for (i = 0; i < genMoves->numChildren; ++i)
	{
		make_move(board, &genMoves->children[i].move);
		genMoves->children[i].score = alpha_beta(board, -1000000, 1000000, 1);
		unmake_move(board, genMoves->children[i].move);
	}

	maxScore = genMoves->children[0].score;
	move = genMoves->children[0].move;

	for (i = 1; i < genMoves->numChildren; ++i)
	{
		if (genMoves->children[i].score < maxScore)
		{
			maxScore = genMoves->children[i].score;
			move = genMoves->children[i].move;
		}
	}

	free(genMoves);

	return move;
}

int alpha_beta(Board* board, int alpha, int beta, int depth)
{
	int i;
	MoveNode * genMoves;

	if(depth == 0)
	{
		return evaluate(board);
	}

	genMoves = generate_moves(board);
		
	for (i = 0; i < genMoves->numChildren; ++i)
	{
		make_move(board, &genMoves->children[i].move);
		genMoves->children[i].score = -alpha_beta(board, -beta, -alpha, depth - 1 );
		unmake_move(board, genMoves->children[i].move);

		if( genMoves->children[i].score >= beta )
		{
			free(genMoves);
			return beta;   //  fail hard beta-cutoff
		}
		if( genMoves->children[i].score > alpha )
		{
			alpha = genMoves->children[i].score; // alpha acts like max in MiniMax
		}
	}

	free(genMoves);

	return alpha;
}
