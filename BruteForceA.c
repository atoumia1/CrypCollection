/* Program to brute force a SHA1 hashed password
 * made up of up to 6 numbers and upper-case letters
*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define MAX_PASSWORD_LENGTH 6
#define PASSWORD_BUFFER_SIZE (MAX_PASSWORD_LENGTH + 1)


void hexConverter(const char* hexString, unsigned char* charArray)
{
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sscanf(hexString + 2 * i, "%2hhx", &charArray[i]);
    }
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
    clock_t startTime = clock(); // Records brute brute force start time

    unsigned char target_hash[SHA_DIGEST_LENGTH];
    hexConverter(target_hash_str, target_hash); // Converting input string into char array

    unsigned char password[PASSWORD_BUFFER_SIZE] = ""; // Initialize password
    unsigned char hash[SHA_DIGEST_LENGTH]; // Array to store password hash
    int passLen = strlen((const char*)password); // Sets length of password to the amount of characters inside it
    int attempts = 0;

    while (passLen <= MAX_PASSWORD_LENGTH) { // Runs as long as the password is 6 characters or lesss
        SHA1(password, strlen((const char *)password), hash); // Calculates SHA1 hash of password and stores it in hash

        // Code is run if hash and target_hash are the same
        if (memcmp(hash, target_hash, SHA_DIGEST_LENGTH) == 0) {
            clock_t endTime = clock(); // Records time when passwords is found
            double elapsedTime = ((double) (endTime - startTime)) / CLOCKS_PER_SEC; // Calculates time spent 
            printf("Password found: %s\n", password);
            printf("Time taken: %f seconds\n", elapsedTime);
            return;
        }

        // Increment the password
        int carry = 1;
        for (int i = passLen - 1; i >= -1 && carry; i--) {
            if (i == -1) {
                password[passLen] = '0';
                passLen++;
                carry = 0;
            }
            else if (password[i] == '9') {
                password[i] = 'A';
                carry = 0;
            } 
            else if (password[i] == 'Z') {
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
    printf("No password found\n");
}

int main(int argc,char *argv[])
{
    if (argc != 8) {
        fprintf(stderr, "Usage: %s <target_hash>\n", argv[0]);
        return 1;
    }

    hashGen(argv[1]);
    hashGen(argv[2]);
    hashGen(argv[3]);
    hashGen(argv[4]);
    hashGen(argv[5]);
    hashGen(argv[6]);
    hashGen(argv[7]);
    return 0;
}
