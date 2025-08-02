#ifndef _INC_FILE
#define _INC_FILE

#include <stdbool.h>
#include <stdio.h>

bool open_file(char *name, FILE **output);
bool read_file(FILE *file, char **output);

#endif
