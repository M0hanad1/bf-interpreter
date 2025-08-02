#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool open_file(char *name, FILE **output) {
    FILE *file = fopen(name, "r");
    if (!file) {
        fprintf(stderr, "Couldn't open the bf file: \"%s\"\n", name);
        return false;
    }
    *output = file;
    return true;
}

bool read_file(FILE *file, char **output) {
    size_t buffer_size = 512;
    char *buffer = malloc(buffer_size + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for file buffer\n");
        return false;
    }

    size_t idx = 0;
    char c = 0;
    while ((c = fgetc(file)) != EOF) {
        if (idx >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                fprintf(stderr, "Failed to reallocate memory for file buffer\n");
                free(buffer);
                return false;
            }
            buffer = new_buffer;
        }
        buffer[idx++] = c;
    }
    buffer[idx] = '\0';

    if (ferror(file)) {
        fprintf(stderr, "Failed to read the file\n");
        free(buffer);
        return false;
    }
    *output = buffer;
    return true;
}
