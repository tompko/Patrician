#include "transposition_table.h"

typedef struct TTNode
{
	TTEntry depth_preferred;
	TTEntry always_replace;
} TTNode;

TTNode * transposition_table = NULL;
unsigned long long table_size = 0;

void init_transposition_table(unsigned long long size_in_bytes)
{
	int i = 0;
	free_transposition_table();

	table_size = size_in_bytes / sizeof(TTNode);
	transposition_table = (TTNode*)malloc(table_size * sizeof(TTNode));

	for (i = 0; i < table_size; ++i)
	{
		transposition_table[i].depth_preferred.key = 0;
		transposition_table[i].depth_preferred.depth = 0;
		transposition_table[i].always_replace.key = 0;
		transposition_table[i].always_replace.depth = 0;
	}
}

void free_transposition_table(void)
{
	free(transposition_table);
	table_size = 0;
}

void put_tt_entry(TTEntry* ttentry)
{
	unsigned long long index = ttentry->key % table_size;
	transposition_table[index].always_replace = *ttentry;
	if (ttentry->depth > transposition_table[index].depth_preferred.depth)
	{
		transposition_table[index].depth_preferred = *ttentry;
	}
}

TTEntry* get_tt_entry(unsigned long long key)
{
	unsigned long long index = key % table_size;
	// The depth_preferred will always be deeper than the always_replace
	// so if it matches return it
	if (transposition_table[index].depth_preferred.key == key)
	{
		return &transposition_table[index].depth_preferred;
	}

	if (transposition_table[index].always_replace.key == key)
	{
		return &transposition_table[index].always_replace;
	}

	return NULL;
}

TTEntry* get_tt_entry_at_depth(unsigned long long key, int depth)
{
	unsigned long long index = key % table_size;
	if (transposition_table[index].depth_preferred.key == key &&
		transposition_table[index].depth_preferred.depth == depth)
	{
		return &transposition_table[index].depth_preferred;
	}

	if (transposition_table[index].always_replace.key == key &&
		transposition_table[index].always_replace.depth == depth)
	{
		return &transposition_table[index].always_replace;
	}

	return NULL;
}

