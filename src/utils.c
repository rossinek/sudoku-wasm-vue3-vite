#include "utils.h"

void resetBits(int *num, int value) {
  *num = 0;
  if (value == 1) {
    *num = ~*num;
  }
}

int getBitValue(int num, unsigned int pos) {
  if ((1 << pos) & num) {
    return 1;
  }
  return 0;
}

void setBitValue(int *num, unsigned int pos, int value) {
  if (value == 0) {
    *num &= ~(1 << pos);
  } else {
    *num |= 1 << pos;
  }
}

int countBits(int num, int n) {
  int i, c = 0;
  for (i = n - 1; i >= 0; i--) {
    if (getBitValue(num, i)) {
      c++;
    }
  }
  return c;
}

int getFirstBitPosition(int num) {
  int i = 0;
  while (getBitValue(num, i) == 0) {
    i++;
  }
  return i;
}
