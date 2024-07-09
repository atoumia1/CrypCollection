#include <stdio.h>
#include "modmath.h"

// Calculates the number mod 11 ensuring the answer is always positive
int mod11(int x)
{
    x = x % 11;
    if (x < 0) {
        x += 11;
    }
    return x;
}

// Calculates the inverse of a number
int inv(const int x, const int mod)
{
    // Loops down through every value from (mod - 1) until finding a result equal to 1 mod 11 (or what mod is)
    int inv = mod - 1;
    while (x * inv % mod != 1) {
        inv--;
        if (inv == 0) {
        return -1; // Returns -1 if there are no results meaning the numbers are not coprime
        }
    }
    return inv;
}

// Does a modular square root
int sqr(const int x, const int mod)
{
    // Finds if there is any value when squared and mod is equal to x
    for (int i = 1; i < mod; i++) {
        if (i * i % mod == x) {
            return i;
        }
    }
    return -1;
}

// Generates BCH code
int* genBCH(const int* inpArr)
{
    // Declaring Variables (i being index used in for multiplication)
    int i7[6]   = {4, 10, 9, 2, 1, 7};
    int i8[6]   = {7, 8, 7, 1, 9, 6};
    int i9[6]   = {9, 1, 7, 8, 7, 7};
    int i10[6]  = {1, 2, 9, 10, 4, 1};
    static int BCH[10] = {0};

    // Array declaration due to static behaviour
    for (int i = 0; i < 10; i++) {
        BCH[i] = i < 6 ? inpArr[i] : 0; // Depending on if i is less than 6: BCH[i] is equal to inpArr[i] if true or 0 if false
    }

    // Multiply values and add to array
    for (int i = 0; i < 6; i++) {
        BCH[6] += inpArr[i] * i7[i];
        BCH[7] += inpArr[i] * i8[i];
        BCH[8] += inpArr[i] * i9[i];
        BCH[9] += inpArr[i] * i10[i];
    }

    // Mod all values and printf if any of the results are 10
    for (int i = 6; i < 10; i++) {
        BCH[i] = mod11(BCH[i]);
        if (BCH[i] == 10) {
            return NULL;
        }
    }
    return BCH;
}

// Detects BCH errors and corrects up to 2 
int decBCH(int* inpArr)
{
    // Take array as input and calculate S1 S2 S3 and S4
    int syn[4] = {0};
    for (int i = 0; i < 10; i++) {
        syn[0] += inpArr[i];
        syn[1] += inpArr[i] * (i + 1);
        syn[2] += inpArr[i] * (i + 1) * (i + 1);
        syn[3] += inpArr[i] * (i + 1) * (i + 1) * (i + 1);
    }
    syn[0] = mod11(syn[0]);
    syn[1] = mod11(syn[1]);
    syn[2] = mod11(syn[2]);
    syn[3] = mod11(syn[3]);
    
    //If all syndrome values are 0 then print that there are no errors and return
    if (syn[0] == 0 && syn[1] == 0 && syn[2] == 0 && syn[3] == 0) {
        return 0; // No errors
    }
    
    // Calculating P, Q, R
    int P = mod11(syn[1] * syn[1] - syn[0] * syn[2]);
    int Q = mod11(syn[0] * syn[3] - syn[1] * syn[2]);
    int R = mod11(syn[2] * syn[2] - syn[1] * syn[3]);
    
    // If all values are 0, there is one error
    if (P == 0 && Q == 0 && R == 0) {
        int a = syn[0];
        int i = mod11(syn[1] * inv(syn[0], 11));
     
    // If magnitude is not equal to 0 then append the single error
        if (i != 0) {
            // Edit code at position 'i' and magnitude 'a' and return
            inpArr[i - 1] = mod11(inpArr[i - 1] - a);
            return 1; // 1 error
        }
    }

    // There are 2 errors if P, Q, R are not equal to 0
    int quad = mod11((Q * Q) - (4 * P * R));
    
    if (sqr(quad, 11) == -1) { // If discriminant has no sqr then more than 2 errors
        return 3; // 3 or more errors
    }
    int i = mod11((-Q + sqr(quad, 11)) * inv(2 * P, 11));
    int j = mod11((-Q - sqr(quad, 11)) * inv(2 * P, 11));
    int b = mod11((i * syn[0] - syn[1]) * inv(mod11(i - j), 11));
    int a = mod11(syn[0] - b);
    
    if (i == 0 || j == 0) { // If position is 0 then more than 2 error
        return 3; // 3 or more errors
    }
    
    inpArr[i - 1] = mod11(inpArr[i - 1] - a);
    inpArr[j - 1] = mod11(inpArr[j - 1] - b);
    
    for (int i = 0; i < 10; i++) {
        if (inpArr[i] == 10) { // if any value has been changed to 10 then there are more than 3 errors
            return 3; // 3 or more errors
        }
    }
    return 2; // 2 errors
}

// Generates ISBN 6 number
int* genISBN6(int* inpArr)
{
    int d6 = 0;
    static int ISBN[6] = {0};
    
    for (int i = 0; i < 6; i++) {
        ISBN[i] = i < 5 ? inpArr[i] : 0;
        d6 += ISBN[i] * (6 - i);
    }
    ISBN[5] = 7 - (d6 % 7);
    return ISBN;
}

// Checks ISBN 6 code
void valISBN6(int* inpArr)
{   
    int index = 0;
    int total = 0;
    int arrCheck[6] = {0};
    
    for (int i = 6; i > 0; i--) {
        arrCheck[index] = inpArr[index];
        arrCheck[index] *= i;
        total += arrCheck[index];
        index++;
    }
    printf(total % 7 == 0 ? "Valid ISBN\n":"Invalid ISBN\n");
}

void arrPrint(const int* inpArr, const int arrSize)
{
    // Quick return if the array is null
    if (inpArr == NULL) {
        return;
    }

    // Print through all iteraions in array
    printf("Array Output: ");
    for (int i = 0; i < arrSize; i ++) {
       printf("%d",inpArr[i]);
    }  
    printf("\n");
}
