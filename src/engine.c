#include "engine.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "search.h"

enum EngineState
{
	ENGINE_OBSERVING,
	ENGINE_WAITING,
	ENGINE_THINKING,
	ENGINE_PONDERING,
	ENGINE_ANALYZING,
	ENGINE_QUIT,
};

typedef struct GameMoves
{
	Move* moves;
	int numMoves;
	int maxMoves;
} GameMoves;

static Board s_CurrentBoard;
static GameMoves s_GameMoves;

static char* s_ICSHost = NULL;
static char* s_OpponentName = NULL;
static enum EngineState s_EngineState;
static int s_RandomEvaluation = 0;
static int s_ICSPlay = 0;
static int s_ShowThinking = 0;
static int s_CanPonder = 0;

void engine_thread(void);
void add_move_to_stack(Move move);

#ifdef _MSC_VER
HANDLE EngineThreadHandle;

DWORD WINAPI engine_thread_wrapper(LPVOID lpParameter)
{
	UNUSED(lpParameter);
	engine_thread();
	return 0;
}
#else
pthread_t EngineThread;

void* engine_thread_wrapper(void* ptr)
{
	engine_thread();
	return 0;	
}
#endif

void set_memory_limit(int megabytes)
{
	UNUSED(megabytes);
}

void set_random_mode(int random)
{
	s_RandomEvaluation = random;
}

int get_random_mode()
{
	return s_RandomEvaluation;
}

void set_ics_host(const char * hostname)
{
	free(s_ICSHost);
	s_ICSHost = (char*)malloc(sizeof(char)*(strlen(hostname) + 1));
	strcpy(s_ICSHost, hostname);
}

void set_ics_mode(int mode)
{
	s_ICSPlay = mode;
}

void set_mps(int mps)
{
	//Do nothing until we implement time controls and management
	UNUSED(mps);
}

void set_base_time(int minutes, int seconds)
{
	//Do nothing until we implement time controls and management
	UNUSED(minutes); UNUSED(seconds);
}

void set_inc_time(int minutes, int seconds)
{
	//Do nothing until we implement time controls and management
	UNUSED(minutes); UNUSED(seconds);
}

void set_engine_time(const char* centiseconds)
{
	//Do nothing until we implement time controls and management
	UNUSED(centiseconds);
}
void set_opponent_time(const char* centiseconds)
{
	//Do nothing until we implement time controls and management
	UNUSED(centiseconds);
}

void set_show_thinking(int showThinking)
{
	s_ShowThinking = showThinking;
}

void set_pondering(int ponder)
{
	//We don't support pondering at the moment, so ignore this command
	//and set can ponder to false
	UNUSED(ponder);
	s_CanPonder = 0;//ponder;
}

void set_opponent_name(const char * name)
{
	free(s_OpponentName);
	s_OpponentName = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(s_OpponentName, name);
}

void start_engine_thread()
{
	s_EngineState = ENGINE_OBSERVING;
	s_GameMoves.moves = malloc(sizeof(Move)*60);
	s_GameMoves.numMoves = 0;
	s_GameMoves.maxMoves = 60;

#ifdef _MSC_VER
	EngineThreadHandle = CreateThread(NULL, 0, engine_thread_wrapper, NULL, 0, NULL);
#else
	int ret = pthread_create(&EngineThread, NULL, engine_thread_wrapper, NULL);
#endif
}

void engine_new_game()
{
	set_from_FEN(&s_CurrentBoard, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	s_EngineState = ENGINE_WAITING;
}

void engine_force_move(const char* moveString)
{
	Move move =  make_move_from_str(&s_CurrentBoard, moveString);
	log_board(&s_CurrentBoard);
	log_move(move, moveString, "usermove");
	make_move(&s_CurrentBoard, &move);
	add_move_to_stack(move);
	log_board(&s_CurrentBoard);
	
	if(s_EngineState == ENGINE_WAITING)
	{
		s_EngineState = ENGINE_THINKING;
	}
}

void engine_set_result(const char* result)
{
	UNUSED(result);
}

void engine_force_mode()
{
	s_EngineState = ENGINE_OBSERVING;
}

void engine_undo(void)
{
	//Stop the engine thinking or pondering, we'll start it again after undoing the move
	enum EngineState currState = s_EngineState;
	s_EngineState = ENGINE_WAITING;

	unmake_move(&s_CurrentBoard, s_GameMoves.moves[--s_GameMoves.numMoves]);

	if(currState == ENGINE_THINKING)
	{
		if(s_CanPonder)
		{
			s_EngineState = ENGINE_PONDERING;
		}
		else
		{
			s_EngineState = ENGINE_WAITING;
		}
	}
	else if(currState == ENGINE_WAITING || currState == ENGINE_PONDERING)
	{
		s_EngineState = ENGINE_THINKING;
	}
	else
	{
		s_EngineState = currState;
	}
}

void engine_go(void)
{
	s_EngineState = ENGINE_THINKING;
}

void engine_thread(void)
{
	while(s_EngineState != ENGINE_QUIT)
	{
		switch(s_EngineState)
		{
			case ENGINE_OBSERVING:
			{
				break;
			}
			case ENGINE_WAITING:
			{
				break;
			}
			case ENGINE_THINKING:
			{
				Move move = root_search(&s_CurrentBoard);
				char moveBuffer[16];

				if (s_EngineState != ENGINE_THINKING)
				{
					//Our thinking has been interrupted
					break;
				}

				make_move(&s_CurrentBoard, &move);
				log_move(move, "", "enginemove");
				add_move_to_stack(move);
				sprint_move(moveBuffer, move);
				printf("move %s\n", moveBuffer);

				if(s_CanPonder)
				{
					s_EngineState = ENGINE_PONDERING;
				}
				else
				{
					s_EngineState = ENGINE_WAITING;
				}
				break;
			}
			case ENGINE_PONDERING:
			{
				break;
			}
			case ENGINE_ANALYZING:
			{
				break;
			}
		}
	}
}

void add_move_to_stack(Move move)
{
	if(s_GameMoves.numMoves == s_GameMoves.maxMoves)
	{
		s_GameMoves.moves = realloc(s_GameMoves.moves, sizeof(Move)*s_GameMoves.maxMoves*2);
		s_GameMoves.maxMoves *= 2;
	}

	s_GameMoves.moves[s_GameMoves.numMoves++] = move;
}
