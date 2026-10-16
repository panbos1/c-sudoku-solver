/********************************************************************
                            grid.c
   Author: Panagiotis Bermperidis / AM: csd4376
   Assignment: Assignment 4 / HY-255
*********************************************************************/
#include <assert.h>
#include "grid.h"

void grid_update_value(Grid_T *g, int i, int j, int n) {
  assert(g != 0);
  assert(i >= 0 && i < 9 && j >= 0 && j < 9);
  assert(n >= 0 && n <= 9);
  g->elts[i][j].val = n;
}

int grid_read_value(Grid_T g, int i, int j) {
  assert(i >= 0 && i < 9 && j >= 0 && j < 9);
  return g.elts[i][j].val;
}

void grid_set_choice(Grid_T *g, int i, int j, int n) {
  assert(g != 0);
  assert(n >= 0 && n <= 9);
  g->elts[i][j].choices.num[n] = 1;
}

void grid_clear_choice(Grid_T *g, int i, int j, int n) {
  assert(g != 0);
  assert(n >= 0 && n <= 9);
  g->elts[i][j].choices.num[n] = 0;
}

int grid_choice_is_valid(Grid_T g, int i, int j, int n) {
  assert(n >= 0 && n <= 9);
  return g.elts[i][j].choices.num[n] == 1;
}

void grid_remove_choice(Grid_T *g, int i, int j, int n) {
  assert(g != 0);
  if (grid_choice_is_valid(*g, i, j, n)) {
    g->elts[i][j].choices.count--;
  }
  g->elts[i][j].choices.num[n] = 0;
}

int grid_read_count(Grid_T g, int i, int j) {
  return g.elts[i][j].choices.count;
}

void grid_set_count(Grid_T *g, int i, int j) {
  assert(g != 0);
  g->elts[i][j].choices.count = 9;
}

void grid_clear_count(Grid_T *g, int i, int j) {
  assert(g != 0);
  g->elts[i][j].choices.count = 0;
}

int grid_read_unique(Grid_T g) {
  return g.unique;
}

void grid_set_unique(Grid_T *g) {
  assert(g != 0);
  g->unique = 1;
}

void grid_clear_unique(Grid_T *g) {
  assert(g != 0);
  g->unique = 0;
}

void grid_read_block(Grid_T g, int i, int j, int array[9]) {
  int z, k, o;
  i = i - (i % 3);
  j = j - (j % 3);
  o = 0;
  for (z = 0; z < 3; z++) {
    for (k = 0; k < 3; k++) {
      array[o++] = grid_read_value(g, i + z, j + k);
    }
  }
}

int grid_number_of_blanks(Grid_T g) {
  int i, j, count;
  count = 0;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (grid_read_value(g, i, j) == 0) {
        count++;
      }
    }
  }
  return count;
}

void grid_set_all_choices(Grid_T *g, int i, int j) {
  int o;
  assert(g != 0);
  for (o = 1; o <= 9; o++) {
    g->elts[i][j].choices.num[o] = 1;
  }
  g->elts[i][j].choices.num[0] = 0;
  g->elts[i][j].choices.count = 9;
}

void grid_clear_all_choices(Grid_T *g, int i, int j) {
  int o;
  assert(g != 0);
  for (o = 0; o < 10; o++) {
    g->elts[i][j].choices.num[o] = 0;
  }
  g->elts[i][j].choices.count = 0;
}
