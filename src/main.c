#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "defines.h"
#include "perftTest.h"
#include "game.h"
#include "bitscans.h"
#include "gameTime.h"
#include "evaluation.h"
#include "engine.h"
#include "hashing/zobrist.h"
#include "hashing/transposition_table.h"
#include "debug_log.h"

typedef struct
{
	char * command;
	char * helpString;
	void (*function)(const char* input);
} Command;

enum InputMode
{
	MODE_SINGLELINE,
	MODE_XBOARD,
	//MODE_UCI,			//Not currently supported
};

void initCommands();
int singleLineInput();
int xboardInput();

void help(const char * input);
void test_perft(const char* input);
void test_eval(const char* input);
void set_fen(const char * input);
void run_perft(const char * input);
void run_divide(const char* input);
void display(const char* input);
void new_game(const char* input);

static Board s_Board;

#define NUM_COMMANDS (9)
Command userCommands[NUM_COMMANDS];
enum InputMode inputMode = MODE_SINGLELINE;

FILE* file;

int main(void)
{
	int done = 0;
	/*Turn off buffering on the output stream as recommended
	  for the xboard protocol*/
	setbuf(stdout, NULL);

	INIT_LOG();
	initCommands();
	init_zobrist_keys();
	init_transposition_table(256*1024*1024);

	start_engine_thread();

	printf("Patrician version 0.01\n");
	printf("Copyright (C) 2012 Chris Tompkinson\n");
	printf("\n");

	while(!done)
	{
		switch(inputMode)
		{
			case MODE_SINGLELINE:

				done = singleLineInput();
				break;
			case MODE_XBOARD:
				done = xboardInput();
				break;
		}
	}

	RELEASE_LOG();

	return 0;
}

void initCommands()
{
	userCommands[0].command = "help";
	userCommands[0].helpString = "Print all available commands";
	userCommands[0].function = help;

	userCommands[1].command = "perftTest";
	userCommands[1].helpString = "Run the perft test suite on the move generator";
	userCommands[1].function = test_perft;

	userCommands[2].command = "evalTest";
	userCommands[2].helpString = "Test the consistency of the evaluation function";
	userCommands[2].function = test_eval;

	userCommands[3].command = "fen";
	userCommands[3].helpString = "Set the current position from the given FEN";
	userCommands[3].function = set_fen;

	userCommands[4].command = "perft";
	userCommands[4].helpString = "Count the number of nodes at a given depth";
	userCommands[4].function = run_perft;

	userCommands[5].command = "divide";
	userCommands[5].helpString = "Show the number of nodes per child move";
	userCommands[5].function = run_divide;

	userCommands[6].command = "display";
	userCommands[6].helpString = "Display the current board";
	userCommands[6].function = display;

	userCommands[7].command = "new";
	userCommands[7].helpString = "Start a new game";
	userCommands[7].function = new_game;

	userCommands[8].command = "quit";
	userCommands[8].helpString = "Quit Patrician";
	userCommands[8].function = 0;
}

int singleLineInput()
{
	char input[512] = "";
	int validCommand = 0, i = 0;

	printf("patrician: ");
	char *stream = fgets(input, 512, stdin);
	if (!stream)
	{
		return 1;
	}

	unsigned long length = strlen(input) - 1;
	if (input[length] == '\n')
	{
		input[length] = 0;
	}

	LOG("input", input);

	if(!strcmp(input, "quit"))
	{
		return 1;
	}

	for(i = 0; i < NUM_COMMANDS; ++i)
	{
		char* spaceLoc = strchr(input, ' ');

		if(spaceLoc && (size_t)(spaceLoc - input) == strlen(userCommands[i].command))
		{
			if(!strncmp(input, userCommands[i].command, spaceLoc - input))
			{
				userCommands[i].function(&input[spaceLoc - input + 1]);
				validCommand = 1;
			}
		}
		else
		{
			if(!strcmp(input, userCommands[i].command))
			{
				userCommands[i].function("");
				validCommand = 1;
			}
		}
	}

	if(!strcmp(input, "xboard"))
	{
		inputMode = MODE_XBOARD;
		printf("\n");
	}
	else if(is_move(input))
	{
		Move move = make_move_from_str(&s_Board, input);
		make_move(&s_Board, &move);
	}
	else if (!validCommand)
	{
		LOG("urecognised", input);
		printf("Unrecognised command: %s\n", input);
	}

	return 0;
}

int xboardInput()
{
	char input[512] = "";

	char *stream = fgets(input, 512, stdin);
	if (!stream)
	{
		return 1;
	}

	LOG("xboard_input", input);

	if(!strncmp(input, "protover", 8))
	{
		// Send a string describing the features we support
		printf("feature done=0 ping=1 setboard=1 playother=1 san=0 usermove=1 time=1 sigint=0 sigterm=0 draw=1 reuse=1 analyze=0 myname=\"Patrician\" variants=\"normal\" colors=0 ics=1 name=1 pause=1 nps=0 debug=1 memory=1 smp=0 done=1\n");
	}
	else if(!strcmp(input, "new"))
	{
		// Reset the board to the starting position, set random mode off
		engine_new_game();
		set_random_mode(0);
	}
	else if(!strncmp(input, "memory", 6))
	{
		// Set the size of memory we're allowed to use for our hash tables etc.
		int megaBytes = atoi(input + 7);
		set_memory_limit(megaBytes);
	}
	else if(!strcmp(input, "random"))
	{
		// Toggle random mode
		set_random_mode(1 - get_random_mode());
	}
	else if(!strncmp(input, "ics", 3))
	{
		set_ics_host(input + 4);
		set_ics_mode(!strcmp(input+4, "-"));
	}
	else if(!strncmp(input, "level", 5))
	{
		// Time control string, of the format "level MPS BASE INC" where MPS is the number of moves per time control
		// BASE is the starting clock time and INC is the increment to be applied after MPS moves. The two times are
		// given as mins[:secs]
		const char * timeString = input + 6;
		int mps = 0, baseMins = 0, baseSecs = 0, incMins = 0, incSecs = 0;

		mps = atoi(timeString);
		while(*(timeString++) != ' ');
		timeString = parse_time(timeString, &baseMins, &baseSecs);
		timeString = parse_time(timeString, &incMins, &incSecs);

		set_mps(mps);
		set_base_time(baseMins, baseSecs);
		set_inc_time(incMins, incSecs);
	}
	else if(!strncmp(input, "accepted", 8) || !strncmp(input, "rejected", 8))
	{
		//Do nothing, we get these in response to our feature string
	}
	else if(!strcmp(input, "post"))
	{
		set_show_thinking(1);
	}
	else if(!strcmp(input, "nopost"))
	{
		set_show_thinking(0);
	}
	else if(!strcmp(input, "hard"))
	{
		set_pondering(1);
	}
	else if(!strcmp(input, "easy"))
	{
		set_pondering(0);
	}
	else if(!strncmp(input, "ping", 4))
	{
		printf("pong%s\n", input+4);
	}
	else if(!strncmp(input, "name", 4))
	{
		set_opponent_name(input+5);
	}
	else if (!strcmp(input, "computer"))
	{
		//do nothing for now
	}
	else if(!strncmp(input, "time", 4))
	{
		set_engine_time(input+5);
	}
	else if(!strncmp(input, "otim", 4))
	{
		set_opponent_time(input+5);
	}
	else if(!strncmp(input, "usermove", 8))
	{
		engine_force_move(input+9);
	}
	else if(!strncmp(input, "result", 6))
	{
		engine_set_result(input+7);
	}
	else if(!strcmp(input, "force"))
	{
		engine_force_mode();
	}
	else if(!strcmp(input, "go"))
	{
		engine_go();
	}
	else if(!strcmp(input, "undo"))
	{
		engine_undo();
	}
	else if(!strcmp(input, "quit"))
	{
		return 1;
	}
	else
	{
		LOG("xboard_unrecognised", input);
		exit(0);
	}

	return 0;
}

void help(const char* input)
{
	int i;
	UNUSED(input);

	for(i = 0; i < NUM_COMMANDS; ++i)
	{
		printf("%s\t%s\n", userCommands[i].command, userCommands[i].helpString);
	}
}

void test_perft(const char* input)
{
	int i, j;
	Board board;
	Timer totalTimer, perftTimer;
	UNUSED(input);

	initPerftTests();
	printf("\n\n\n");

	start_timer(&totalTimer);

	for (i = 0; i < NUM_PERFT_TESTS; ++i)
	{
		printf("%i/%i FEN: %s\n", i+1, NUM_PERFT_TESTS, perftTests[i].FEN);
		set_from_FEN(&board, perftTests[i].FEN);
		for (j = 0; j < 6; ++j)
		{
			if (perftTests[i].perfts[j] >= 0)
			{
				int result;

				start_timer(&perftTimer);
				result = perft(&board, j+1);
				stop_timer(&perftTimer);

				printf("perft %i: %i [%03fs] (%i) %s\n", j+1, result, get_elapsed_time(&perftTimer), perftTests[i].perfts[j],
					result == perftTests[i].perfts[j] ? "PASS" : "FAIL");
				if (result != perftTests[i].perfts[j])
				{
					printf("Fail in perft test\n");
					printf("FEN: %s\n", perftTests[i].FEN);
					printf("Level: %i\n", j+1);
					printf("Expected: %i\n", perftTests[i].perfts[j]);
					printf("Found: %i\n", result);
					return;
				}
			}
		}
		printf("\n");
	}

	stop_timer(&totalTimer);
	printf("Perft suite finished in %03fs\n", get_elapsed_time(&totalTimer));
}

void test_eval(const char* input)
{
	int i, j;
	Board board;

	UNUSED(input);

	initPerftTests();
	printf("\n\n\n");

	for(i = 0; i < NUM_PERFT_TESTS; ++i)
	{
		int score, mirrorScore, otherScore, otherMirrorScore;

		set_from_FEN(&board, perftTests[i].FEN);

		score = evaluate(&board);

		board.sideToMove = 1 - board.sideToMove;

		otherScore = -evaluate(&board);

		for(j = 0; j < NUM_PIECES; ++j)
		{
			board.pieces[j] = mirror_horizontal(board.pieces[j]);
		}

		otherMirrorScore = -evaluate(&board);

		board.sideToMove = 1 - board.sideToMove;

		mirrorScore = evaluate(&board);

		printf("%s: ", perftTests[i].FEN);

		if(score != mirrorScore || mirrorScore != otherScore || otherScore != otherMirrorScore)
		{
			printf("FAIL\n");
			printf("Score: %i\nMirror Score: %i\n", score, mirrorScore);
			printf("Other Score: %i\nOther Mirror Score: %i\n", otherScore, otherMirrorScore);
			return;
		}
		else
		{
			printf("PASS\n");
		}
	}
}

void set_fen(const char * input)
{
	set_from_FEN(&s_Board, input);
}

void run_perft(const char * input)
{
	int level = atoi(input);

	int result = perft(&s_Board, level);

	printf("perft %i: %i\n", level, result);
}

void run_divide(const char* input)
{
	int level = atoi(input);

	divide(&s_Board, level);
}

void display(const char* input)
{
	UNUSED(input);
	print_board(&s_Board);
}

void new_game(const char* input)
{
	UNUSED(input);
	set_from_FEN(&s_Board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
