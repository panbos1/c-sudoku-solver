/********************************************************************
                            sudoku.c
   Author: Panagiotis Bermperidis / AM: csd4376
   Assignment: Assignment 4 / HY-255

   Modes:
     sudoku              solve puzzle from stdin
     sudoku -c           check puzzle from stdin
     sudoku -g N [-u]    generate puzzle with ~N filled cells
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudoku.h"

/* ---------------- helpers: validity ---------------- */

static int used_in_row(Grid_T g, int row, int num) {
  int col;
  for (col = 0; col < 9; col++) {
    if (grid_read_value(g, row, col) == num) {
      return 1;
    }
  }
  return 0;
}

static int used_in_col(Grid_T g, int col, int num) {
  int row;
  for (row = 0; row < 9; row++) {
    if (grid_read_value(g, row, col) == num) {
      return 1;
    }
  }
  return 0;
}

static int used_in_box(Grid_T g, int br, int bc, int num) {
  int r, c;
  for (r = 0; r < 3; r++) {
    for (c = 0; c < 3; c++) {
      if (grid_read_value(g, br + r, bc + c) == num) {
        return 1;
      }
    }
  }
  return 0;
}

static int is_safe(Grid_T g, int row, int col, int num) {
  return !used_in_row(g, row, num) &&
         !used_in_col(g, col, num) &&
         !used_in_box(g, row - row % 3, col - col % 3, num);
}

/* Return 1 if row/col/box group has a duplicate among non-zero values.
   If print_pos!=0, print 1-based positions of duplicates to stderr. */
static int has_dup(int vals[9], int print_pos) {
  int j, flags[10];
  int bad;

  for (j = 0; j < 10; j++) {
    flags[j] = 0;
  }
  bad = 0;
  for (j = 0; j < 9; j++) {
    if (vals[j] == 0) {
      continue;
    }
    if (flags[vals[j]] == 0) {
      flags[vals[j]] = j + 1;
    } else {
      bad = 1;
      if (print_pos) {
        fprintf(stderr, " %d", j + 1);
      }
    }
  }
  return bad;
}

/* Fully filled and no conflicts */
static int is_complete_and_correct(Grid_T g) {
  if (grid_number_of_blanks(g) != 0) {
    return 0;
  }
  return sudoku_is_correct(g);
}

/* Find first empty cell; return 1 if found */
static int find_empty(Grid_T g, int *row, int *col) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (grid_read_value(g, i, j) == 0) {
        *row = i;
        *col = j;
        return 1;
      }
    }
  }
  return 0;
}

/* Count how many candidates 1..9 fit in empty cell (i,j) */
static int count_candidates(Grid_T g, int i, int j) {
  int n, c;
  c = 0;
  for (n = 1; n <= 9; n++) {
    if (is_safe(g, i, j, n)) {
      c++;
    }
  }
  return c;
}

/* Pick next cell: prefer unique-choice cells; else fewest candidates.
   Returns 0 if no empty cell, -1 if some empty has 0 candidates. */
static int pick_cell(Grid_T g, int *row, int *col) {
  int i, j, best_i, best_j, best_count, c, found;

  best_i = -1;
  best_j = -1;
  best_count = 10;
  found = 0;

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (grid_read_value(g, i, j) != 0) {
        continue;
      }
      found = 1;
      c = count_candidates(g, i, j);
      if (c == 0) {
        return -1;
      }
      if (c < best_count) {
        best_count = c;
        best_i = i;
        best_j = j;
        if (c == 1) {
          *row = best_i;
          *col = best_j;
          return 1;
        }
      }
    }
  }
  if (!found) {
    return 0;
  }
  *row = best_i;
  *col = best_j;
  return 1;
}

/* Recursive solver.
   only_unique: if 1, only fill cells that have exactly one candidate.
   Returns 1 on success. Sets *used_backtrack if a multi-choice branch
   was taken. */
static int solve_rec(Grid_T *g, int only_unique, int *used_backtrack) {
  int row, col, n, status;
  Grid_T backup;

  status = pick_cell(*g, &row, &col);
  if (status == 0) {
    return is_complete_and_correct(*g);
  }
  if (status < 0) {
    return 0;
  }

  if (only_unique && count_candidates(*g, row, col) != 1) {
    return 0;
  }

  for (n = 1; n <= 9; n++) {
    if (!is_safe(*g, row, col, n)) {
      continue;
    }
    if (count_candidates(*g, row, col) > 1) {
      *used_backtrack = 1;
    }
    backup = *g;
    grid_update_value(g, row, col, n);
    if (solve_rec(g, only_unique, used_backtrack)) {
      return 1;
    }
    *g = backup;
  }
  return 0;
}

/* Count number of solutions up to limit (for uniqueness checks) */
static int count_solutions(Grid_T g, int limit) {
  int row, col, n, total;
  Grid_T next;

  if (!find_empty(g, &row, &col)) {
    return sudoku_is_correct(g) ? 1 : 0;
  }
  total = 0;
  for (n = 1; n <= 9; n++) {
    if (!is_safe(g, row, col, n)) {
      continue;
    }
    next = g;
    grid_update_value(&next, row, col, n);
    total += count_solutions(next, limit - total);
    if (total >= limit) {
      return total;
    }
  }
  return total;
}

/* ---------------- public API ---------------- */

Grid_T sudoku_read(void) {
  Grid_T g;
  int i, j, n;

  memset(&g, 0, sizeof(g));
  grid_clear_unique(&g);
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (scanf("%d", &n) != 1) {
        n = 0;
      }
      grid_update_value(&g, i, j, n);
    }
  }
  return g;
}

void sudoku_print(FILE *s, Grid_T g) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      fprintf(s, "%d", grid_read_value(g, i, j));
      if (j < 8) {
        fprintf(s, " ");
      }
    }
    fprintf(s, "\n");
  }
}

void sudoku_print_errors(Grid_T g) {
  int array[9];
  int i, j;

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      array[j] = grid_read_value(g, i, j);
    }
    if (has_dup(array, 0)) {
      fprintf(stderr, "error in row %d, columns:", i + 1);
      has_dup(array, 1);
      fprintf(stderr, "\n");
    }
  }
  for (j = 0; j < 9; j++) {
    for (i = 0; i < 9; i++) {
      array[i] = grid_read_value(g, i, j);
    }
    if (has_dup(array, 0)) {
      fprintf(stderr, "error in column %d, rows:", j + 1);
      has_dup(array, 1);
      fprintf(stderr, "\n");
    }
  }
  for (i = 0; i < 9; i += 3) {
    for (j = 0; j < 9; j += 3) {
      grid_read_block(g, i, j, array);
      if (has_dup(array, 0)) {
        fprintf(stderr, "error in block (%d,%d)\n",
                (i / 3) + 1, (j / 3) + 1);
      }
    }
  }
}

int sudoku_is_correct(Grid_T g) {
  int i, j, array[9];

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      array[j] = grid_read_value(g, i, j);
    }
    if (has_dup(array, 0)) {
      return 0;
    }
  }
  for (j = 0; j < 9; j++) {
    for (i = 0; i < 9; i++) {
      array[i] = grid_read_value(g, i, j);
    }
    if (has_dup(array, 0)) {
      return 0;
    }
  }
  for (i = 0; i < 9; i += 3) {
    for (j = 0; j < 9; j += 3) {
      grid_read_block(g, i, j, array);
      if (has_dup(array, 0)) {
        return 0;
      }
    }
  }
  return 1;
}

Grid_T sudoku_solve(Grid_T g) {
  int used_backtrack;
  Grid_T work;

  work = g;
  used_backtrack = 0;
  grid_clear_unique(&work);

  /* Try unique-choice path first */
  if (solve_rec(&work, 1, &used_backtrack)) {
    grid_set_unique(&work);
    return work;
  }

  /* Full backtracking */
  work = g;
  used_backtrack = 0;
  grid_clear_unique(&work);
  if (solve_rec(&work, 0, &used_backtrack)) {
    if (!used_backtrack) {
      grid_set_unique(&work);
    }
    return work;
  }
  /* No solution: return a filled attempt (whatever we have) */
  return work;
}

int sudoku_solution_is_unique(Grid_T g) {
  return grid_read_unique(g);
}

/* Known complete valid board used as fallback seed */
static void fill_seed_board(Grid_T *g) {
  static const int seed[9][9] = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9},
    {4, 5, 6, 7, 8, 9, 1, 2, 3},
    {7, 8, 9, 1, 2, 3, 4, 5, 6},
    {2, 3, 4, 5, 6, 7, 8, 9, 1},
    {5, 6, 7, 8, 9, 1, 2, 3, 4},
    {8, 9, 1, 2, 3, 4, 5, 6, 7},
    {3, 4, 5, 6, 7, 8, 9, 1, 2},
    {6, 7, 8, 9, 1, 2, 3, 4, 5},
    {9, 1, 2, 3, 4, 5, 6, 7, 8}
  };
  int i, j;
  memset(g, 0, sizeof(*g));
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      grid_update_value(g, i, j, seed[i][j]);
    }
  }
}

/* Shuffle numbers 1..9 into out[] */
static void shuffle_1_to_9(int out[9]) {
  int i, j, tmp;
  for (i = 0; i < 9; i++) {
    out[i] = i + 1;
  }
  for (i = 8; i > 0; i--) {
    j = rand() % (i + 1);
    tmp = out[i];
    out[i] = out[j];
    out[j] = tmp;
  }
}

static Grid_T generate_full_board(void) {
  Grid_T g;
  int attempt, i, j, nums[9], n;
  int used_bt;

  for (attempt = 0; attempt < 20; attempt++) {
    memset(&g, 0, sizeof(g));
    /* Place a few random seeds then solve */
    for (i = 0; i < 9; i++) {
      shuffle_1_to_9(nums);
      for (j = 0; j < 3; j++) {
        n = nums[j];
        if (is_safe(g, i, (i * 3 + j) % 9, n)) {
          grid_update_value(&g, i, (i * 3 + j) % 9, n);
        }
      }
    }
    used_bt = 0;
    if (solve_rec(&g, 0, &used_bt) && is_complete_and_correct(g)) {
      return g;
    }
  }
  fill_seed_board(&g);
  return g;
}

Grid_T sudoku_generate(int nelts, int unique) {
  Grid_T full, puzzle, trial, solved;
  int filled, target, tries, i, j, val;
  int used_bt;
  int positions[81];
  int p, tmp, idx;

  if (nelts < 0) {
    nelts = 0;
  }
  if (nelts > 81) {
    nelts = 81;
  }

  full = generate_full_board();
  puzzle = full;
  filled = 81;
  target = nelts;

  for (p = 0; p < 81; p++) {
    positions[p] = p;
  }
  /* Fisher-Yates shuffle of cell positions */
  for (p = 80; p > 0; p--) {
    idx = rand() % (p + 1);
    tmp = positions[p];
    positions[p] = positions[idx];
    positions[idx] = tmp;
  }

  tries = 0;
  p = 0;
  while (filled > target && tries < 500) {
    if (p >= 81) {
      /* reshuffle and continue */
      for (i = 80; i > 0; i--) {
        idx = rand() % (i + 1);
        tmp = positions[i];
        positions[i] = positions[idx];
        positions[idx] = tmp;
      }
      p = 0;
    }
    i = positions[p] / 9;
    j = positions[p] % 9;
    p++;
    tries++;

    val = grid_read_value(puzzle, i, j);
    if (val == 0) {
      continue;
    }

    trial = puzzle;
    grid_update_value(&trial, i, j, 0);

    if (unique) {
      solved = trial;
      used_bt = 0;
      if (!solve_rec(&solved, 1, &used_bt) ||
          !is_complete_and_correct(solved)) {
        continue; /* keep cell: would break unique-choice solvability */
      }
    } else {
      /* Keep puzzle solvable (at least one solution) */
      if (count_solutions(trial, 1) < 1) {
        continue;
      }
    }

    puzzle = trial;
    filled--;
  }

  grid_clear_unique(&puzzle);
  if (unique) {
    grid_set_unique(&puzzle);
  }
  return puzzle;
}

/* ---------------- main / CLI ---------------- */

static void usage(const char *prog) {
  fprintf(stderr,
          "Usage:\n"
          "  %s              solve puzzle from stdin\n"
          "  %s -c           check puzzle from stdin\n"
          "  %s -g N [-u]    generate puzzle with ~N filled cells\n"
          "                  -u: unique-choice solvable\n",
          prog, prog, prog);
}

int main(int argc, char *argv[]) {
  Grid_T g, solved;
  int check_mode, gen_mode, unique, nelts, i;

  srand((unsigned)getpid());

  check_mode = 0;
  gen_mode = 0;
  unique = 0;
  nelts = 30;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      check_mode = 1;
    } else if (strcmp(argv[i], "-g") == 0) {
      gen_mode = 1;
      if (i + 1 < argc) {
        nelts = atoi(argv[++i]);
      }
    } else if (strcmp(argv[i], "-u") == 0) {
      unique = 1;
    } else {
      usage(argv[0]);
      return 1;
    }
  }

  if (gen_mode) {
    g = sudoku_generate(nelts, unique);
    fprintf(stderr, "New puzzle:\n");
    sudoku_print(stdout, g);
    return 0;
  }

  g = sudoku_read();
  fprintf(stderr, "Input puzzle:\n");
  sudoku_print(stderr, g);

  if (check_mode) {
    if (sudoku_is_correct(g) && grid_number_of_blanks(g) == 0) {
      fprintf(stderr, "Puzzle solution is correct.\n");
    } else if (sudoku_is_correct(g)) {
      fprintf(stderr, "Puzzle has no conflicts but is incomplete.\n");
    } else {
      fprintf(stderr, "Puzzle is not correct.\n");
      sudoku_print_errors(g);
    }
    return 0;
  }

  /* solve mode */
  solved = sudoku_solve(g);
  if (is_complete_and_correct(solved)) {
    if (sudoku_solution_is_unique(solved)) {
      fprintf(stderr, "Puzzle has a (unique choice) solution:\n");
    } else if (count_solutions(g, 2) > 1) {
      fprintf(stderr, "Puzzle has multiple solutions; one is:\n");
    } else {
      fprintf(stderr, "Puzzle has a solution:\n");
    }
  } else {
    fprintf(stderr, "Puzzle has no solution; examined:\n");
    sudoku_print_errors(solved);
  }
  sudoku_print(stdout, solved);
  return 0;
}
