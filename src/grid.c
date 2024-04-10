/********************************************************************
                            grid.c
   Author: Panagiotis Berberidis  /AM:        csd4376
   Assignment:   Assignment 4       /Lesson:     HY-255
*********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include "grid.h"

static void zero_memset(int *array, int size) {
  for (int j = 0; j < size; j++) {
    array[j] = 0;
  }
}

void grid_update_value(Grid_T *g, int i, int j, int n) {
  g->elts[i][j].val = n;
}

int grid_read_value(Grid_T g, int i, int j) {
  return g.elts[i][j].val;
}

void grid_set_choice(Grid_T *g, int i, int j, int n) {
  g->elts[i][j].choices.num[n] = 1;
}

void grid_clear_choice(Grid_T *g, int i, int j, int n) {
  assert(g != NULL);
  g->elts[i][j].choices.num[n] = 0;
}

int grid_choice_is_valid(Grid_T g, int i, int j, int n) {
  return g.elts[i][j].choices.num[n] == 1;
}

void grid_remove_choice(Grid_T *g, int i, int j, int n) {
  assert(g != NULL);
  if (grid_choice_is_valid(*g, i, j, n)) {
    g->elts[i][j].choices.count--;
  }
  g->elts[i][j].choices.num[n] = 0;
}

int grid_read_count(Grid_T g, int i, int j) {
  return g.elts[i][j].choices.count;
}

void grid_set_count(Grid_T *g, int i, int j) {
  assert(g != NULL);
  g->elts[i][j].choices.count = 9;
}

void grid_clear_count(Grid_T *g, int i, int j) {
  g->elts[i][j].choices.count = 0;
}

int grid_read_unique(Grid_T g) {
  return g.unique;
}

void grid_set_unique(Grid_T *g) {
  g->unique = 1;
}

void grid_clear_unique(Grid_T *g) {
  assert(g != NULL);
  g->unique = 0;
}

void grid_readBlock(int i, int j, Grid_T g, int array[9]) {
  int z;
  int k;
  int o = 0;

  i = i - i % 3;
  j = j - j % 3;
  for (z = 0; z < 3; z++) {
    for (k = 0; k < 3; k++) {
      array[o++] = grid_read_value(g, i + z, j + k);
    }
  }
}

int grid_number_of_blanks(Grid_T g) {
  int count = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (g.elts[i][j].val == 0) {
        count++;
      }
    }
  }
  return count;
}

void grid_set_all_choices(Grid_T *g, int i, int j) {
  assert(g != NULL);
  for (int o = 0; o < 10; o++) {
    g->elts[i][j].choices.num[o] = 1;
  }
}

void grid_clear_all_choices(Grid_T *g, int i, int j) {
  assert(g != NULL);
  for (int o = 0; o < 10; o++) {
    g->elts[i][j].choices.num[o] = 0;
  }
}
