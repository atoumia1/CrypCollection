#include <stdio.h>

int main()
{
    // Variable declarations
    int arrLength = 10;
    int myArray[arrLength];
    int result = 0;

    // Gather input from user into array 
    printf("Enter %d Numbers:\n", arrLength);
    for (int i = 0; i < arrLength; i++) {
        scanf("%d", &myArray[i]);
    }
  
    // Print out input into one line to user
    printf("\nInput Array Is:..........");
    for(int i = 0; i < arrLength; i++) {
        printf("%d", myArray[i]);
        printf(i == arrLength - 1 ? "\n" : ", ");
    }

    // Multiply each instance in array by the index of it + 1
    printf("\nProcessed Array is:......");
    for(int i = 0; i < arrLength; i++) {
        myArray[i] *= (i + 1);
        result += myArray[i];
        printf("%d", myArray[i]);
        printf(i == arrLength - 1 ? "\n" : ", ");
    }

    // Print mod result of complete array
    result = result % 11;
    printf("\nISBN Verification Result: %s ", result==0 ? "True" : "False");
    printf("(%d)\n", result);
    return 0;
}
