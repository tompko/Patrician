#include "epd.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "hashing/zobrist.h"

typedef enum 
{
	EPD_START,
	EPD_COMMENT,
	EPD_PIECES,
	EPD_SIDE_TO_MOVE,
	EPD_CASTLING,
	EPD_EN_PASSANT,
	EPD_OPERATIONS,
	EPD_OPCODE,
	EPD_OPERAND,
	EPD_END,
} EPDState;

void epd_free(EPD* epd);

EPDFile* epd_read_file(char* filename)
{
	FILE *file;
	char *contents = NULL, *string = NULL;
	size_t fileSize;
	EPDFile* epdFile = NULL;
	EPD* epd = NULL;
	EPDOperation* operation = NULL;
	EPDState state = EPD_START;
	int i = 0;
	int brank = 0, bfile = 0;
	int operandLength = 0, descriptionLength = 0;
	int done = 0;
	int lineNo = 1;

	file = fopen(filename, "r");

	if (!file)
	{
		fprintf(stderr,
		        "Unable to open file %s: %s\n",
		        filename,
		        strerror(errno));
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	contents = malloc(fileSize + 1);
	size_t bytesRead = fread(contents, 1, fileSize, file);
	if (bytesRead != fileSize)
	{
		fprintf(stderr, "Unable to read from file %s\n", filename);
		goto error;
	}
	contents[fileSize] = 0;
	string = contents;

	epdFile = (EPDFile*)malloc(sizeof(EPDFile));
	epdFile->epds = NULL;
	epdFile->numEPD = 0;

	while (!done)
	{
		switch(state)
		{
			case EPD_START:
				if (*string == '#')
				{
					state = EPD_COMMENT;
					++string;
				}
				else if (*string == '\n')
				{
					++string;
					++lineNo;
				}
				else if (*string == 0)
				{
					done = 1;
				}
				else
				{
					epdFile->numEPD++;
					epdFile->epds = realloc(epdFile->epds, sizeof(EPD) * epdFile->numEPD);
					epd = &epdFile->epds[epdFile->numEPD - 1];
					epd->operations = NULL;
					epd->numOperations = 0;

					epd->description = malloc(sizeof(char));
					epd->description[0] = 0;
					descriptionLength = 0;

					brank = 7;
					bfile = 0;
					memset(&epd->board, 0, sizeof(Board));

					epd->board.stateHistory = (BoardState*)malloc(sizeof(BoardState));
					epd->board.numHistory = 0;
					epd->board.maxHistory = 1;
					state = EPD_PIECES;
				}
				break;
			case EPD_COMMENT:
				if (*string == '\n')
				{
					++lineNo;
					state = EPD_START;
				}
				++string;
				break;
			case EPD_PIECES:
				switch(*string)
				{
					case 'r':
						epd->board.pieces[BLACK_ROOK] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[BLACK] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'n':
						epd->board.pieces[BLACK_KNIGHT] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[BLACK] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'b':
						epd->board.pieces[BLACK_BISHOP] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[BLACK] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'q':
						epd->board.pieces[BLACK_QUEEN] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[BLACK] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'k':
						epd->board.pieces[BLACK_KING] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[BLACK] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'p':
						epd->board.pieces[BLACK_PAWN] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[BLACK] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'R':
						epd->board.pieces[WHITE_ROOK] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[WHITE] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'N':
						epd->board.pieces[WHITE_KNIGHT] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[WHITE] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'B':
						epd->board.pieces[WHITE_BISHOP] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[WHITE] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'Q':
						epd->board.pieces[WHITE_QUEEN] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[WHITE] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'K':
						epd->board.pieces[WHITE_KING] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[WHITE] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case 'P':
						epd->board.pieces[WHITE_PAWN] |= 1ULL << (brank*8 + bfile);
						epd->board.sides[WHITE] |= 1ULL << (brank*8 + bfile);
						epd->board.occupied |= 1ULL << (brank*8 + bfile);
						epd->board.empty = ~epd->board.occupied;
						++bfile;
						break;
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
						bfile += *string - '0';
						break;
					case '/':
						brank -= 1;
						bfile = 0;
						break;
					case ' ':
						state = EPD_SIDE_TO_MOVE;
						break;
					default:
						fprintf(stderr,
						        "(%i) Unable to parse piece placement, unrecognised character: %c\n",
						        lineNo,
								*string);
						goto error;
				}
				epd->description[descriptionLength++] = *string;
				epd->description = realloc(epd->description, descriptionLength + 1);
				epd->description[descriptionLength] = 0;
				++string;
				break;
			case EPD_SIDE_TO_MOVE:
				if (*string == 'w')
				{
					epd->board.sideToMove = WHITE;
				}
				else if (*string == 'b')
				{
					epd->board.sideToMove = BLACK;
				}
				else
				{
					fprintf(stderr,
					        "(%i) Unable to parse side to move, unrecognised character: %c\n",
					        lineNo,
					        *string);
					goto error;
				}
				epd->description[descriptionLength++] = *string;
				epd->description = realloc(epd->description, descriptionLength + 1);
				epd->description[descriptionLength] = 0;
				++string;
				while(*string == ' ')
				{
					epd->description[descriptionLength++] = *string;
					epd->description = realloc(epd->description, descriptionLength + 1);
					epd->description[descriptionLength] = 0;
					++string;
				}
				state = EPD_CASTLING;
				break;
			case EPD_CASTLING:
				switch(*string)
				{
					case 'k':
						epd->board.castling |= 1u << BLACK_KINGSIDE;
						break;
					case 'q':
						epd->board.castling |= 1u << BLACK_QUEENSIDE;
						break;
					case 'K':
						epd->board.castling |= 1u << WHITE_KINGSIDE;
						break;
					case 'Q':
						epd->board.castling |= 1u << WHITE_QUEENSIDE;
						break;
					case '-':
						break;
					case ' ':
						state = EPD_EN_PASSANT;
						break;
					default:
						fprintf(stderr,
						        "(%i) Unable to parse castling, unrecognised character: %c\n",
						        lineNo,
						        *string);
						goto error;
				}
				epd->description[descriptionLength++] = *string;
				epd->description = realloc(epd->description, descriptionLength + 1);
				epd->description[descriptionLength] = 0;
				++string;
				break;
			case EPD_EN_PASSANT:
				if(*string == '-')
				{
					epd->board.enPassant = 0;
				}
				else if (*string == ' ')
				{
					epd->board.zobrist = calculate_zobrist(&epd->board);
					state = EPD_OPERATIONS;
				}
				else if (*string == '\n' || *string == 0)
				{
					state = EPD_END;
					--string;
				}
				else
				{
					if ('a' <= *string && *string <= 'h')
					{
						bfile = *string - 'a';
					}
					else
					{
						fprintf(stderr,
						        "(%i) Unable to parse en passant, unrecognised rank character: %c\n",
						        lineNo,
						        *string);
						goto error;
					}
					epd->description[descriptionLength++] = *string;
					epd->description = realloc(epd->description, descriptionLength + 1);
					epd->description[descriptionLength] = 0;
					++string;
			
					if ('1' <= *string && *string <= '8')
					{
						brank = *string - '1';
					}
					else
					{
						fprintf(stderr,
						        "(%i) Unable to parse en passant, unrecognised file character: %c\n",
						        lineNo,
						        *string);
						goto error;
					}
			
					epd->board.enPassant = 1ull << (brank*8 + bfile);
				}
				epd->description[descriptionLength++] = *string;
				epd->description = realloc(epd->description, descriptionLength + 1);
				epd->description[descriptionLength] = 0;
				++string;
				break;
			case EPD_OPERATIONS:
				if (*string == '\n' || *string == 0)
				{
					--string;
					state = EPD_END;
				}
				else if (*string != ' ')
				{
					epd->numOperations++;
					epd->operations = realloc(epd->operations,
					                          sizeof(EPDOperation)*epd->numOperations);
					operation = &epd->operations[epd->numOperations - 1];

					for (i = 0; i < 16; ++i)
					{
						operation->opcode[i] = 0;
					}
					operation->operand = NULL;

					state = EPD_OPCODE;
					--string;
				}
				++string;
				break;
			case EPD_OPCODE:
				if (*string == ' ')
				{
					operation->operand = (char*)malloc(sizeof(char));
					operation->operand[0] = 0;
					operandLength = 0;
					state = EPD_OPERAND;
				}
				else
				{
					if (strlen(operation->opcode) == 0 && 
					    !('a' <= *string && *string <= 'z') &&
					    !('A' <= *string && *string <= 'Z'))
					{
						fprintf(stderr,
						        "(%i) Expected letter as first character of opcode\n",
						        lineNo);
						goto error;
					}
					strncat(operation->opcode, string, 1);
					if(strlen(operation->opcode) > 14)
					{
						fprintf(stderr,
						        "(%i) Opcode was longer than 14 characters, %s\n",
						        lineNo,
						        operation->opcode);
						goto error;
					}
				}
				++string;
				break;
			case EPD_OPERAND:
				if (*string == ';')
				{
					state = EPD_OPERATIONS;
				}
				else
				{
					operation->operand = realloc(operation->operand, operandLength + 2);
					operation->operand[operandLength++] = *string;
					operation->operand[operandLength] = 0;
				}
				++string;
				break;
			case EPD_END:
				if (*string == 0)
				{
					--string;
					done = 1;
				}
				else
				{
					state = EPD_START;
				}
				++string;
				break;
		}
	}

	free(contents);
	fclose(file);
	return epdFile;

error:
	if (file)
	{
		fclose(file);
	}
	if (contents)
	{
		free(contents);
	}
	if (epdFile)
	{
		epd_file_free(epdFile);
	}

	return NULL;
}

void epd_file_free(EPDFile* epdFile)
{
	int i = 0;

	for (i = 0; i < epdFile->numEPD; ++i)
	{
		epd_free(&epdFile->epds[i]);
	}

	free(epdFile->epds);
	epdFile->epds = NULL;
	epdFile->numEPD = 0;
}

void epd_free(EPD* epd)
{
	int i = 0;
	free(epd->board.stateHistory);

	for (i = 0; i < epd->numOperations; ++i)
	{
		free(epd->operations[i].operand);
	}

	free(epd->operations);
	epd->operations = NULL;
	epd->numOperations = 0;

	free(epd->description);
}

int epd_has_operation(EPD* epd, char* opcode)
{
	int i;
	for (i = 0; i < epd->numOperations; ++i)
	{
		if (!strcmp(epd->operations[i].opcode, opcode))
		{
			return 1;
		}
	}

	return 0;
}

int epd_int_operation(EPD* epd, char* opcode)
{
	int i;
	for (i = 0; i < epd->numOperations; ++i)
	{
		if (!strcmp(epd->operations[i].opcode, opcode))
		{
			return atoi(epd->operations[i].operand);
		}
	}

	return 0;
}

unsigned int epd_uint_operation(EPD* epd, char* opcode)
{
	int i;
	for (i = 0; i < epd->numOperations; ++i)
	{
		if (!strcmp(epd->operations[i].opcode, opcode))
		{
			return strtoul(epd->operations[i].operand, NULL, 10);
		}
	}

	return 0;
}

Move epd_move_operation(EPD* epd, char* opcode)
{
	int i;
	Move move;

	for (i = 0; i < epd->numOperations; ++i)
	{
		if (!strcmp(epd->operations[i].opcode, opcode))
		{
			return make_move_from_san(&epd->board, epd->operations[i].operand);
		}
	}

	return move;
}

