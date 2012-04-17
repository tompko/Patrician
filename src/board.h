#ifndef PATRICIAN_BOARD
#define PATRICIAN_BOARD

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

enum Pieces
{
	WHITE_PAWN,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING,
	NUM_PIECES
};

enum Castling
{
    WHITE_KINGSIDE,
    WHITE_QUEENSIDE,
    BLACK_KINGSIDE,
    BLACK_QUEENSIDE
};

typedef struct
{
	bitboard pieces[NUM_PIECES];   //One bitboard for each piece type
	bitboard white;                //Combination of all white piece bitboards
	bitboard black;                //Combination of all black piece bitboards
	bitboard empty;                //All empty squares
	bitboard occupied;             //All occupied squares

    unsigned char whiteMove;        //non-zero if white has the next move
    unsigned int castling;          //Castling availability
    bitboard enPassant;             //The square to capture on if available
    unsigned int halfmove;          //The number of half moves since the last capture or pawn move
    unsigned int move;              //The number of full moves
} Board;

int set_from_FEN(Board* board, const char* FEN);

#endif