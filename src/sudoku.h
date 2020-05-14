#ifndef SUDOKU_H
#define SUDOKU_H

typedef struct sudoku_cell {
  int value;
  int *row;
  int *column;
  int *square;
} sudoku_cell;

typedef struct allowed_values {
  int *rows;
  int *columns;
  int *squares;
} allowed_values;

typedef struct sudoku_game {
  sudoku_cell **board;
  allowed_values lookup;
} sudoku_game;

enum solutions_number {
  none = 0,
  one = 1,
  multiple = 2,
};

sudoku_game *createGame(int size);
void cleanup(sudoku_game *sudoku, int size);
sudoku_game *createSolvedGame(int size);
int isBoardComplete(uint8_t *board, int size);
int isBoardValid(uint8_t *board, int size, uint8_t *validationResults);
int isCellValid(uint8_t *board, int size, int index);
int allowedCellValues(sudoku_game *sudoku, int size, int y, int x);
void assignRandomFirstRow(sudoku_game *sudoku, int size);
int assignRandomAllowedRowWithReverts(sudoku_game *sudoku, int size, int r, int tries);
void assignCell(sudoku_game *sudoku, int size, int value, int y, int x);
void shuffle(int array[], int n);
int resetSafeRandomCellsWithReverts(sudoku_game *sudoku, int size, int nempty, int n, int tries);
void resetCell(sudoku_game *sudoku, int size, int y, int x);
void resetRow(sudoku_game *sudoku, int size, int r);
int leastPossibleValues(sudoku_game *sudoku, int size, int y, int *x);
enum solutions_number countSolutions(sudoku_game *sudoku, int size, int nempty);
enum solutions_number countSolutions2(sudoku_game *sudoku, int size, int nempty, int *nfound);

void printBoard(sudoku_game *sudoku, int size);

#endif
