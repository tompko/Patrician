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

typedef struct SearchMove
{
	Move move;
	int score;
} SearchMove;

static int nodes;

int alpha_beta(Board * board, int alpha, int beta, int depth, Timer* searchTimer, double timeToSearch);
int get_move_score(Board* board, Move move);
int compare_search_moves(const void* a, const void* b);
int compare_moves(const void* a, const void* b);

static inline int force_exact(int x)
{
	return (x + 1) & ~3;
}

static inline int force_lb(int x)
{
	return force_exact(x) + 1;
}

Move root_search(Board * board, double timeToSearch)
{
	int i, depth;
	unsigned int numMoves = 0, legalMoves = 0;
	Move moves[256];
	SearchMove smoves[256];
	Move bestMove = 0;
	int bestScore = -INFINITY;
	Timer searchTimer;
	char moveStr[16];

	nodes = 0;
	depth = 0;
	start_timer(&searchTimer);

	numMoves = generate_moves(board, moves);

	for (i = 0; i < numMoves; ++i)
	{
		make_move(board, moves[i]);

		if ((board->sideToMove == BLACK &&
			!black_attacks_square(board, bit_scan_forward(board->pieces[WHITE_KING]))) ||
			(board->sideToMove == WHITE &&
			!white_attacks_square(board, bit_scan_forward(board->pieces[BLACK_KING]))))
		{
			smoves[legalMoves].move = moves[i];
			smoves[legalMoves].score = -alpha_beta(board, -INFINITY, INFINITY, 0, &searchTimer, timeToSearch);
			++legalMoves;
		}

		unmake_move(board, moves[i]);
	}

	qsort(smoves, legalMoves, sizeof(SearchMove), compare_search_moves);

	while (get_elapsed_time(&searchTimer) < timeToSearch)
	{
		++depth;
		bestScore = -INFINITY;

		for (i = 0; i < legalMoves; ++i)
		{
			make_move(board, smoves[i].move);
			smoves[i].score = -alpha_beta(board, bestScore, INFINITY, depth, &searchTimer, timeToSearch);

			if (get_elapsed_time(&searchTimer) > timeToSearch)
			{
				// We probably aborted the search for this score, so it's not valid
				// ignore it.
				unmake_move(board, smoves[i].move);
				break;
			}

			if (smoves[i].score > bestScore)
			{
				int centi_seconds = (int)(get_elapsed_time(&searchTimer) * 100);
				bestScore = smoves[i].score;
				bestMove = smoves[i].move;
				sprint_move(moveStr, bestMove);
				// ply score time(centiseconds) nodes (selective depth) (speed) (tbhits) \t pv
				printf("%i %i %i %i\t%s\n", depth, smoves[i].score / 4, centi_seconds, nodes, moveStr);
			}
			unmake_move(board, smoves[i].move);
		}
		qsort(smoves, legalMoves, sizeof(SearchMove), compare_search_moves);
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
			Move engineMove = root_search(&epd->board, 60.0);
			Move bestMove = epd_move_operation(epd, "bm");
			char engineMoveStr[16], bestMoveStr[16];

			sprint_move(engineMoveStr, engineMove);
			sprint_move(bestMoveStr, bestMove);

			int sameMove = engineMove == bestMove;
			printf("Engine Move: %s (%i)\nBest Move: %s (%i)\n%s\n",
			       engineMoveStr,
			       get_move_score(&epd->board, engineMove),
			       bestMoveStr,
			       get_move_score(&epd->board, bestMove),
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
			Move engineMove = root_search(&epd->board, 60.0);
			Move avoidMove = epd_move_operation(epd, "am");
			char engineMoveStr[16], avoidMoveStr[16];

			sprint_move(engineMoveStr, engineMove);
			sprint_move(avoidMoveStr, avoidMove);

			int sameMove = engineMove == avoidMove;
			printf("Engine Move: %s (%i)\nAvoid Move: %s (%i)\n%s\n",
			       engineMoveStr,
			       get_move_score(&epd->board, engineMove),
			       avoidMoveStr,
			       get_move_score(&epd->board, avoidMove),
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

int alpha_beta(Board * board, int alpha, int beta, int depth, Timer* searchTimer, double timeToSearch)
{
	int i;
	Move moves[256];
	Move bestMove = 0;
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
	else if (ttentry && ttentry->best_move)
	{
		// Try the hash move for a fast beta-cutoff
		make_move(board, ttentry->best_move);
		int score = -alpha_beta(board, -beta, -alpha, depth - 1, searchTimer, timeToSearch);
		unmake_move(board, ttentry->best_move);

		if (score >= cut)
		{
			return force_lb(beta);
		}
	}

	unsigned int numMoves = generate_moves(board, moves);

	qsort(moves, numMoves, sizeof(Move), compare_moves);

	for (i = 0; i < numMoves; ++i)
	{
		if ((board->sideToMove == BLACK &&
			!black_attacks_square(board, bit_scan_forward(board->pieces[WHITE_KING]))) ||
			(board->sideToMove == WHITE &&
			!white_attacks_square(board, bit_scan_forward(board->pieces[BLACK_KING]))))
		{
			make_move(board, moves[i]);
			int score = -alpha_beta(board, -beta, -alpha, depth - 1, searchTimer, timeToSearch);
			unmake_move(board, moves[i]);

			if (get_elapsed_time(searchTimer) > timeToSearch)
			{
				return 0;
			}

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

int get_move_score(Board* board, Move move)
{
	TTEntry* ttentry;

	make_move(board, move);
	ttentry = get_tt_entry(board->zobrist);
	unmake_move(board, move);

	return ttentry->score;
}

int compare_search_moves(const void* a, const void* b)
{
	const SearchMove* sma = (const SearchMove*)a;
	const SearchMove* smb = (const SearchMove*)b;

	return (sma->score < smb->score) - (sma->score > smb->score);
	// The above is taken from
	// http://stackoverflow.com/questions/10996418/efficient-integer-compare-function
	// and is functionally identical to the below, but optimizes better
	// if (sma->score > smb->score)
	// 	return -1;
	// else if (sma->score < smb->score)
	// 	return 1;
	// return 0;
}

int compare_moves(const void* a, const void* b)
{
	const Move* ma = (const Move*)a;
	const Move* mb = (const Move*)b;

	return (*ma < *mb) - (*ma > *mb);
}

