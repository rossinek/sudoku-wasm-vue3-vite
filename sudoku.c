#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "utils.h"

sudoku_game *createGame() {
  sudoku_game *sudoku = (sudoku_game *)malloc(sizeof(sudoku_game));
  for (int i = 0; i < 9; i++) {
    resetBits(&sudoku->lookup.rows[i], 1);
    resetBits(&sudoku->lookup.columns[i], 1);
    resetBits(&sudoku->lookup.squares[i], 1);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      sudoku->board[i][j].value = 0;
      sudoku->board[i][j].row = &sudoku->lookup.rows[i];
      sudoku->board[i][j].column = &sudoku->lookup.columns[j];
      sudoku->board[i][j].square = &sudoku->lookup.squares[(i / 3) * 3 + (j / 3)];
    }
  }
  return sudoku;
}

sudoku_game *createSolvedGame() {
  sudoku_game *sudoku = createGame();
  assignRandomFirstRow(sudoku);
  for (int y = 1; y < 9; y++) {
    if (y > 4 && countSolutions(sudoku, (9 - y) * 9) == none) {
      free(sudoku);
      return createSolvedGame();
    }
    assignRandomAllowedRow(sudoku, y);
  }
  return sudoku;
}

void assignRandomFirstRow(sudoku_game *sudoku) {
  int array[9];
  for (int i = 0; i < 9; i++) {
    array[i] = i + 1;
  }

  shuffle(array, 9);

  for (int i = 0; i < 9; i++) {
    assignCell(sudoku, array[i], 0, i);
  }
}

void shuffle(int array[], int n) {
  int j, temp;
  for (int i = 0; i < 5; i++) {
    j = rand() % n;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void assignRandomAllowedRow(sudoku_game *sudoku, int y) {
  int i, it, j, x;
  for (it = 0; it < 9; it++) {
    if (leastPossibleValues(sudoku, y, &x) == 0) {
      resetRow(sudoku, y);
      assignRandomAllowedRow(sudoku, y);
      return;
    }
    j = rand() % 9;

    int cell_allowed =
        (*sudoku->board[y][x].row) & (*sudoku->board[y][x].column) & (*sudoku->board[y][x].square);

    i = j;
    while (getBitValue(cell_allowed, i) != 1) {
      i = (i + 1) % 9;
    }
    assignCell(sudoku, i + 1, y, x);
  }
}

int leastPossibleValues(sudoku_game *sudoku, int y, int *x) {
  int i, counter, min = 10, hasFillableCell = 0;

  for (i = 0; i < 9; i++) {
    if (sudoku->board[y][i].value == 0) {
      counter = countBits((*sudoku->board[y][i].row) & (*sudoku->board[y][i].column) &
                          (*sudoku->board[y][i].square));
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

void assignCell(sudoku_game *sudoku, int value, int y, int x) {
  if (value == 0) return;
  sudoku->board[y][x].value = value;
  setBitValue(sudoku->board[y][x].row, value - 1, 0);
  setBitValue(sudoku->board[y][x].column, value - 1, 0);
  setBitValue(sudoku->board[y][x].square, value - 1, 0);
}

void resetSafeRandomCells(sudoku_game *sudoku, int n) {
  int i = 0, y, x, temp[4] = {0, 0, 0, 0};

  while (i < n) {
    do {
      x = rand() % 9;
      y = rand() % 9;
    } while (sudoku->board[y][x].value == 0);

    int bool = 0;
    if (sudoku->board[y][x].value) {
      temp[0] = sudoku->board[y][x].value;
      resetCell(sudoku, y, x);
      setBitValue(&bool, 0, 1);
      i++;
    }
    if (i < n && sudoku->board[x][y].value && rand() % 4 == 0) {
      temp[1] = sudoku->board[x][y].value;
      resetCell(sudoku, x, y);
      setBitValue(&bool, 1, 1);
      i++;
    }
    if (i < n && sudoku->board[8 - y][8 - x].value && rand() % 4 == 0) {
      temp[2] = sudoku->board[8 - y][8 - x].value;
      resetCell(sudoku, 8 - y, 8 - x);
      setBitValue(&bool, 2, 1);
      i++;
    }
    if (i < n && sudoku->board[8 - x][8 - y].value && rand() % 4 == 0) {
      temp[3] = sudoku->board[8 - x][8 - y].value;
      resetCell(sudoku, 8 - x, 8 - y);
      setBitValue(&bool, 3, 1);
      i++;
    }

    if (countSolutions(sudoku, i) != one) {
      if (getBitValue(bool, 0)) assignCell(sudoku, temp[0], y, x);
      if (getBitValue(bool, 1)) assignCell(sudoku, temp[1], x, y);
      if (getBitValue(bool, 2)) assignCell(sudoku, temp[2], 8 - y, 8 - x);
      if (getBitValue(bool, 3)) assignCell(sudoku, temp[3], 8 - x, 8 - y);
      i -= countBits(bool);
    }
  }
}

enum solutions_number countSolutions(sudoku_game *sudoku, int nempty) {
  if (nempty < 2) return one;
  int nfound = 0;
  return countSolutions2(sudoku, nempty, &nfound);
}

enum solutions_number countSolutions2(sudoku_game *sudoku, int nempty, int *nfound) {
  if (nempty == 0) {
    *nfound += 1;
    return *nfound > 1 ? multiple : one;
  }
  if (*nfound > 1) {
    return multiple;
  }

  int y = 0, x = 0, z;

  while (sudoku->board[y][x].value) {
    x++;
    if (x >= 9) {
      y++;
      x = 0;
    }
  }

  int cell_allowed =
      *sudoku->board[y][x].row & *sudoku->board[y][x].column & *sudoku->board[y][x].square;

  for (z = 0; z < 9; z++) {
    if (getBitValue(cell_allowed, z)) {
      assignCell(sudoku, z + 1, y, x);
      countSolutions2(sudoku, nempty - 1, nfound);
      resetCell(sudoku, y, x);
      if (*nfound > 1) {
        return multiple;
      }
    }
  }
  return *nfound > 0 ? one : none;
}

void printBoard(sudoku_game *sudoku) {
  int y, x;
  for (y = 0; y < 9; y++) {
    if (y % 3 == 0) printf("\n");
    for (x = 0; x < 9; x++) {
      if (x == 8) {
        printf(" %d\n", sudoku->board[y][x].value);
      } else {
        if (x % 3 == 0) {
          printf("  %d ", sudoku->board[y][x].value);
        } else {
          printf(" %d ", sudoku->board[y][x].value);
        }
      }
    }
  }
  printf("\n\n");
}

void resetCell(sudoku_game *sudoku, int y, int x) {
  int temp = sudoku->board[y][x].value;
  sudoku->board[y][x].value = 0;

  setBitValue(sudoku->board[y][x].row, temp - 1, 1);
  setBitValue(sudoku->board[y][x].column, temp - 1, 1);
  setBitValue(sudoku->board[y][x].square, temp - 1, 1);
}

void resetRow(sudoku_game *sudoku, int r) {
  for (int i = 0; i < 9; i++) {
    resetCell(sudoku, r, i);
  }
}
