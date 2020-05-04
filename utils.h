#ifndef UTILS_H
#define UTILS_H

void resetBits(int *num, int value);
int getBitValue(int num, unsigned int pos);
void setBitValue(int *num, unsigned int pos, int value);
int countBits(int num);
int getFirstBitPosition(int num);

#endif
