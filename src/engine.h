#ifndef ENGINE_H
#define ENGINE_H

void set_memory_limit(int megabytes);

void set_random_mode(int random);
int get_random_mode(void);

void set_ics_host(const char * hostname);
void set_ics_mode(int mode);

void set_mps(int mps);
void set_base_time(int minutes, int seconds);
void set_inc_time(int minutes, int seconds);
void set_engine_time(const char* centiseconds);
void set_opponent_time(const char* centiseconds);

void set_show_thinking(int showThinking);

void set_pondering(int ponder);

void set_opponent_name(const char * name);

void start_engine_thread(void);

void engine_new_game(void);
void engine_force_move(const char* moveString);
void engine_set_result(const char* result);
void engine_force_mode(void);
void engine_undo(void);

#endif