#ifndef SUDOKU_H
#define SUDOKU_H

typedef struct sudoku_cell {
  int wpisane;
  int *rzad;
  int *kolumna;
  int *kwadrat;
} sudoku_cell;

typedef struct allowed_values {
  int rzad[9];
  int kolumna[9];
  int kwadrat[9];
} allowed_values;

void createEmptyBoard(sudoku_cell *sudoku[9][9], allowed_values *tab);
void generateFullBoard(sudoku_cell *sudoku[9][9], allowed_values *tab);
void assignRandomFirstRow(sudoku_cell *sudoku[9][9]);
void assignRandomAllowedRow(int ktory, sudoku_cell *sudoku[9][9]) ;
void assignCell(int co, int x, int y, sudoku_cell *sudoku[9][9]);

void shuffle(int tab[], int iloscEl);

int leastPossibleValues(int ktory, sudoku_cell *sudoku[9][9], int *yw);
void resetSafeRandomCells(int ile, sudoku_cell *sudoku[9][9]);
int hasOneSolution(sudoku_cell *sudoku[9][9], int ilePustych);
void countSolutions(sudoku_cell *sudoku[9][9], int *ileRozw, int ilePustych);

void cleanup(sudoku_cell *sudoku[9][9], allowed_values *tab);
void resetCell(int x, int y, sudoku_cell *sudoku[9][9]);
void resetRow(int ktory, sudoku_cell *sudoku[9][9]);
void saveBoard(sudoku_cell *plansza[9][9], FILE *gdzie);

#endif
