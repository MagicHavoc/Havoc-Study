#ifndef AFL_FUZZ_H
#define AFL_FUZZ_H

#include "types.h"
#include "afl-fuzz.h"

void handle_testcase(afl_state_t *afl, char* fname);
int get_file_length(unsigned char* fname);

#endif