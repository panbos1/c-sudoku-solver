# HY-255 Assignment 4: Sudoku checker, solver, and generator

**Author:** Panagiotis Bermperidis (csd4376)

## Build

```bash
make
```

Produces the `sudoku` executable. Requires `gcc`.

```bash
make clean
```

## Usage

```bash
# Solve a puzzle from stdin (prints input on stderr, solution on stdout)
./sudoku < puzzles/s.10

# Check whether a completed puzzle is correct
./sudoku -c < solved.txt

# Generate a puzzle with ~N filled cells
./sudoku -g 40

# Generate a puzzle solvable with unique-choice steps only
./sudoku -g 40 -u

# Pipeline demo (as in the assignment)
./sudoku -g 50 -u | ./sudoku | ./sudoku -c
```

## What it does

1. **Solve** — backtracking with preference for unique-choice cells; reports if the path used only unique choices or if multiple solutions exist.
2. **Check (`-c`)** — validates rows, columns and 3×3 blocks; prints error locations.
3. **Generate (`-g N [-u]`)** — builds a full valid board, then removes cells down to about `N` filled ones (optionally keeping unique-choice solvability).

## Notes

- Implemented to match the assignment CLI and behaviour for portfolio use.
- Test puzzles are under `puzzles/`.
