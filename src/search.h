#ifndef SEARCH_H
#define SEARCH_H

struct Board;
struct Move;
struct EPDFile;

struct Move root_search(struct Board* game, double timeToSearch);

void search_test_search(struct EPDFile* epdFile);

#endif

