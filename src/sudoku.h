/********************************************************************
   sudoku.h

   Sudoku solver/generator interface.
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

Grid_T sudoku_read(void); /* diavazei ena sudoku apo to stdin kai epistrefei ena object Grid_T pou arxikopoiite se autes tis grammes. To input einai :
1 2 3 4 5 6 7 8 9 
4 5 6 7 8 9 1 2 3
7 8 9 1 2 3 4 5 6
2 3 4 5 6 7 8 9 1 
5 6 7 8 9 1 2 3 4
8 9 1 2 3 4 5 6 7 
3 4 5 6 7 8 9 1 2
6 7 8 9 1 2 3 4 5 
9 1 2 3 4 5 6 7 8  kathe arithmos akolouthite apo space kai kathe gramei teleiwnei me \n kai oi times me 0 einai kenes */

void sudoku_print(FILE *s, Grid_T g); /* emfanizei to sudoku puzzle  pou orizetai apo to g gia to stream s stin idia morfi apo to sudoku_print()*/

void sudoku_print_errors(Grid_T g); /* emfanizei ola ta row/col/sub-grid errors tou sudoku g */

int sudoku_is_correct(Grid_T g);  /* epistrefei true an to sudoku g einai swsto */

Grid_T sudoku_solve(Grid_T g); /* linei to sudoku g kai epistrefei to limeno sudoku se mia po tis pithanes luseis tou */

int sudoku_solution_is_unique(Grid_T g);  /* epistrefei true an to sudoku g exei mia lusi monadiki afou tha exei ferei true kai to sudoku_solve() */

Grid_T sudoku_generate(int nelts); /* dimiourgei kai epistrefei ena sudoku me "approximetly" nelts stoixeia pou dn exoun timi 0 */