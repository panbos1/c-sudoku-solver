/********************************************************************
   sudoku.h

   Sudoku checker / solver / generator interface (HY-255 Assignment 4)
*********************************************************************/
#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include "grid.h"

Grid_T sudoku_read(void);
void sudoku_print(FILE *s, Grid_T g);
void sudoku_print_errors(Grid_T g);
int sudoku_is_correct(Grid_T g);

/* Solve puzzle; sets unique flag if solved only with unique choices */
Grid_T sudoku_solve(Grid_T g);

/* True if last solve used only unique-choice steps */
int sudoku_solution_is_unique(Grid_T g);

/* Generate a puzzle with about nelts filled cells.
   If unique!=0, puzzle is solvable with unique-choice steps. */
Grid_T sudoku_generate(int nelts, int unique);

#endif
