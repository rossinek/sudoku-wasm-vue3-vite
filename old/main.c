/*
 * Artur Rosa
 *
 * SUDOKU cz.1 - GENEROWANIE
 *
 * WYWOLANIE PROGRAMU:
 *
 * Jako drugi argument wywolania programu wpisujemy sciezke pliku w ktorym zapiszemy utworzone plansze sudoku.
 *
 * OPIS METODY generowania planszy sudoku:
 *
 * Na poczatku generuje pelna plansze sudoku rzad po rzedzie,
 * sprawdzajac przed wstawieniem nowego rzedu czy plansza ma w ogole rozwiazanie.
 * Jesli nie generuje plansze od nowa.
 *
 * Nastepnie usuwam z planszy zadana ilosc pol symetrycznie wzgledem przekatnych
 * sprawdzajac przy tym czy plansza ma dokladnie jedno rozwiazanie.
 * Jesli nie wycofuje sie wstawiajac wczesniej usuniete pola i usuwam inne.
 *
 * Do sprawdzenia czy plansza ma w ogole rozwiazania oraz czy ma dokladnie jedno
 * uzywam rekurencyjnego algorytmu z nawrotami.
 *
 * STRUKTURY:
 *
 * Kazde pole w dwuwymiarowej tablicy reprezentujacej plansze zawiera:
 * - wartosc wpisana w dane pole
 * - wskazniki do trzech liczb calkowitych: rzad, kolumna, kwadrat,
 *   ktorych i-ty bit oznacza mozliwosc wpisania i+1 w kolejno rzedzie,
 *   kolumnie i kwadracie w ktorym znajduje sie dane pole
 * Druga struktura przetrzymuje w trzech tablicach liczby calkowite.
 * i-ty bit k-tej liczby z tablicy rzad, kolumna, kwadrat
 * oznacza mozliwosc wpisania i+1 w kolejno k-tym rzedzie, kolumnie i kwadracie.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku.h"
#include "utils.h"

int main (int argc, const char * argv[])
{
  if(argc!=2) {
    fprintf(stderr, "Zle podane do programu argumenty\n");
    return 0;
  }

  FILE *plik=NULL;
  plik=fopen(argv[1],"w");
  if(!plik) {
    fprintf(stderr, "Nie udalo sie utworzyc pliku\n");
    return 0;
  }

  fprintf(stdout, "GENERATOR SUDOKU\n");

  sudoku_cell *sudoku[9][9];
  allowed_values *tab=NULL;
  char wybor[7];
  int trudnosc, boolPl=0, ok;

  while(1){
    fprintf(stdout,"Aby utworzyc plansze napisz 'utworz'\nAby zakonczyc napisz 'zakoncz'\n");
    fscanf(stdin, "%s", wybor);
    if(strcmp(wybor, "zakoncz")==0) {
      if(boolPl) cleanup(sudoku,tab);
      fclose(plik);
      fprintf(stdout, "Do widzenia!\n");
      return 0;
    }
    if(strcmp(wybor, "utworz")==0) {
      fprintf(stdout, "Wybierz poziom trudnosci w skali od 0 do 20: ");
      ok=fscanf(stdin, "%d", &trudnosc);
      if(ok==0||trudnosc<0||trudnosc>20) {
        fprintf(stdout, "Blednie podany poziom trudnosci\n\n");
      }
      else {
        createEmptyBoard(sudoku,tab);
        generateFullBoard(sudoku,tab);
        resetSafeRandomCells(30+trudnosc, sudoku);
        saveBoard(sudoku, plik);
        fprintf(stdout, "Gotowe!\n\n");
        boolPl=1;
      }
    }
    else
      fprintf(stdout, "Nie ma takiej opcji, sprobuj jeszcze raz\n\n");
  }
}
