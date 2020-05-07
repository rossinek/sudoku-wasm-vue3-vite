/*
 * @author Artur Rosa
 * 2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku.h"
#include "utils.h"

const int SIZE = 9;

extern uint8_t *generateBoard(int level) {
  if (level < 0 || level > 20) {
    printf("No such level\n\n");
    return NULL;
  }

  uint8_t board[SIZE * SIZE];
  sudoku_game *sudoku = createSolvedGame();
  resetSafeRandomCells(sudoku, 30 + level);

  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      board[y * SIZE + x] = sudoku->board[y][x].value;
    }
  }
  free(sudoku);
  printf("Done!\n\n");

  uint8_t* arrayPtr = &board[0];
  return arrayPtr;
}
