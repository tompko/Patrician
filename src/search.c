#include "search.h"

#include <malloc.h>

#include "game.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"

int alpha_beta (Board * board, int alpha, int beta, int depth);

Move root_search (Board * board)
{
	int i;
	Move moves[256];
	Move bestMove;
	int bestScore = 1000000;

	unsigned int numMoves = generate_moves (board, moves);

	for (i = 0; i < numMoves; ++i)
	{
		make_move (board, &moves[i]);
		int score = alpha_beta (board, -1000000, 1000000, 1);
		unmake_move (board, moves[i]);

		if (score < bestScore)
		{
			bestScore = score;
			bestMove = moves[i];
		}
	}

	return bestMove;
}

int alpha_beta (Board * board, int alpha, int beta, int depth)
{
	int i;
	Move moves[256];

	if (depth == 0)
	{
		return evaluate (board);
	}

	unsigned int numMoves = generate_moves (board, moves);

	for (i = 0; i < numMoves; ++i)
	{
		make_move (board, &moves[i]);
		int score = -alpha_beta (board, -beta, -alpha, depth - 1);
		unmake_move (board, moves[i]);

		if (score >= beta)
		{
			return beta;		//  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score;	// alpha acts like max in MiniMax
		}
	}

	return alpha;
}

