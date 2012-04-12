#include <stdio.h>

int testsPassed = 0;
int testsFailed = 0;

void main(void)
{
	int totalTests = testsPassed + testsFailed;
	printf("Passed %i/%i\n", testsPassed, totalTests);
}
