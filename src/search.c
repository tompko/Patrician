#include "search.h"

#include <malloc.h>

#include "bitscans.h"
#include "game.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "game_time.h"
#include "io/epd.h"
#include "hashing/transposition_table.h"

#define INFINITY (10000000) // highest possible score, used as initial alpha, beta values

static int nodes;

int alpha_beta(Board * board, int alpha, int beta, int depth);

static inline int force_exact(int x)
{
	return (x + 1) & ~3;
}

static inline int force_lb(int x)
{
	return force_exact(x) + 1;
}

Move root_search(Board * board)
{
	int i, depth;
	Move moves[256];
	Move bestMove;
	int bestScore = -INFINITY;
	Timer searchTimer;
	char moveStr[16];

	nodes = 0;
	start_timer(&searchTimer);

	unsigned int numMoves = generate_moves(board, moves);

	for (depth = 1; depth <= 5; ++depth)
	{
		int score = -INFINITY;
		bestScore = -INFINITY;

		for (i = 0; i < numMoves; ++i)
		{
			make_move(board, moves[i]);
			if ((board->sideToMove == BLACK &&
				!black_attacks_square(board, bit_scan_forward(board->pieces[WHITE_KING]))) ||
				(board->sideToMove == WHITE &&
				!white_attacks_square(board, bit_scan_forward(board->pieces[BLACK_KING]))))
			{
				score = -alpha_beta(board, bestScore, INFINITY, depth);

				if (score > bestScore)
				{
					double elapsed = get_elapsed_time(&searchTimer);
					int centi_seconds = (int)(elapsed * 100);
					bestScore = score;
					bestMove = moves[i];
					sprint_move(moveStr, bestMove);
					// ply score time(centiseconds) nodes (selective depth) (speed) (tbhits) \t pv
					printf("%i %i %i %i\t%s\n", depth, score / 4, centi_seconds, nodes, moveStr);
				}
			}
			unmake_move(board, moves[i]);
		}
	}

	return bestMove;
}

void search_test_search(EPDFile* epdFile)
{
	int i = 0;
	int hits = 0;

	printf("Testing %i positions\n", epdFile->numEPD);
	clear_transposition_table();

	for (i = 0; i < epdFile->numEPD; ++i)
	{
		EPD* epd = &epdFile->epds[i];

		printf("%i/%i %s\n", i + 1, epdFile->numEPD, epd->description);
		if (epd_has_operation(epd, "id"))
		{
			printf("id: %s\n", epd_string_operation(epd, "id"));
		}

		if (epd_has_operation(epd, "bm"))
		{
			// bm - Best Move
			// Pass if we found the same move
			Move engineMove = root_search(&epd->board);
			Move bestMove = epd_move_operation(epd, "bm");
			char engineMoveStr[16], bestMoveStr[16];

			sprint_move(engineMoveStr, engineMove);
			sprint_move(bestMoveStr, bestMove);

			int sameMove = ((engineMove.from == bestMove.from) &&
			                (engineMove.to == bestMove.to));
			printf("Engine Move: %s\nBest Move: %s\n%s\n", engineMoveStr, bestMoveStr,
			sameMove ? "PASS" : "FAIL");

			if (sameMove)
			{
				++hits;
			}
		}
		else if (epd_has_operation(epd, "am"))
		{
			// am - Avoid move
			// Pass if we found a different move
			Move engineMove = root_search(&epd->board);
			Move avoidMove = epd_move_operation(epd, "am");
			char engineMoveStr[16], avoidMoveStr[16];

			sprint_move(engineMoveStr, engineMove);
			sprint_move(avoidMoveStr, avoidMove);

			int sameMove = ((engineMove.from == avoidMove.from) &&
			                (engineMove.to == avoidMove.to));
			printf("Engine Move: %s\nAvoid Move: %s\n%s\n", engineMoveStr, avoidMoveStr,
			sameMove ? "FAIL" : "PASS");

			if (!sameMove)
			{
				++hits;
			}
		}
		clear_transposition_table();
	}
	printf("Test finished: %i/%i (%.2f%%)\n",
	       hits,
	       epdFile->numEPD,
	       (float)hits * 100 / (float)epdFile->numEPD);
}

int alpha_beta(Board * board, int alpha, int beta, int depth)
{
	int i;
	Move moves[256];
	Move bestMove;
	int bestScore = -INFINITY;
	int cut = force_exact(beta);

	++nodes;

	if (depth == 0)
	{
		return 4 * eval_board_eval(board);
	}

	TTEntry* ttentry = get_tt_entry(board->zobrist);
	if (ttentry && ttentry->depth >= depth)
	{
		if (ttentry->score == force_exact(ttentry->score))
		{
			// We have an exact score for this node at at least this depth,
			// return it
			return ttentry->score;
		}
		else if (ttentry->score >= beta)
		{
			// We have a beta cut off for this node at at least this depth,
			// return it
			return force_lb(ttentry->score);
		}
	}

	unsigned int numMoves = generate_moves(board, moves);

	for (i = 0; i < numMoves; ++i)
	{
		make_move(board, moves[i]);
		int score = -alpha_beta(board, -beta, -alpha, depth - 1);
		unmake_move(board, moves[i]);

		if (score >= cut)
		{
			TTEntry new_entry;
			new_entry.key = board->zobrist;
			new_entry.best_move = moves[i];
			new_entry.score = force_lb(score);
			new_entry.depth = depth;
			new_entry.age = board->move;

			put_tt_entry(&new_entry);
			return force_lb(beta); //  fail hard beta-cutoff
		}
		if (score > alpha)
		{
			alpha = score;	// alpha acts like max in MiniMax
		}
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = moves[i];
		}
	}

	TTEntry new_entry;
	new_entry.key = board->zobrist;
	new_entry.best_move = bestMove;
	new_entry.score = alpha;
	new_entry.depth = depth;
	new_entry.age = board->move;

	put_tt_entry(&new_entry);

	return alpha;
}

