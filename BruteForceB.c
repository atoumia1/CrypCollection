/* Program to brute force a SHA1 hashed 
 * password which is a valid ISBN6 number
*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "modmath.h"

#define ISBN_LENGTH 6
#define LOOP_NUM_LENGTH 5
#define LOOP_NUM_BUFFER_SIZE (LOOP_NUM_LENGTH + 1)


void hexConverter(const char* hexString, unsigned char* charArray)
{
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sscanf(hexString + 2 * i, "%2hhx", &charArray[i]);
    }
}

void strToInt(unsigned char* string, int* arr)
{
    for (int i = 0; i < LOOP_NUM_LENGTH; i++) {
        arr[i] = string[i] - '0'; // Change character to integer and store it into arr 
    }
}

void intToStr(int* arr, unsigned char* string)
{ 
    for (int i = 0; i < ISBN_LENGTH; i++) {
        string[i] = arr[i] + '0'; // Change integer to character and store it into string
    }
    string[ISBN_LENGTH] = '\0'; // Null terminates string
}

// Print progress every 100 attempts
void printProgress(int attempts, unsigned char* password)
{
    if (attempts % 100 == 0) {
        printf("Attempts: %d, Current Password: %s\n", attempts, password);
    }
}

void hashGen(const char* target_hash_str)
{
    clock_t startTime = clock(); // Records when the brute force started

    unsigned char target_hash[SHA_DIGEST_LENGTH];
    hexConverter(target_hash_str, target_hash); // Converting input string into char array

    unsigned char password[LOOP_NUM_BUFFER_SIZE] = "00000"; // Initialize password
    int passwordNum[LOOP_NUM_LENGTH]; // Stores password as an integer
    int* ISBN6; // Stores ISBN6 integer array
    unsigned char strISBN6[ISBN_LENGTH + 1] = "000000"; // Stores ISBN6 string after its changed back from string
    unsigned char hash[SHA_DIGEST_LENGTH]; // Array to store password hash
    int attempts = 0;

    while (attempts <= 99999) {

        strToInt(password, passwordNum); // Converts current attempt from string to int

        ISBN6 = genISBN6(passwordNum); // Generates ISBN6 using the attempt saves it

        intToStr(ISBN6, strISBN6); // Return the new ISBN6 code into string

        SHA1(strISBN6, ISBN_LENGTH, hash); // Calculates SHA1 hash of password and stores it in hash

        // Code is run if hash and target_hash are the same
        if (memcmp(hash, target_hash, SHA_DIGEST_LENGTH) == 0) {
            clock_t endTime = clock(); // Records time when password is found
            double elapsedTime = ((double) (endTime - startTime)) / CLOCKS_PER_SEC; // Calculates the time spend
            printf("Password found: %s\n", strISBN6);
            printf("Time taken: %f seconds\n", elapsedTime);
            return;
        }

        // Increment the password
        int carry = 1;
        for (int i = LOOP_NUM_LENGTH - 1; i >= 0 && carry; i--) {
            if (password[i] == '9') {
                password[i] = '0';
            } 
            else {
                password[i]++;
                carry = 0;
            }
        }
        attempts++;
        // printProgress(attempts, password);
    }
    printf("Password not found\n");
}

int main(int argc,char *argv[])
{
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <target_hash>\n", argv[0]);
        return 1;
    }

    hashGen(argv[1]);
    hashGen(argv[2]);
    hashGen(argv[3]);
    hashGen(argv[4]);
    hashGen(argv[5]);
    return 0;
}
