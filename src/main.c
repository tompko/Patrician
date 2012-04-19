#include <stdio.h>
#include <string.h>

typedef struct
{
	char * command;
	char * helpString;
	void (*function)(char* input);
} Command;

void help(char * input);

Command* userCommands = NULL;
static const int numCommands = 2;

void initCommands()
{
	userCommands = (Command*)malloc(sizeof(Command)*numCommands);
	
	userCommands[0].command="help";
	userCommands[0].helpString="Print all available commands";
	userCommands[0].function=help;

	userCommands[1].command="quit";
	userCommands[1].helpString="Quit Patrician";
	userCommands[1].function = 0;
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

		for(i = 0; i < numCommands; ++i)
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

void help(char * input)
{
	int i;
	for(i = 0; i < numCommands; ++i)
	{
		printf("%s\t%s\n", userCommands[i].command, userCommands[i].helpString);
	}
}
