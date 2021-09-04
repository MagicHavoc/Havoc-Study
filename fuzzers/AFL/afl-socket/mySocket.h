#ifndef __MYSOCKET
#define __MYSOCKET

void initServer();
void finalizeServer();
void initClient();
void check_and_send(char** argv);
void wait_and_run(char** argv);
void block_if_server(char** argv);
void insert_queue(unsigned char* fname);
void set_time_limit(unsigned int t);
void set_iteration(unsigned int t);
void set_port(unsigned int t);

#endif