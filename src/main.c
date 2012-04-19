#include <stdio.h>
#include <string.h>

void main(void)
{
	char input[512] = "";

	/*Turn off buffering on the output stream as recommended
	  for the xboard protocol*/
	setbuf(stdout, NULL);

	printf("Patrician version 0.01\n");
	printf("Copyright (C) 2012 Chris Tompkinson\n");
	printf("\n");

	for(;;)
	{
		printf("patrician: ");
		gets(input);
		if(!strcmp(input, "quit"))
		{
			break;
		}
		else
		{
			printf("Unrecognised command: %s\n", input);
		}	
	}
}
