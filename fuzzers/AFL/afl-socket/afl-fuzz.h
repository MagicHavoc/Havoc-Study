#ifndef AFL_FUZZ_H
#define AFL_FUZZ_H

#include "types.h"

void add_to_queue(u8* fname, u32 len, u8 passed_det, u8 from_socket);
void handle_testcase(char** argv, char* fname);
int get_file_length(unsigned char* fname);

#endif