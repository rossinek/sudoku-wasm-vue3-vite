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
  int board_size = size * size;

  int min_to_remove = board_size * size;
  int max_to_remove = (int)(board_size * size * 2);
  if (level < 0 || level > 10) {
    return NULL;
  }
  int cells_to_remove = (int)(level * 0.1 * (max_to_remove - min_to_remove) + min_to_remove);
  uint8_t board[board_size * board_size];
  sudoku_game *sudoku = createSolvedGame(size);
  while (resetSafeRandomCellsWithReverts(sudoku, size, 0, cells_to_remove, 200) == 0) {
    // Do or do not. There is no try.
  }
  for (int y = 0; y < board_size; y++) {
    for (int x = 0; x < board_size; x++) {
      board[y * board_size + x] = sudoku->board[y][x].value;
    }
  }
  cleanup(sudoku, size);

  uint8_t *arrayPtr = &board[0];
  return arrayPtr;
}

extern int validateBoard(int size, uint8_t *board) {
  if (isBoardValid(board, size)) {
    if (isBoardComplete(board, size)) {
      return valid_complete;
    }
    return valid_incomplete;
  }
  return invalid;
}
