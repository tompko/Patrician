#ifndef SEARCH_H
#define SEARCH_H

#include "move.h"

struct Board;
struct EPDFile;

Move root_search(struct Board* game, double timeToSearch);

void search_test_search(struct EPDFile* epdFile);

#endif

