#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "utils.h"

void createEmptyBoard(sudoku_cell *sudoku[9][9], allowed_values *tab)
/*
 * Argumentami funkcji sa dwuwymiaowa tablica pol,
 * oraz wskaznik na tablice ktore beda opisywaly utworzona plansze.
 * Funkcja tworzy pola o wartosci wpisanej rownej 0
 * oraz ustawia wartosci bitow w tablicach na 1.
 */
{
  tab=(allowed_values*)malloc(sizeof(allowed_values));

  int i,j;

  for(i=0;i<9;i++){
    resetBits(&tab->rzad[i], 1);
    resetBits(&tab->kolumna[i], 1);
    resetBits(&tab->kwadrat[i], 1);
  }
  for(i=0;i<9;i++)
    for(j=0;j<9;j++){
      sudoku[i][j]=(sudoku_cell*)malloc(sizeof(sudoku_cell));
      sudoku[i][j]->wpisane=0;
      sudoku[i][j]->rzad=&tab->rzad[i];
      sudoku[i][j]->kolumna=&tab->kolumna[j];
      sudoku[i][j]->kwadrat=&tab->kwadrat[(i/3)*3+j/3];
    }
}

void generateFullBoard(sudoku_cell *sudoku[9][9], allowed_values *tab)
/*
 * Argumentami funkcji sa plansza sudoku
 * oraz tablice opisujace co mozna wpisac w rzedach, kolumnach i kwadratach.
 * Funkcja generuje dowolny pierwszy rzad, generuje pozniej 3 poprawne rzedy,
 * a nastepnie przed wygenerowaniem kolejnego wiersza sprawdza czy w ogole jest jeszcze jakies rozwiazanie.
 * Jezli nie ma, tworzy plansze od poczatku.
 */
{
  assignRandomFirstRow(sudoku);

  int i;
  for(i=1;i<9;i++){
    if(i>4&&hasOneSolution(sudoku, (9-i)*9)==2){
      cleanup(sudoku,tab);
      createEmptyBoard(sudoku,tab);
      return generateFullBoard(sudoku,tab);
    }
    assignRandomAllowedRow(i, sudoku);
  }
}

void assignRandomFirstRow(sudoku_cell *sudoku[9][9])
/*
 * Argumentem funkcji jest plansza sudoku.
 * Funkcja generuje dowolny pierwszy rzad.
 */
{
  int i;

  int losujZ[9];
  for(i=0;i<9;i++)
    losujZ[i]=i+1;

  shuffle(losujZ,9);

  for(i=0;i<9;i++)
    assignCell(losujZ[i],0,i,sudoku);
}

void shuffle(int tab[], int iloscEl)
/*
 * Argumentami funkcji sa tablica, oraz ilosc jej elementow.
 * Funkcja miesza elementy tablicy.
 */
{
  int i,j,temp;
  for (i=0; i<5; i++){
    j = rand()%iloscEl;
    temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
  }
}

void assignRandomAllowedRow(int ktory, sudoku_cell *sudoku[9][9])
/*
 * Argumentami funkcji sa numer rzedu oraz plansza sudoku.
 * Funkcja wpisuje dowolna cyfre (ktora moze wpisac) w polach danego rzedu w kolejnosci od pola do ktorego mozna wpisac najmniej cyfr.
 * Jesli przed wpisaniem wszystkich 9ciu pol juz nie moze nigdzie wpisac cyfry, zeruje rzad i generuje go ponownie.
 */
{
  int i,it,j,y0;

  for(it=0;it<9;it++){
    if(leastPossibleValues(ktory, sudoku, &y0)==0){
      resetRow(ktory, sudoku);
      assignRandomAllowedRow(ktory, sudoku);
      return;
    }
    j=rand()%9;

    int coMoznaWpisac=*sudoku[ktory][y0]->rzad&*sudoku[ktory][y0]->kolumna&*sudoku[ktory][y0]->kwadrat;

    for(i=j;getBitValue(coMoznaWpisac, i)!=1;i=(i+1)%9);
    assignCell(i+1,ktory,y0,sudoku);
  }
}

int leastPossibleValues(int ktory, sudoku_cell *sudoku[9][9], int *yw)
/*
 * Argumentami funkcji sa numer rzedu, plansza sudoku oraz wskaznik na liczbe calkowita.
 * Funkcja sprawdza w ktore pole rzedu mozna wpisac najmniej cyfr i zapisuje numer do liczby wskazywanej przez wskaznik.
 * Jezli w zadne pole nie da sie nic wpisac funkcja zwroci 0, wpp 1.
 */
{
  int i,counter,min=10,zamiana=0;

  for(i=0;i<9;i++){
    if(sudoku[ktory][i]->wpisane==0) counter=countBits(*sudoku[ktory][i]->rzad&*sudoku[ktory][i]->kolumna&*sudoku[ktory][i]->kwadrat);
    else counter=0;
    if(counter!=0&&counter<min) {
      min=counter;
      *yw=i;
      zamiana=1;
    }
  }
  return zamiana;
}

void assignCell(int co, int x, int y, sudoku_cell *sudoku[9][9])
/*
 * Argumentami funkcji sa wartosc, ktora ma zostac wpisana w pole okreslone kolejnymi dwoma argumentami
 * oraz plansza sudoku zawierajaca to pole.
 * Funkcja wpisuje wartosc w pole oraz zapisuje ze tej wartosci nie mozna juz wpisac w rzedzie kolumnie ani kwadracie w ktorym znajduje sie to pole.
 */
{
  if(co==0) return;
  sudoku[x][y]->wpisane=co;
  setBitValue(sudoku[x][y]->rzad, co-1, 0);
  setBitValue(sudoku[x][y]->kolumna, co-1, 0);
  setBitValue(sudoku[x][y]->kwadrat, co-1, 0);
}

void resetSafeRandomCells(int ile, sudoku_cell *sudoku[9][9])
/*
 * Argumentami funkcji sa ilosc pol do usuniecia, oraz plansza sudoku z ktorej maja byc one usuniete.
 * Funkcja usuwa z planszy zadana ilosc pol symetrycznie wzgledem przekatnych
 * sprawdzajac przy tym czy plansza ma dokladnie jedno rozwiazanie.
 * Jesli nie, wycofuje sie wstawiajac wczesniej usuniete pola i usuwa inne.
 */
{
  int i=0,x,y,temp[4]={0,0,0,0};

  while(i<ile){
    for(x=rand()%9,	y=rand()%9; sudoku[x][y]->wpisane==0; x=rand()%9, y=rand()%9);
    int bool=0;

    if(sudoku[x][y]->wpisane)
    {
      temp[0]=sudoku[x][y]->wpisane;
      resetCell(x, y, sudoku);
      setBitValue(&bool, 0, 1);
      i++;
    }
    if(sudoku[y][x]->wpisane&&i<ile)
    {
      temp[1]=sudoku[y][x]->wpisane;
      resetCell(y, x, sudoku);
      setBitValue(&bool, 1, 1);
      i++;
    }
    if(sudoku[8-x][8-y]->wpisane&&i<ile)
    {
      temp[2]=sudoku[8-x][8-y]->wpisane;
      resetCell(8-x, 8-y, sudoku);
      setBitValue(&bool, 2, 1);
      i++;
    }
    if(sudoku[8-y][8-x]->wpisane&&i<ile)
    {
      temp[3]=sudoku[8-y][8-x]->wpisane;
      resetCell(8-y, 8-x, sudoku);
      setBitValue(&bool, 3, 1);
      i++;
    }

    if(hasOneSolution(sudoku, i)!=1) {
      if(getBitValue(bool, 0)) assignCell(temp[0], x, y, sudoku);
      if(getBitValue(bool, 1)) assignCell(temp[1], y, x, sudoku);
      if(getBitValue(bool, 2)) assignCell(temp[2], 8-x, 8-y, sudoku);
      if(getBitValue(bool, 3)) assignCell(temp[3], 8-y, 8-x, sudoku);
      i-=countBits(bool);
    }

  }

}

int hasOneSolution(sudoku_cell *sudoku[9][9], int ilePustych)
/*
 * Argumentami funkcji sa plansza sudoku,
 * oraz liczba pol ktorych wartosc wpisana = 0.
 * Funkcja zwraca 1 jezeli jest dokladnie jedno rozwiazanie,
 * 2 gdy nie ma rozwiazan
 * 0 gdy jest wiecej niz jedno rozwiazanie.
 */
{
  if(ilePustych<2) return 1;

  int ile=0;
  countSolutions(sudoku, &ile, ilePustych);

  if(ile==1) return 1;
  if(ile==0) return 2;
  return 0;
}

void countSolutions(sudoku_cell *sudoku[9][9], int *ileRozw, int ilePustych)
/*
 * Argumentami funkcji sa plansza sudoku,
 * wskaznik na liczbe calkowita ileRozw ktora przed wywolaniem funkcji powinna byc = 0
 * oraz ilosc pozostalych pol do wpisania.
 * Funkcja liczy ilosc rozwiazan sudoku, rekurencyjna metoda z nawrotami, zapisujac wynik w ileRozw.
 * Jesli jest wiecej niz jedno rozwiazanie funkcja przerywa dzialanie.
 */
{
  if(ilePustych==0) { *ileRozw+=1; return; }
  if(*ileRozw>1) return;

  int i=0,j=0,k;

  while(sudoku[i][j]->wpisane){
    j++;
    if(j==9) {
      i++;
      j=0;
    }
  }

  int coMoznaWpisac=*sudoku[i][j]->rzad&*sudoku[i][j]->kolumna&*sudoku[i][j]->kwadrat;

  for(k=0;k<9;k++)
    if(getBitValue(coMoznaWpisac, k)){
      assignCell(k+1, i, j, sudoku);
      countSolutions(sudoku, ileRozw, ilePustych-1);
      resetCell(i, j, sudoku);
      if(*ileRozw>1)return;
    }
}

void saveBoard(sudoku_cell *plansza[9][9], FILE *gdzie)
/*
 * Argumentami funkcji sa plansza sudoku,
 * oraz wskaznik na plik w ktorym ma wypisac plansze.
 * Funkcja wypisuje plansze sudoku w pliku.
 */
{
  int i,j;
  for(i=0;i<9;i++){
    if(i%3==0) fprintf(gdzie, "\n");
    for(j=0;j<9;j++){
      if(j==8) fprintf(gdzie," %d\n",plansza[i][j]->wpisane);
      else{
        if(j%3==0) fprintf(gdzie,"  %d ",plansza[i][j]->wpisane);
        else fprintf(gdzie, " %d ",plansza[i][j]->wpisane);
      }
    }
  }
  fprintf(gdzie, "\n\n");
}

void cleanup(sudoku_cell *sudoku[9][9], allowed_values *tab)
/*
 * Argumentami funkcji sa plansza sudoku
 * oraz tablice opisujace co mozna wpisac w rzedach, kolumnach i kwadratach planszy.
 * Funkcja zwalnia miejsce zalokowane dla pol planszy oraz tablic opisujacych ta plansze.
 */
{
  int i,j;
  for(i=0;i<9;i++)
    for(j=0;j<9;j++)
      free(sudoku[i][j]);
  free(tab);
}

void resetCell(int x, int y, sudoku_cell *sudoku[9][9])
/*
 * Argumentami funkcji sa wspolrzedne pola, oraz plansza sudoku,
 * Funkcja usuwa wartosc wpisana w polu i uzupelnia int co mozna wpisac w rzedzie kolumnie i kwadracie w ktorym znajduje sie pole.
 */
{
  int temp=sudoku[x][y]->wpisane;

  sudoku[x][y]->wpisane=0;

  setBitValue(sudoku[x][y]->rzad, temp-1, 1);
  setBitValue(sudoku[x][y]->kolumna, temp-1, 1);
  setBitValue(sudoku[x][y]->kwadrat, temp-1, 1);
}

void resetRow(int ktory, sudoku_cell *sudoku[9][9])
/*
 * Argumentami funkcji sa numer rzedu, oraz plansza sudoku,
 * Funkcja zeruje pola konkretnego rzedu kozystajac z funkcji resetCell.
 */
{
  int i;

  for(i=0;i<9;i++){
    resetCell(ktory,i,sudoku);
  }
}
