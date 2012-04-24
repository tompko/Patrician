#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "perftTest.h"
#include "game.h"
#include "bitscans.h"

typedef struct
{
	char * command;
	char * helpString;
	void (*function)(const char* input);
} Command;

void help(const char * input);
void test_perft(const char* input);
void set_fen(const char * input);
void run_perft(const char * input);
void run_divide(const char* input);
void display(const char* input);
void new_game(const char* input);

Game g_Game;

#define NUM_COMMANDS (8)
Command userCommands[NUM_COMMANDS];

void initCommands()
{
	userCommands[0].command = "help";
	userCommands[0].helpString = "Print all available commands";
	userCommands[0].function = help;

	userCommands[1].command = "perftTest";
	userCommands[1].helpString = "Run the perft test suite on the move generator";
	userCommands[1].function = test_perft;

	userCommands[2].command = "fen";
	userCommands[2].helpString = "Set the current position from the given FEN";
	userCommands[2].function = set_fen;

	userCommands[3].command = "perft";
	userCommands[3].helpString = "Count the number of nodes at a given depth";
	userCommands[3].function = run_perft;

	userCommands[4].command = "divide";
	userCommands[4].helpString = "Show the number of nodes per child move";
	userCommands[4].function = run_divide;

	userCommands[5].command = "display";
	userCommands[5].helpString = "Display the current board";
	userCommands[5].function = display;

	userCommands[6].command = "new";
	userCommands[6].helpString = "Start a new game";
	userCommands[6].function = new_game;

	userCommands[7].command = "quit";
	userCommands[7].helpString = "Quit Patrician";
	userCommands[7].function = 0;
}

int main(void)
{
	char input[512] = "";

	/*Turn off buffering on the output stream as recommended
	  for the xboard protocol*/
	setbuf(stdout, NULL);

	printf("Patrician version 0.01\n");
	printf("Copyright (C) 2012 Chris Tompkinson\n");
	printf("\n");

	initCommands();

	for(;;)
	{
		int validCommand = 0;
		int i;
		printf("patrician: ");
		gets(input);
		if(!strcmp(input, "quit"))
		{
			break;
		}

		for(i = 0; i < NUM_COMMANDS; ++i)
		{
			char* spaceLoc = strchr(input, ' ');

			if(spaceLoc && spaceLoc - input == strlen(userCommands[i].command))
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

		if(is_move(input))
		{
			Move move = make_move_from_str(&g_Game.board, input);
			make_move(&g_Game.board, &move);
			g_Game.moves.children = NULL;
			g_Game.moves.numChildren = 0;
			g_Game.moves.maxChildren = 0;
		}
		else if (!validCommand)
		{
			printf("Unrecognised command: %s\n", input);
		}	
	}

	return 0;
}

void help(const char* input)
{
	int i;

	for(i = 0; i < NUM_COMMANDS; ++i)
	{
		printf("%s\t%s\n", userCommands[i].command, userCommands[i].helpString);
	}
}

static int maxPerftLevel = 2;

void test_perft(const char* input)
{
	Game game;
	game.moves.children = NULL;
	int i, j;

	initPerftTests();
	printf("\n\n\n");

	for (i = 0; i < NUM_PERFT_TESTS; ++i)
	{
		printf("FEN: %s\n", perftTests[i].FEN);
		set_game_from_FEN(&game, perftTests[i].FEN);
		for (j = 0; j < maxPerftLevel; ++j)
		{
			if (perftTests[i].perfts[j] >= 0)
			{
				int result = perft(&game, j+1);
				printf("perft %i: %i (%i) %s\n", j+1, result, perftTests[i].perfts[j],
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
		free_move_node(&game.moves);
	}
}

void set_fen(const char * input)
{
	set_game_from_FEN(&g_Game, input);
}

void run_perft(const char * input)
{
	int level = atoi(input);

	int result = perft(&g_Game, level);

	printf("perft %i: %i\n", level, result);
}

void run_divide(const char* input)
{
	int level = atoi(input);

	divide(&g_Game, level);
}

void display(const char* input)
{
	print_board(&g_Game.board);
}

void new_game(const char* input)
{
	set_game_from_FEN(&g_Game, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
