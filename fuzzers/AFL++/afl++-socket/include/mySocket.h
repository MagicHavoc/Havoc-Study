#ifndef __MYSOCKET
#define __MYSOCKET

#include "afl-fuzz.h"
void initServer();
void finalizeServer();
void initClient();
void check_and_send(afl_state_t *afl);
void wait_and_run(afl_state_t *afl);
void block_if_server(afl_state_t *afl);
void insert_queue(unsigned char* fname);
void set_time_limit(unsigned int t);
void set_iteration(unsigned int t);
void set_port(unsigned int t);

#endif