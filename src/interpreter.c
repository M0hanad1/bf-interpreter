#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELLS_SIZE 30000

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

bool run(char *code, size_t *jumps) {
    unsigned char cells[CELLS_SIZE] = {0};
    size_t cells_idx = 0;

    for (size_t i = 0; code[i]; i++) {
        switch (code[i]) {
            case '>':
                if (cells_idx == CELLS_SIZE - 1) {
                    fprintf(stderr, "Pointer overflow: memory pointer moved after the end of the cell array.\n");
                    return false;
                }
                cells_idx++;
                break;
            case '<':
                if (cells_idx == 0) {
                    fprintf(stderr, "Pointer underflow: memory pointer moved before the start of the cell array.\n");
                    return false;
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
    return true;
}
