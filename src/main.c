#include <stdio.h>
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

#define NUM_COMMANDS (3)
Command userCommands[NUM_COMMANDS];

void initCommands()
{
	userCommands[0].command = "help";
	userCommands[0].helpString = "Print all available commands";
	userCommands[0].function = help;

	userCommands[1].command = "perftTest";
	userCommands[1].helpString = "Run the perft test suite on the move generator";
	userCommands[1].function = test_perft;

	userCommands[2].command = "quit";
	userCommands[2].helpString = "Quit Patrician";
	userCommands[2].function = 0;
}

void main(void)
{
	char input[512] = "";

	/*Turn off buffering on the output stream as recommended
	  for the xboard protocol*/
	setbuf(stdout, NULL);

	printf("Patrician version 0.01\n");
	printf("Copyright (C) 2012 Chris Tompkinson\n");
	printf("\n");

	initCommands();

	test_perft("");

	return;

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
			if(spaceLoc)
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
		
		if (!validCommand)
		{
			printf("Unrecognised command: %s\n", input);
		}	
	}
}

void help(const char* input)
{
	int i;

	for(i = 0; i < NUM_COMMANDS; ++i)
	{
		printf("%s\t%s\n", userCommands[i].command, userCommands[i].helpString);
	}
}

void test_perft(const char* input)
{
	Game game;
	int i, j;

	initPerftTests();
	printf("\n\n\n");

	for (i = 0; i < NUM_PERFT_TESTS; ++i)
	{
		printf("FEN: %s\n", perftTests[i].FEN);
		set_game_from_FEN(&game, perftTests[i].FEN);
		for (j = 0; j < 6; ++j)
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
	}
}