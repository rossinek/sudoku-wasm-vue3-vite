#ifndef SUDOKU_H
#define SUDOKU_H

typedef struct sudoku_cell {
  int value;
  int *row;
  int *column;
  int *square;
} sudoku_cell;

typedef struct allowed_values {
  int rows[9];
  int columns[9];
  int squares[9];
} allowed_values;

typedef struct sudoku_game {
  sudoku_cell board[9][9];
  allowed_values lookup;
} sudoku_game;

enum solutions_number {
  none = 0,
  one = 1,
  multiple = 2,
};

sudoku_game *createGame();
sudoku_game *createSolvedGame();
void assignRandomFirstRow(sudoku_game *sudoku);
void assignRandomAllowedRow(sudoku_game *sudoku, int r);
void assignCell(sudoku_game *sudoku, int value, int y, int x);
void shuffle(int array[], int n);
void resetSafeRandomCells(sudoku_game *sudoku, int n);
void resetCell(sudoku_game *sudoku, int y, int x);
void resetRow(sudoku_game *sudoku, int r);
int leastPossibleValues(sudoku_game *sudoku, int y, int *x);

enum solutions_number countSolutions(sudoku_game *sudoku, int nempty);
enum solutions_number countSolutions2(sudoku_game *sudoku, int nempty, int *nfound);

void printBoard(sudoku_game *sudoku);

#endif
