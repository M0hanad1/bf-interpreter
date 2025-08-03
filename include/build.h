#ifndef _INC_BUILD
#define _INC_BUILD

#include <stdbool.h>
#include <stdio.h>

bool remove_comments(char *input, char **output);
bool build_jumps(const char *code, size_t **output);

#endif
