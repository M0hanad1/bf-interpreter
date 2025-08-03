#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool remove_comments(char *input, char **output) {
    char *code = malloc(strlen(input) + 1);
    if (!code) {
        printf("Failed to allocate memory for code validate\n");
        return false;
    }

    size_t idx = 0;
    for (size_t i = 0; input[i]; i++) {
        switch (input[i]) {
            case '>':
            case '<':
            case '+':
            case '-':
            case '[':
            case ']':
            case '.':
            case ',':
                code[idx++] = input[i];
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
