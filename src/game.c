#include "game.h"

int set_game_from_FEN(Game* game, const char* FEN)
{
    return set_from_FEN(&game->board, FEN);
}

int perft(Game* game, int level)
{
    return 0;
}

void divide(Game* game, int level)
{
    
}