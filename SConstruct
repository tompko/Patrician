import itertools

common_files = ['board.c', 'move.c', 'game.c', 'engine.c', 'search.c',
                'gameTime.c', 'evaluation.c', 'debug_log.c', 'perft.c',
                'hashing/mt64.c', 'hashing/zobrist.c', 'hashing/transposition_table.c',
                'io/epd.c',]

Export('common_files')
SConscript(['build/debug.sconscript'])
SConscript(['build/release.sconscript'])
