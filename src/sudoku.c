#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "utils.h"

sudoku_game *createGame(int size) {
  sudoku_game *sudoku = (sudoku_game *)malloc(sizeof(sudoku_game));
  int board_size = size * size;
  sudoku->lookup.rows = (int *)malloc(sizeof(int) * board_size);
  sudoku->lookup.columns = (int *)malloc(sizeof(int) * board_size);
  sudoku->lookup.squares = (int *)malloc(sizeof(int) * board_size);
  for (int i = 0; i < board_size; i++) {
    resetBits(&sudoku->lookup.rows[i], 1);
    resetBits(&sudoku->lookup.columns[i], 1);
    resetBits(&sudoku->lookup.squares[i], 1);
  }
  sudoku->board = (sudoku_cell **)malloc(sizeof(sudoku_cell *) * board_size);
  for (int i = 0; i < board_size; i++) {
    sudoku->board[i] = (sudoku_cell *)malloc(sizeof(sudoku_cell) * board_size);
    for (int j = 0; j < board_size; j++) {
      sudoku->board[i][j].value = 0;
      sudoku->board[i][j].row = &sudoku->lookup.rows[i];
      sudoku->board[i][j].column = &sudoku->lookup.columns[j];
      sudoku->board[i][j].square = &sudoku->lookup.squares[(i / size) * size + (j / size)];
    }
  }
  return sudoku;
}

void cleanup(sudoku_game *sudoku, int size) {
  int board_size = size * size;
  for (int i = 0; i < board_size; i++) {
    free(sudoku->board[i]);
  }
  free(sudoku->board);
  free(sudoku->lookup.rows);
  free(sudoku->lookup.columns);
  free(sudoku->lookup.squares);
  free(sudoku);
}

sudoku_game *createSolvedGame(int size) {
  sudoku_game *sudoku = createGame(size);
  int board_size = size * size;
  assignRandomFirstRow(sudoku, size);
  for (int y = 1; y < board_size; y++) {
    int success = assignRandomAllowedRowWithReverts(sudoku, size, y, 200);
    if (success == 0) {
      cleanup(sudoku, size);
      return createSolvedGame(size);
    }
  }
  return sudoku;
}

int isBoardComplete(uint8_t *board, int size) {
  const int n = size * size * size * size;
  for (int i = 0; i < n; i++) {
    if (board[i] == 0) return 0;
  }
  return 1;
}

int isBoardValid(uint8_t *board, int size, uint8_t *validationResults) {
  const int board_size = size * size;
  int isValid = 1;
  for (int index = 0; index < board_size * board_size; index++) {
    validationResults[index] = isCellValid(board, size, index);
    isValid &= validationResults[index];
  }
  return isValid;
}

int isCellValid(uint8_t *board, int size, int index) {
  const int value = board[index];
  if (value == 0) {
    return 1;
  }
  const int board_size = size * size;
  const int x = index % board_size;
  const int y = index / board_size;
  for (int idx = y * board_size; idx < y * board_size + board_size; idx++) {
    if (idx != index && board[idx] == value) {
      return 0;
    }
  }
  for (int idx = x; idx < board_size * board_size; idx += board_size) {
    if (idx != index && board[idx] == value) {
      return 0;
    }
  }
  int offset = x - (x % size) + (y - (y % size)) * board_size;
  for (int i = 0; i < board_size; i++) {
    int idx = offset + (i % size) + (i / size) * board_size;
    if (idx != index && board[idx] == value) {
      return 0;
    }
  }
  return 1;
}

int allowedCellValues(sudoku_game *sudoku, int size, int y, int x) {
  return (*sudoku->board[y][x].row) & (*sudoku->board[y][x].column) & (*sudoku->board[y][x].square);
}

void assignRandomFirstRow(sudoku_game *sudoku, int size) {
  int board_size = size * size;
  int array[board_size];
  for (int i = 0; i < board_size; i++) {
    array[i] = i + 1;
  }

  shuffle(array, board_size);

  for (int i = 0; i < board_size; i++) {
    assignCell(sudoku, size, array[i], 0, i);
  }
}

void shuffle(int array[], int n) {
  int j, temp, repeat = n / 2 + 1;
  for (int i = 0; i < repeat; i++) {
    j = rand() % n;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

// returns 0 if failed, 1 otherwise
int assignRandomAllowedRowWithReverts(sudoku_game *sudoku, int size, int y, int tries) {
  if (tries == 0) {
    return 0;
  }
  int board_size = size * size;
  int i, it, x;
  for (it = 0; it < board_size; it++) {
    if (leastPossibleValues(sudoku, size, y, &x) == 0) {
      resetRow(sudoku, size, y);
      return assignRandomAllowedRowWithReverts(sudoku, size, y, tries - 1);
    }

    int cell_allowed = allowedCellValues(sudoku, size, y, x);

    i = rand() % board_size;
    while (getBitValue(cell_allowed, i) != 1) {
      i = (i + 1) % board_size;
    }
    assignCell(sudoku, size, i + 1, y, x);
  }
  return 1;
}

int leastPossibleValues(sudoku_game *sudoku, int size, int y, int *x) {
  int board_size = size * size;
  int i, counter, min = board_size + 1, hasFillableCell = 0;

  for (i = 0; i < board_size; i++) {
    if (sudoku->board[y][i].value == 0) {
      counter = countBits(allowedCellValues(sudoku, size, y, i), board_size);
      if (counter == 0) {
        return 0;
      }
    } else {
      counter = 0;
    }
    if (counter != 0 && counter < min) {
      min = counter;
      *x = i;
      hasFillableCell = 1;
    }
  }
  return hasFillableCell;
}

void assignCell(sudoku_game *sudoku, int size, int value, int y, int x) {
  if (value == 0) return;
  sudoku->board[y][x].value = value;
  setBitValue(sudoku->board[y][x].row, value - 1, 0);
  setBitValue(sudoku->board[y][x].column, value - 1, 0);
  setBitValue(sudoku->board[y][x].square, value - 1, 0);
}

// returns 0 if failed, 1 otherwise
int resetSafeRandomCellsWithReverts(sudoku_game *sudoku, int size, int nempty, int n, int tries) {
  if (n == 0) {
    return 1;
  }
  int board_size = size * size;
  int count, used, y[4], x[4], temp[4] = {0, 0, 0, 0};
  for (int t = 0; t < tries; t++) {
    used = 0;
    count = 0;
    do {
      x[0] = rand() % board_size;
      y[0] = rand() % board_size;
    } while (sudoku->board[y[0]][x[0]].value == 0);
    x[1] = y[0];
    y[1] = x[0];
    x[2] = board_size - 1 - x[0];
    y[2] = board_size - 1 - y[0];
    x[3] = y[2];
    y[3] = x[2];
    for (int i = 0; i < 4; i++) {
      if (i == 0 || (i < n && sudoku->board[y[i]][x[i]].value && rand() % 3 == 0)) {
        temp[i] = sudoku->board[y[i]][x[i]].value;
        resetCell(sudoku, size, y[i], x[i]);
        setBitValue(&used, i, 1);
        count++;
      }
    }
    if ((countSolutions(sudoku, size, nempty + count) == one) &&
        resetSafeRandomCellsWithReverts(sudoku, size, nempty + count, n - count, tries)) {
      return 1;
    } else {
      for (int i = 0; i < 4; i++) {
        if (getBitValue(used, i)) {
          assignCell(sudoku, size, temp[i], y[i], x[i]);
        }
      }
    }
  }
  return 0;
}

enum solutions_number countSolutions(sudoku_game *sudoku, int size, int nempty) {
  if (nempty <= 1) return one;
  int nfound = 0;
  return countSolutions2(sudoku, size, nempty, &nfound);
}

enum solutions_number countSolutions2(sudoku_game *sudoku, int size, int nempty, int *nfound) {
  if (nempty == 0) {
    *nfound += 1;
    return *nfound > 1 ? multiple : one;
  }
  if (*nfound > 1) {
    return multiple;
  }

  int board_size = size * size;
  int y = 0, x = 0, z;

  while (sudoku->board[y][x].value) {
    x++;
    if (x >= board_size) {
      y++;
      x = 0;
    }
  }

  int cell_allowed = allowedCellValues(sudoku, size, y, x);

  for (z = 0; z < board_size; z++) {
    if (getBitValue(cell_allowed, z)) {
      assignCell(sudoku, size, z + 1, y, x);
      countSolutions2(sudoku, size, nempty - 1, nfound);
      resetCell(sudoku, size, y, x);
      if (*nfound > 1) {
        return multiple;
      }
    }
  }
  return *nfound > 0 ? one : none;
}

void printBoard(sudoku_game *sudoku, int size) {
  int board_size = size * size;
  int y, x;
  for (y = 0; y < board_size; y++) {
    if (y % size == 0) printf("\n");
    for (x = 0; x < board_size; x++) {
      if (x == board_size - 1) {
        printf(" %d\n", sudoku->board[y][x].value);
      } else {
        if (x % size == 0) {
          printf("  %d ", sudoku->board[y][x].value);
        } else {
          printf(" %d ", sudoku->board[y][x].value);
        }
      }
    }
  }
  printf("\n\n");
}

void resetCell(sudoku_game *sudoku, int size, int y, int x) {
  int temp = sudoku->board[y][x].value;
  sudoku->board[y][x].value = 0;

  setBitValue(sudoku->board[y][x].row, temp - 1, 1);
  setBitValue(sudoku->board[y][x].column, temp - 1, 1);
  setBitValue(sudoku->board[y][x].square, temp - 1, 1);
}

void resetRow(sudoku_game *sudoku, int size, int r) {
  int board_size = size * size;
  for (int i = 0; i < board_size; i++) {
    resetCell(sudoku, size, r, i);
  }
}
