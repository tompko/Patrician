#include "search.h"

#include <malloc.h>

#include "bitscans.h"
#include "game.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "io/epd.h"

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
		if ((board->sideToMove == BLACK &&
			!black_attacks_square(board, bit_scan_forward(board->pieces[WHITE_KING]))) ||
			(board->sideToMove == WHITE &&
			!white_attacks_square(board, bit_scan_forward(board->pieces[BLACK_KING]))))
		{
			int score = alpha_beta (board, -1000000, 1000000, 1);

			if (score < bestScore)
			{
				bestScore = score;
				bestMove = moves[i];
			}
		}
		unmake_move (board, moves[i]);
	}

	return bestMove;
}

void search_test_search(EPDFile* epdFile)
{
	int i = 0;
	int hits = 0;

	printf("Testing %i positions\n", epdFile->numEPD);

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
	}
	printf("Test finished: %i/%i (%.2f%%)\n", hits, epdFile->numEPD, (float)hits * 100 / (float)epdFile->numEPD);
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

