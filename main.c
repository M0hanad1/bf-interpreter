#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELLS_SIZE 30000

bool validate(char *input, char **output) {
    char *code = malloc(strlen(input) + 1);
    if (!code) {
        printf("Failed to allocate memory for code validate\n");
        return false;
    }

    size_t idx = 0;
    for (size_t j = 0; input[j]; j++) {
        switch (input[j]) {
            case '>':
            case '<':
            case '+':
            case '-':
            case '[':
            case ']':
            case '.':
            case ',':
                code[idx++] = input[j];
                break;
        }
    }
    code[idx] = '\0';
    *output = code;
    return true;
}

bool build_jumps(const char *code, size_t **output) {
    size_t len = strlen(code);
    size_t *jumps = malloc(len * sizeof(size_t));
    size_t *stack = malloc(len / 2 * sizeof(size_t));
    if (!jumps || !stack) {
        fprintf(stderr, "Failed to allocate memory for jumps\n");
        free(jumps);
        free(stack);
        return false;
    }
    size_t stack_idx = 0;

    for (size_t i = 0; code[i]; i++) {
        if (code[i] == '[') {
            stack[stack_idx++] = i;
        } else if (code[i] == ']') {
            if (stack_idx == 0) {
                fprintf(stderr, "Unmatched closing loop bracket ']'\n");
                free(stack);
                free(jumps);
                return false;
            }
            size_t start = stack[--stack_idx];
            jumps[start] = i;
            jumps[i] = start;
        }
    }

    free(stack);
    if (stack_idx == 0) {
        *output = jumps;
        return true;
    }
    free(jumps);
    fprintf(stderr, "Unmatched opening loop bracket '['\n");
    return false;
}

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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No bf file provided\n");
        return -1;
    }

    FILE *file = NULL;
    char *code = NULL;
    size_t *jumps = NULL;
    size_t cells_idx = 0;
    int return_value = 0;
    unsigned char cells[CELLS_SIZE] = {0};

    if (!open_file(argv[1], &file) ||
        !read_file(file, &code) ||
        !build_jumps(code, &jumps)) {
        return_value = -1;
        goto cleanup;
    }

    for (size_t i = 0; code[i]; i++) {
        switch (code[i]) {
            case '>':
                if (cells_idx == CELLS_SIZE - 1) {
                    fprintf(stderr, "Pointer overflow: memory pointer moved after the end of the cell array.\n");
                    return_value = -1;
                    goto cleanup;
                }
                cells_idx++;
                break;
            case '<':
                if (cells_idx == 0) {
                    fprintf(stderr, "Pointer underflow: memory pointer moved before the start of the cell array.\n");
                    return_value = -1;
                    goto cleanup;
                }
                cells_idx--;
                break;
            case '+':
                if (cells[cells_idx] == 255) {
                    cells[cells_idx] = 0;
                } else {
                    cells[cells_idx]++;
                }
                break;
            case '-':
                if (cells[cells_idx] == 0) {
                    cells[cells_idx] = 255;
                } else {
                    cells[cells_idx]--;
                }
                break;
            case '[':
                if (!cells[cells_idx]) i = jumps[i];
                break;
            case ']':
                if (cells[cells_idx]) i = jumps[i];
                break;
            case '.':
                printf("%c", cells[cells_idx]);
                break;
            case ',':
                scanf(" %c", &cells[cells_idx]);
                break;
        }
    }
    goto cleanup;

cleanup:
    if (file) fclose(file);
    free(code);
    free(jumps);
    return return_value;
}
