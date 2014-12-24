#include "perft.h"

#include <stdio.h>

#include "bitscans.h"
#include "board.h"
#include "move.h"
#include "game.h"
#include "gameTime.h"
#include "hashing/transposition_table.h"
#include "io/epd.h"

int perft_test_perft(EPDFile* epdFile)
{
	int i = 0;
	Timer totalTimer, perftTimer;
	printf("Testing %i perfts\n", epdFile->numEPD);

	start_timer(&totalTimer);

	for (i = 0; i < epdFile->numEPD; ++i)
	{
		EPD* epd = &epdFile->epds[i];
		int depth = 1;
		char depthKey[8];
		unsigned int perftExpected = 0, perftActual;

		printf("%i/%i %s\n", i + 1, epdFile->numEPD, epd->description);

		while (1)
		{
			snprintf(depthKey, 8, "D%i", depth);

			if (!epd_has_operation(epd, depthKey))
			{
				break;
			}

			perftExpected = epd_uint_operation(epd, depthKey);

			start_timer(&perftTimer);
			perftActual = perft_perft(&epd->board, depth);
			stop_timer(&perftTimer);

			printf("perft %i: %i [%03fs] (%i) %s\n", depth, perftActual, get_elapsed_time(&perftTimer), perftExpected,
			perftActual == perftExpected ? "PASS" : "FAIL");
			if (perftActual != perftExpected)
			{
				printf("Fail in perft test\n");
				printf("FEN: %s\n", epd->description);
				printf("Level: %i\n", depth);
				printf("Expected: %i\n", perftExpected);
				printf("Found: %i\n", perftActual);
				return 0;
			}

			++depth;
		}

		printf("\n");
	}

	stop_timer(&totalTimer);
	printf("Perft suite finished in %03fs\n", get_elapsed_time(&totalTimer));

	return 1;
}

unsigned int perft_perft(Board* board, unsigned int depth)
{
	int i, nperft = 0;
	Move moves[256];

	if (depth == 0)
	{
		return 1;
	}

	TTEntry* ttentry = get_tt_entry_at_depth(board->zobrist, depth);
	if (ttentry)
	{
		return ttentry->score;
	}

	unsigned int numMoves = generate_moves(board, moves);

	for (i = 0; i < numMoves; ++i)
	{
		make_move(board, &moves[i]);
		if ((board->sideToMove == BLACK &&
			!black_attacks_square(board, bit_scan_forward(board->pieces[WHITE_KING]))) ||
			(board->sideToMove == WHITE &&
			!white_attacks_square(board, bit_scan_forward(board->pieces[BLACK_KING]))))
		{
			nperft += perft_perft(board, depth - 1);
		}
		unmake_move(board, moves[i]);
	}

	TTEntry new_entry;
	new_entry.key = board->zobrist;
	new_entry.score = nperft;
	new_entry.depth = depth;

	put_tt_entry(&new_entry);

	return nperft;
}

void perft_divide(FILE* stream, Board* board, unsigned int depth)
{
	int i, totalMoves = 0;
	Move moves[256];

	if (depth <= 0)
	{
		return;
	}

	int num_moves = generate_moves(board, moves);

	for (i = 0; i < num_moves; ++i)
	{
		int numDividedMoves = 0;
		char moveString[8];
		sprint_move(moveString, moves[i]);
		printf("%s ", moveString);

		make_move(board, &moves[i]);
		numDividedMoves = perft_perft(board, depth - 1);
		unmake_move(board, moves[i]);

		totalMoves += numDividedMoves;
		printf("%i\n", numDividedMoves);
	}

	fprintf(stream, "\nMoves: %i\n", num_moves);
	fprintf(stream, "Nodes: %i\n", totalMoves);
}

