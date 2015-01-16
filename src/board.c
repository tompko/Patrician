#include "board.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hashing/zobrist.h"
#include "eval/piece_tables.h"
#include "bitscans.h"
#include "evaluation.h"
#include "debug_log.h"

char * strSquare[] = {
"a1","b1","c1","d1","e1","f1","g1","h1",
"a2","b2","c2","d2","e2","f2","g2","h2",
"a3","b3","c3","d3","e3","f3","g3","h3",
"a4","b4","c4","d4","e4","f4","g4","h4",
"a5","b5","c5","d5","e5","f5","g5","h5",
"a6","b6","c6","d6","e6","f6","g6","h6",
"a7","b7","c7","d7","e7","f7","g7","h7",
"a8","b8","c8","d8","e8","f8","g8","h8"
};

bitboard squareFiles[] = {
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080
};

bitboard ranks[] = 
{
	0x00000000000000FF,
	0x000000000000FF00,
	0x0000000000FF0000,
	0x00000000FF000000,
	0x000000FF00000000,
	0x0000FF0000000000,
	0x00FF000000000000,
	0xFF00000000000000,
};

bitboard backranks[] = 
{
	0xFF00000000000000,
	0x00000000000000FF
};

int set_from_FEN(Board* board, const char* FEN)
{
	int i;
	int rank = 7, file = 0;
	memset(board, 0, sizeof(Board));

	for (i = 0; i < 64; ++i)
	{
		board->mailbox[i] = NUM_PIECES;
	}

	board->stateHistory = (BoardState*)malloc(sizeof(BoardState));
	board->numHistory = 0;
	board->maxHistory = 1;

	while(*FEN != ' ')
	{
		switch(*FEN)
		{
			case 'r':
			{
				board->mailbox[rank*8 + file] = BLACK_ROOK;
				board->staticScore -= pieceSquareValues[BLACK_ROOK][rank*8 + file];
				board->staticScore -= pieceValues[BLACK_ROOK];
				board->pieces[BLACK_ROOK] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'n':
			{
				board->mailbox[rank*8 + file] = BLACK_KNIGHT;
				board->staticScore -= pieceSquareValues[BLACK_KNIGHT][rank*8 + file];
				board->staticScore -= pieceValues[BLACK_KNIGHT];
				board->pieces[BLACK_KNIGHT] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'b':
			{
				board->mailbox[rank*8 + file] = BLACK_BISHOP;
				board->staticScore -= pieceSquareValues[BLACK_BISHOP][rank*8 + file];
				board->staticScore -= pieceValues[BLACK_BISHOP];
				board->pieces[BLACK_BISHOP] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'q':
			{
				board->mailbox[rank*8 + file] = BLACK_QUEEN;
				board->staticScore -= pieceSquareValues[BLACK_QUEEN][rank*8 + file];
				board->staticScore -= pieceValues[BLACK_QUEEN];
				board->pieces[BLACK_QUEEN] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'k':
			{
				board->mailbox[rank*8 + file] = BLACK_KING;
				board->staticScore -= pieceSquareValues[BLACK_KING][rank*8 + file];
				board->staticScore -= pieceValues[BLACK_KING];
				board->pieces[BLACK_KING] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'p':
			{
				board->mailbox[rank*8 + file] = BLACK_PAWN;
				board->staticScore -= pieceSquareValues[BLACK_PAWN][rank*8 + file];
				board->staticScore -= pieceValues[BLACK_PAWN];
				board->pieces[BLACK_PAWN] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'R':
			{
				board->mailbox[rank*8 + file] = WHITE_ROOK;
				board->staticScore += pieceSquareValues[WHITE_ROOK][rank*8 + file];
				board->staticScore += pieceValues[WHITE_ROOK];
				board->pieces[WHITE_ROOK] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'N':
			{
				board->mailbox[rank*8 + file] = WHITE_KNIGHT;
				board->staticScore += pieceSquareValues[WHITE_KNIGHT][rank*8 + file];
				board->staticScore += pieceValues[WHITE_KNIGHT];
				board->pieces[WHITE_KNIGHT] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'B':
			{
				board->mailbox[rank*8 + file] = WHITE_BISHOP;
				board->staticScore += pieceSquareValues[WHITE_BISHOP][rank*8 + file];
				board->staticScore += pieceValues[WHITE_BISHOP];
				board->pieces[WHITE_BISHOP] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'Q':
			{
				board->mailbox[rank*8 + file] = WHITE_QUEEN;
				board->staticScore += pieceSquareValues[WHITE_QUEEN][rank*8 + file];
				board->staticScore += pieceValues[WHITE_QUEEN];
				board->pieces[WHITE_QUEEN] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'K':
			{
				board->mailbox[rank*8 + file] = WHITE_KING;
				board->staticScore += pieceSquareValues[WHITE_KING][rank*8 + file];
				board->staticScore += pieceValues[WHITE_KING];
				board->pieces[WHITE_KING] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case 'P':
			{
				board->mailbox[rank*8 + file] = WHITE_PAWN;
				board->staticScore += pieceSquareValues[WHITE_PAWN][rank*8 + file];
				board->staticScore += pieceValues[WHITE_PAWN];
				board->pieces[WHITE_PAWN] |= 1ULL << (rank*8 + file);
				++file;
				break;
			}
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			{
				file += *FEN - '0';
				break;
			}
			case '/':
			{
				rank -= 1;
				file = 0;
				break;
			}
			default:
			{
				return 0;
			}
		}
		++FEN;
	}
	++FEN;

	board->sides[WHITE] = board->pieces[WHITE_PAWN];
	board->sides[WHITE] |= board->pieces[WHITE_KNIGHT];
	board->sides[WHITE] |= board->pieces[WHITE_BISHOP];
	board->sides[WHITE] |= board->pieces[WHITE_ROOK];
	board->sides[WHITE] |= board->pieces[WHITE_QUEEN];
	board->sides[WHITE] |= board->pieces[WHITE_KING];

	board->sides[BLACK] = board->pieces[BLACK_PAWN];
	board->sides[BLACK] |= board->pieces[BLACK_KNIGHT];
	board->sides[BLACK] |= board->pieces[BLACK_BISHOP];
	board->sides[BLACK] |= board->pieces[BLACK_ROOK];
	board->sides[BLACK] |= board->pieces[BLACK_QUEEN];
	board->sides[BLACK] |= board->pieces[BLACK_KING];

	board->occupied = board->sides[WHITE] | board->sides[BLACK];
	board->empty = ~board->occupied;

	if (*FEN == 'w')
	{
		board->sideToMove = WHITE;
	}
	else if (*FEN == 'b')
	{
		board->sideToMove = BLACK;
	}
	else
	{
		return 0;
	}

	FEN += 2;

	while(*FEN != ' ')
	{
		switch(*FEN)
		{
			case 'k':
			{
				board->castling |= 1u << BLACK_KINGSIDE;
				break;
			}
			case 'q':
			{
				board->castling |= 1u << BLACK_QUEENSIDE;
				break;
			}
			case 'K':
			{
				board->castling |= 1u << WHITE_KINGSIDE;
				break;
			}
			case 'Q':
			{
				board->castling |= 1u << WHITE_QUEENSIDE;
				break;
			}
			case '-':
			{
				break;
			}
			default:
			{
				return 0;
			}
		}
		++FEN;
	}

	++FEN;
	if(*FEN == '-')
	{
		++FEN;
		board->enPassant = 0;
	}
	else
	{
		int rank, file;
		if ('a' <= *FEN && *FEN <= 'h')
		{
			file = *FEN - 'a';
		}
		else
		{
			return 0;
		}
		++FEN;

		if ('1' <= *FEN && *FEN <= '8')
		{
			rank = *FEN - '1';
		}
		else
		{
			return 0;
		}

		++FEN;
		board->enPassant = 1ull << (rank*8 + file);
	}
	++FEN;

	board->halfmove = atoi(FEN);

	while(*FEN++ != ' ');

	board->move = atoi(FEN);

	board->zobrist = calculate_zobrist(board);

	verify_board(board);

	return 1;
}

void print_board(Board* board)
{
	int i, rank, file;
	char strBoard[64];
	char boardPieces[NUM_PIECES] = {'p', 'P', 'n', 'N', 'b', 'B', 'r', 'R', 'q', 'Q', 'k', 'K'};
	char* sideStr[2] = {"black", "white"};
	char* castlingStr[4] = {"K", "Q", "k", "q"};
	char castlingBuf[5] = {0,0,0,0,0};
	char * enPassant = "-";

	for (i = 0; i < 64; ++i)
	{
		strBoard[i] = '.';
	}

	for (rank = 0; rank < 8; ++rank)
	{
		for (file = 0; file < 8; ++file)
		{
			for (i = 0; i < NUM_PIECES; ++i)
			{
				if (board->pieces[i] & (1ull << (rank*8 + file)))
				{
					strBoard[(7-rank)*8 + file] = boardPieces[i];
				}
			}
		}
	}

	for (i = 0; i < 64; ++i)
	{
		if (i % 8 == 0)
		{
			printf("\n");
		}
		printf("%c", strBoard[i]);
	}

	printf("\n\n");

	for(i = 0; i < 4; ++i)
	{
		if (board->castling & (1ull << i))
		{
			strcat(castlingBuf, castlingStr[i]);
		}
	}

	if (board->enPassant)
	{
		enPassant = strSquare[bit_scan_forward(board->enPassant)];
	}

	printf("move: %i, turn: %s\n", board->move, sideStr[board->sideToMove]);
	printf("castling: %s, ep-file: %s\n", castlingBuf, enPassant);
}

void log_board(Board* board)
{
	char* boardBuffer = malloc(sizeof(char)*1024);

	sprintf(boardBuffer, "{\\\"wp\\\": \\\"0x%016llx\\\", \\\"wn\\\": \\\"0x%016llx\\\", "
		"\\\"wb\\\": \\\"0x%016llx\\\", \\\"wr\\\": \\\"0x%016llx\\\", "
		"\\\"wq\\\": \\\"0x%016llx\\\", \\\"wk\\\": \\\"0x%016llx\\\", "
		"\\\"bp\\\": \\\"0x%016llx\\\", \\\"bn\\\": \\\"0x%016llx\\\", "
		"\\\"bb\\\": \\\"0x%016llx\\\", \\\"br\\\": \\\"0x%016llx\\\", "
		"\\\"bq\\\": \\\"0x%016llx\\\", \\\"bk\\\": \\\"0x%016llx\\\", "
		"\\\"ws\\\": \\\"0x%016llx\\\", \\\"bs\\\": \\\"0x%016llx\\\", "
		"\\\"oo\\\": \\\"0x%016llx\\\", \\\"ee\\\": \\\"0x%016llx\\\", "
		"\\\"ep\\\": \\\"0x%016llx\\\", \\\"si\\\": \\\"%s\\\"}",
		board->pieces[WHITE_PAWN], board->pieces[WHITE_KNIGHT], board->pieces[WHITE_BISHOP],
		board->pieces[WHITE_ROOK], board->pieces[WHITE_QUEEN], board->pieces[WHITE_KING],
		board->pieces[BLACK_PAWN], board->pieces[BLACK_KNIGHT], board->pieces[BLACK_BISHOP],
		board->pieces[BLACK_ROOK], board->pieces[BLACK_QUEEN], board->pieces[BLACK_KING],
		board->sides[WHITE], board->sides[BLACK], board->occupied, board->empty,
		board->enPassant, board->sideToMove == WHITE ? "white" : "black");
	LOG("board", boardBuffer);

	free(boardBuffer);
}

void push_state(Board* board)
{
	if (board->numHistory == board->maxHistory)
	{
		board->maxHistory *= 2;
		board->stateHistory = realloc(board->stateHistory, sizeof(BoardState)*board->maxHistory);
	}

	board->stateHistory[board->numHistory].enPassant = board->enPassant;
	board->stateHistory[board->numHistory].castling = board->castling;
	board->stateHistory[board->numHistory].halfmove = board->halfmove;

	++board->numHistory;
}

void pop_state(Board* board)
{
	--board->numHistory;

	board->enPassant = board->stateHistory[board->numHistory].enPassant;
	board->castling = board->stateHistory[board->numHistory].castling;
	board->halfmove = board->stateHistory[board->numHistory].halfmove;
}

void verify_board(Board* board)
{
#ifndef NDEBUG

	int i;
	int valid = 1;
	bitboard occupied = 0;
	bitboard whitePieces = 0, blackPieces = 0;
	bitboard pieces[NUM_PIECES + 1] = {0};

	for (i = 0; i < NUM_PIECES; ++i)
	{
		occupied |= board->pieces[i];
	}
	if (occupied != board->occupied)
	{
		fprintf(stderr, "Occupied board: %llu, calculated :%llu\n", board->occupied, occupied);
		valid = 0;
	}
	if (~occupied != board->empty)
	{
		fprintf(stderr, "Empty board: %llu, calculated :%llu\n", board->empty, ~occupied);
		valid = 0;
	}

	for (i = 0; i < NUM_PIECES; i += 2)
	{
		whitePieces |= board->pieces[i + WHITE];
		blackPieces |= board->pieces[i + BLACK];
	}
	if (whitePieces != board->sides[WHITE])
	{
		fprintf(stderr, "White board: %llu, calculated :%llu\n", board->sides[WHITE], whitePieces);
		valid = 0;
	}
	if (blackPieces != board->sides[BLACK])
	{
		fprintf(stderr, "Black board: %llu, calculated :%llu\n", board->sides[BLACK], blackPieces);
		valid = 0;
	}
	if ((board->sides[WHITE] & board->sides[BLACK]) != 0)
	{
		fprintf(stderr,
		        "Overlap of sides, White: %llu, Black: %llu, Overlap: %llu\n",
		        board->sides[WHITE],
		        board->sides[BLACK],
		        board->sides[WHITE] & board->sides[BLACK]);
		valid = 0;
	}

	if (board->zobrist != calculate_zobrist(board))
	{
		fprintf(stderr, "Zobrist board: %llu, calculated :%llu\n", board->zobrist, calculate_zobrist(board));
		valid = 0;
	}

	for (i = 0; i < 64; ++i)
	{
		pieces[board->mailbox[i]] |= 1ull << i;
	}

	for (i = 0; i < NUM_PIECES; ++i)
	{
		if (pieces[i] != board->pieces[i])
		{
			fprintf(stderr, "Piece %i board: %llu, calculated: %llu\n", i, board->pieces[i], pieces[i]);
			valid = 0;
		}
	}

	if (pieces[NUM_PIECES] != board->empty)
	{
		fprintf(stderr, "EmptyMail board: %llu, calculated: %llu\n", board->empty, pieces[NUM_PIECES]);
		valid = 0;
	}

	assert(valid);

	assert(board->staticScore == eval_full_eval(board));

	assert(population(board->pieces[WHITE_KING]) == 1);
	assert(population(board->pieces[BLACK_KING]) == 1);
#endif
}

