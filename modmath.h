#ifndef MODMATH_H
#define MODMATH_H

int mod11(int x);
int inv(const int x, const int mod);
int sqr(const int x, const int mod);
int* genBCH(const int* inpArr);
int decBCH(int* inpArr);
int* genISBN6(int* inpArr);
void valISBN6(int* inpArr);
void arrPrint(const int* inpArr, const int arrSize);

#endif
