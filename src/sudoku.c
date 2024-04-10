
/********************************************************************
                            sudoku.c
   Author: Panagiotis Berberidis  /AM:        csd4376
   Assigment:   Assigment 4       /Lesson:     HY-255
*********************************************************************/#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

Grid_T sudoku_read(void) {
    int i = 0, j = 0;
    int n;
    Grid_T *g = malloc(sizeof(Grid_T));
    grid_set_unique(g);
    while (i < 9) {
        if (j == 9) {
            i++;
            j = 0;
            continue;
        }
        scanf("%d", &n);
        grid_update_value(g, i, j, n);
        j++;
    }
    return *g;
}

void sudoku_print(FILE *s, Grid_T g) {
    int i;
    int j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fprintf(s, "%d ", grid_read_value(g, i, j));
        }
        fprintf(s, "\n");
    }
}

int checkDupl(int row[9], int print) {
    int j, flags[10] = {0};

    for (j = 0; j < 9; j++) {
        if (row[j] == 0) {
            if (print) {
                printf(" %d", j);
                continue;
            }
            return 1;
        }
        if (flags[row[j]] == 0) {
            flags[row[j]] = 1;
        } else {
            flags[0] = 1;
            if (print) {
                printf(" %d", j + 1);
            }
        }
    }
    return flags[0];
}

void sudoku_print_errors(Grid_T g) {
    int array[9];
    int i;
    int j;

    if (sudoku_is_correct(g)) {
        printf("To sudoku einai swsto den eixes kapoio lathos\n");
        return;
    }
    printf("lathakia:");
    /* elegxei ta rows */
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            array[j] = grid_read_value(g, i, j);
        if (checkDupl(array, 0))
            printf("\nIn row %d and column", i + 1);
        checkDupl(array, 1);
    }
    /* elegxei ta columns */
    for (j = 0; j < 9; j++) {
        for (i = 0; i < 9; i++)
            array[i] = grid_read_value(g, i, j);
        if (checkDupl(array, 0)) {
            printf("\nIn column %d and row", j + 1);
            checkDupl(array, 1);
        }
    }
    /* elegxei ta Blocks */
    for (i = 0; i < 9; i += 3) {
        for (j = 0; j < 9; j += 3) {
            grid_readBlock(i, j, g, array);
            if (checkDupl(array, 0))
                printf("\nBlock %d", i + 1 + (j / 3));
        }
    }
    printf("\n");
}

int sudoku_is_correct(Grid_T g) {
    int i, j, array[9];
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            array[j] = grid_read_value(g, i, j);
        }
        if (checkDupl(array, 0)) {
            return 0;
        }
        for (j = 0; j < 9; j++) {
            array[j] = grid_read_value(g, j, i);
        }
        if (checkDupl(array, 0)) {
            return 0;
        }
    }

    for (i = 0; i < 9; i += 3) {
        for (j = 0; j < 9; j += 3) {
            grid_readBlock(i, j, g, array);
            if (checkDupl(array, 0)) {
                return 0;
            }
        }
    }
    return 1;
}

Grid_T sudoku_solve(Grid_T g) {
    int row, col;
    /* If there is no unassigned location, we are done */
    if (!sudoku_find_unassigned_location(g, &row, &col))
        return g; // success!
    /* Consider digits 1 to 9 */
    for (int num = 1; num <= 9; num++) {
        /* If placing digit in current position is safe */
        if (sudoku_is_safe(g, row, col, num)) {
            /* Make tentative assignment */
            grid_update_value(&g, row, col, num);
            /* Return, if success, yay! */
            if (sudoku_solve(g))
                return g;
            /* If failure, unmake & try again */
            grid_update_value(&g, row, col, 0);
        }
    }
    /* This triggers backtracking */
    return g;
}

/* Helper function to find an unassigned location in Sudoku grid */
int sudoku_find_unassigned_location(Grid_T g, int *row, int *col) {
    for (*row = 0; *row < 9; (*row)++) {
        for (*col = 0; *col < 9; (*col)++) {
            if (grid_read_value(g, *row, *col) == 0)
                return 1; // unassigned location found
        }
    }
    return 0; // no unassigned location found
}

/* Helper function to check if a given digit can be placed in a given row,col of Sudoku grid */
int sudoku_is_safe(Grid_T g, int row, int col, int num) {
    /* Check if 'num' is not already placed in current row, current column and current 3x3 grid */
    return !sudoku_used_in_row(g, row, num) &&
           !sudoku_used_in_col(g, col, num) &&
           !sudoku_used_in_box(g, row - row % 3, col - col % 3, num);
}

/* Helper function to check if 'num' is already used in current row of Sudoku grid */
int sudoku_used_in_row(Grid_T g, int row, int num) {
    for (int col = 0; col < 9; col++) {
        if (grid_read_value(g, row, col) == num)
            return 1; // num is already used in row
    }
    return 0; // num is not used in row
}

/* Helper function to check if 'num' is already used in current column of Sudoku grid */
int sudoku_used_in_col(Grid_T g, int col, int num) {
    for (int row = 0; row < 9; row++) {
        if (grid_read_value(g, row, col) == num)
            return 1; // num is already used in column
    }
    return 0; // num is not used in column
}

/* Helper function to check if 'num' is already used in current 3x3 box of Sudoku grid */
int sudoku_used_in_box(Grid_T g, int box_start_row, int box_start_col, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (grid_read_value(g, row + box_start_row, col + box_start_col) == num)
                return 1; // num is already used in 3x3 box
        }
    }
    return 0; // num is not used in 3x3 box
}

int main() {
    Grid_T sudoku_grid = sudoku_read(); // Read Sudoku grid from input
    sudoku_grid = sudoku_solve(sudoku_grid); // Solve Sudoku
    sudoku_print(stdout, sudoku_grid); // Print solved Sudoku grid
    sudoku_print_errors(sudoku_grid); // Print any errors in Sudoku grid
    return 0;
}
