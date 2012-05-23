#ifndef PATRICIAN_BOARD
#define PATRICIAN_BOARD

#include <stdio.h>

typedef unsigned long long bitboard;

enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};

extern char * strSquare[];      //The string representation for each square

extern bitboard squareFiles[];  //The file mask for each square
extern bitboard ranks[];        //The rank mask for each rank
extern bitboard backranks[];    //The back rank for each side

enum Pieces
{
    PAWN,
    BLACK_PAWN = PAWN,
    WHITE_PAWN,
    KNIGHT,
    BLACK_KNIGHT = KNIGHT,
    WHITE_KNIGHT,
    BISHOP,
    BLACK_BISHOP = BISHOP,
    WHITE_BISHOP,
    ROOK,
    BLACK_ROOK = ROOK,
    WHITE_ROOK,
    QUEEN,
    BLACK_QUEEN = QUEEN,
    WHITE_QUEEN,
    KING,
    BLACK_KING = KING,
    WHITE_KING,
    NUM_PIECES
};

enum Colours
{
    BLACK,
    WHITE,
    NUM_COLOURS
};

enum Castling
{
    WHITE_KINGSIDE,
    WHITE_QUEENSIDE,
    BLACK_KINGSIDE,
    BLACK_QUEENSIDE
};

typedef struct BoardState
{
    bitboard enPassant;             //The file to capture on if available
    unsigned int castling;          //Castling availability
    unsigned int halfmove;          //The number of half moves since the last capture or pawn move
} BoardState;

typedef struct Board
{
    bitboard pieces[NUM_PIECES];   //One bitboard for each piece type
    bitboard sides[NUM_COLOURS];   //Occupancy per side
    bitboard empty;                //All empty squares
    bitboard occupied;             //All occupied squares
    bitboard enPassant;             //The file to capture on if available

    unsigned long long zobrist;     //Zobrist hash for the board
    BoardState* stateHistory;
    unsigned int castling;          //Castling availability
    unsigned int halfmove;          //The number of half moves since the last capture or pawn move
    unsigned int numHistory;
    unsigned int maxHistory;
    unsigned int move;              //The number of full moves
    unsigned char sideToMove;       //which side has the next move
} Board;

int set_from_FEN(Board* board, const char* FEN);

void print_board(Board* board);
void log_board(Board* board);

void push_state(Board* board);
void pop_state(Board* board);

#endif
