/* Program to brute force a SHA1 hashed 
 * password which is a valid BCH(10,6) number
*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "modmath.h"

#define BCH_LENGTH 10
#define LOOP_NUM_LENGTH 6
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
    for (int i = 0; i < BCH_LENGTH; i++) {
        string[i] = arr[i] + '0'; // Change integer to character and store it into string
    }
    string[BCH_LENGTH] = '\0'; // Null terminates the string
}

// Print progress every 100,000 attempts
void printProgress(int attempts, unsigned char* password)
{
    if (attempts % 100000 == 0) {
        printf("Attempts: %d, Current Password: %s\n", attempts, password);
    }
}

void hashGen(const char* target_hash_str)
{
    clock_t startTime = clock(); // Records when brute force is started

    unsigned char target_hash[SHA_DIGEST_LENGTH];
    hexConverter(target_hash_str, target_hash); // Converting input string into char array

    unsigned char password[LOOP_NUM_BUFFER_SIZE] = "000000"; // Initialize password
    int passwordNum[LOOP_NUM_LENGTH]; // Stores password as an integer
    int* BCH; // Stores BCH integer array
    unsigned char strBCH[BCH_LENGTH + 1] = "0000000000"; // Stores BCH string after its changed back from string
    unsigned char hash[SHA_DIGEST_LENGTH]; // Array to store password hash
    int attempts = 0;

    while (attempts < 999999) {

        strToInt(password, passwordNum); // Converts current attempt from string to int

        BCH = genBCH(passwordNum); // Generates BCH using the attempt saves it

        if (BCH != NULL) {
            intToStr(BCH, strBCH); // Return the new BCH code into string
            SHA1(strBCH, BCH_LENGTH, hash); // Calculates SHA1 hash of password and stores it in hash
        }

        // Code is run if hash and target_hash are the same
        if (memcmp(hash, target_hash, SHA_DIGEST_LENGTH) == 0) {
            clock_t endTime = clock(); // Records time when password is found
            double elapsedTime = ((double) (endTime - startTime)) / CLOCKS_PER_SEC; // Calculates time spent
            printf("Password found: %s\n", strBCH);
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
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <target_hash>\n", argv[0]);
        return 1;
    }

    hashGen(argv[1]);
    hashGen(argv[2]);
    hashGen(argv[3]);
    hashGen(argv[4]);
    return 0;
}
