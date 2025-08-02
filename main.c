#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELLS_SIZE 30000

void validate(char *input, char *output) {
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
                output[idx++] = input[j];
                break;
        }
    }
    output[idx] = '\0';
}

bool build_jumps(const char *code, size_t *jumps) {
    size_t *stack = malloc(strlen(code) / 2 * sizeof(size_t));
    size_t stack_index = 0;

    for (size_t i = 0; code[i]; i++) {
        if (code[i] == '[') {
            stack[stack_index++] = i;
        } else if (code[i] == ']') {
            if (stack_index == 0) {
                fprintf(stderr, "Unmatched closing loop bracket\n");
                free(stack);
                return false;
            }
            size_t start = stack[--stack_index];
            jumps[start] = i;
            jumps[i] = start;
        }
    }

    free(stack);
    if (stack_index == 0) return true;
    fprintf(stderr, "Unmatched opening loop bracket\n");
    return false;
}

int main() {
    char code[CELLS_SIZE] = {0};
    size_t jumps[CELLS_SIZE] = {0};
    unsigned char cells[CELLS_SIZE] = {0};
    size_t cells_idx = 0;
    validate("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.", code);
    if (!build_jumps(code, jumps)) return -1;

    for (size_t i = 0; code[i]; i++) {
        switch (code[i]) {
            case '>':
                if (cells_idx == CELLS_SIZE - 1) {
                    fprintf(stderr, "Pointer overflow: memory pointer moved after the end of the cell array.\n");
                    return -1;
                }
                cells_idx++;
                break;
            case '<':
                if (cells_idx == 0) {
                    fprintf(stderr, "Pointer underflow: memory pointer moved before the start of the cell array.\n");
                    return -1;
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
    return 0;
}
