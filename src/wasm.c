#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku.h"
#include "utils.h"

enum board_validation_result {
  valid_incomplete = -1,
  invalid = 0,
  valid_complete = 1,
};

extern uint8_t *generateBoard(int size, int level) {
  if (level < 0 || level > 20) {
    printf("No such level\n\n");
    return NULL;
  }
  int board_size = size * size;
  uint8_t board[board_size * board_size];
  sudoku_game *sudoku = createSolvedGame();
  resetSafeRandomCells(sudoku, 30 + level);

  for (int y = 0; y < board_size; y++) {
    for (int x = 0; x < board_size; x++) {
      board[y * board_size + x] = sudoku->board[y][x].value;
    }
  }
  free(sudoku);
  printf("Done!\n");

  uint8_t *arrayPtr = &board[0];
  return arrayPtr;
}

extern int validateBoard(int size, uint8_t *board) {
  if (isBoardValid(board)) {
    if (isBoardComplete(board)) {
      return valid_complete;
    }
    return valid_incomplete;
  }
  return invalid;
}
