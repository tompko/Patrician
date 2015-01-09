
#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include "move.h"

typedef struct TTEntry 
{
	unsigned long long key;
	Move best_move;
	int depth;
	int score;
	int age;
} TTEntry;

void init_transposition_table(unsigned long long size_in_bytes);
void free_transposition_table(void);
void clear_transposition_table(void);

void put_tt_entry(TTEntry* ttentry);
TTEntry* get_tt_entry(unsigned long long key);			// Returns the deepest matching node
TTEntry* get_tt_entry_at_depth(unsigned long long key, int depth);	// Returns the matching node at a given depth, or NULL

#endif

