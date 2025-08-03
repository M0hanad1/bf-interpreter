#include <stdio.h>
#include <stdlib.h>

#include "build.h"
#include "file.h"
#include "interpreter.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No bf file provided\n");
        return -1;
    }

    FILE *file = NULL;
    char *raw = NULL;
    char *code = NULL;
    size_t *jumps = NULL;
    int return_value = 0;

    if (!open_file(argv[1], &file) || !read_file(file, &raw) ||
        !remove_comments(raw, &code) || !build_jumps(code, &jumps) ||
        !run(code, jumps)) {
        return_value = -1;
    }

    if (file) fclose(file);
    free(raw);
    free(code);
    free(jumps);
    return return_value;
}
