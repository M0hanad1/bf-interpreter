#include <stdbool.h>
#include <stdio.h>

#define CELLS_SIZE 30000

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
