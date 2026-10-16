/********************************************************************
   grid.h

   Grid ADT for sudoku puzzles: cell values, choice maps, unique flag.
*********************************************************************/
#ifndef GRID_H
#define GRID_H

typedef struct grid_s {
  int unique;      /* 1 if puzzle has a unique-choice solution path */
  struct {
    int val;       /* value of element i,j (0 = empty) */
    struct {
      int count;   /* number of possible choices for element i,j */
      int num[10]; /* num[k]==1 if k is a valid choice */
    } choices;
  } elts[9][9];
} Grid_T;

/* update / read cell value */
void grid_update_value(Grid_T *g, int i, int j, int n);
int grid_read_value(Grid_T g, int i, int j);

/* choice map helpers */
void grid_set_choice(Grid_T *g, int i, int j, int n);
void grid_clear_choice(Grid_T *g, int i, int j, int n);
int grid_choice_is_valid(Grid_T g, int i, int j, int n);
void grid_remove_choice(Grid_T *g, int i, int j, int n);

/* choice count helpers */
int grid_read_count(Grid_T g, int i, int j);
void grid_set_count(Grid_T *g, int i, int j);
void grid_clear_count(Grid_T *g, int i, int j);

/* unique flag helpers */
int grid_read_unique(Grid_T g);
void grid_set_unique(Grid_T *g);
void grid_clear_unique(Grid_T *g);

/* extras used by sudoku.c */
void grid_read_block(Grid_T g, int i, int j, int array[9]);
int grid_number_of_blanks(Grid_T g);
void grid_set_all_choices(Grid_T *g, int i, int j);
void grid_clear_all_choices(Grid_T *g, int i, int j);

#endif
