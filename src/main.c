#include <stdio.h>

void main(void)
{
	/*Turn off buffering on the output stream as recommended
	  for the xboard protocol*/
	setbuf(stdout, NULL);

	printf("Patrician version 0.01\n");
	printf("Copyright (C) 2012 Chris Tompkinson\n");
	printf("\n");

	char input[512] = "";

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
