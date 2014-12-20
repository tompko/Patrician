#ifndef EPD_H
#define EPD_H

#include "board.h"

struct Move;
typedef struct EPD EPD;

typedef struct EPDOperation
{
	char opcode[16];
	char* operand;
} EPDOperation;

typedef struct EPD
{
	Board board;
	char* description;
	EPDOperation* operations;
	int numOperations;
} EPD;

typedef struct EPDFile
{
	EPD* epds;
	unsigned int numEPD;
} EPDFile;

EPDFile* epd_read_file(char* filename);
void epd_file_free(EPDFile* epd);

int epd_has_operation(EPD* epd, char* opcode);
int epd_int_operation(EPD* epd, char* opcode);
unsigned int epd_uint_operation(EPD* epd, char* opcode);

#endif

