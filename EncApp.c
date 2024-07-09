/* Program that:
 * 1. Takes 2 messages and hides message 2 into message 1 using a stream cipher
 * 2. Read a ciphertext and and recover both messages
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BYTE_LENGTH 8

// Generates random key using the input message
void randomKey(int length, char* key)
{
    srand(time(NULL)); // Uses the time as a seed
    for (int i = 0; i < length; i++) {
        key[i] = rand() % 256; // Generates random number and adds it to the key
    }
}

// Converts the message into binary
void binaryConverter(char* message, int length, int* binary)
{
 for (int i = 0; i < length; i++) {
        for (int j = 0; j < BYTE_LENGTH; j++) {
            // Isolates each bit by shifting them to the end and using & 0x01 to only focus on the final bit
            int bitPosition = i * BYTE_LENGTH + j;
            binary[bitPosition] = (message[i] >> (BYTE_LENGTH - 1 - j)) & 0x01;
        }
    }
}

// Returns binary array into previous state
void binaryReverter(int * binary, int length, char* message)
{
    for (int i = 0; i < length; i++) {
        int character = 0; // Initialising the character to 0
        for (int j = 0; j < BYTE_LENGTH; j++) {
            int bitPosition = i * BYTE_LENGTH + j;
            character = (character << 1) | binary[bitPosition]; // Setting the character to the value of each byte
        }
        message[i] = character; // Sets the completed character
    }
    message[length] = '\0';
}

// XORs a message with a key to get an encrypted messsage
void messageEncryptor(char* message,int length, char* key)
{
    for (int i = 0; i < length; i++) {
        message[i] ^= key[i];
    }
}

void messageHide(char* message, int* secretCode, int length)
{
    for (int i = 0; i < length; i++) {
        if (secretCode[i] == 0) {
            // Append space for bit 0
            strcat(message, " ");
        }
        else {
            // Append tab for bit 1
            strcat(message, "\t");
        }
    }
}

void extractMessage(char* message, int* extractedBinary, int length, int ogLength)
{
    int binaryIndex = 0;
    for (int i = ogLength; i < length; i++) { // Loops from the end where the original string ended
        if (message[i] == ' ') {
            extractedBinary[binaryIndex] = 0; // Adds a 0 if message was ' '
            binaryIndex++;
        }
        else if (message[i] == '\t') {
            extractedBinary[binaryIndex] = 1; // Adds a 1 if message was '\t'
            binaryIndex++;
        }
    }
}

void getUserInput(char* input, const char* prompt)
{
    printf("%s: ", prompt); // Prints whatevers is put into the function
    scanf(" %[^\n]s", input); // Takes users input until \n
}

int main(int argc, char** argv)
{
    /*
    printf("What would you like to do?\n");
    printf("1. Encrypt a message?\n");
    printf("2. Decrypt a message?\n");
    printf("3. Exit\n");
    
    int choice;
    scanf("%d", &choice);
    char story[350]; // Stores message where data will be hidden
    char message[50]; // Stores secret message
    int length; // Length of the secret message
    int storyLength;

    switch (choice) {
        case 1:
            getUserInput(story, "Enter message to hide data into: \n");
            storyLength = strlen(story); // Stores the length of story

            getUserInput(message, "Enter a secret message to hide: \n");
            length = strlen(message); // Stores the length of the message
            
            char storyCopy[400]; // Will store a copy of the message with data hidden inside
            strcpy(storyCopy, story);
            
            char key[50]; // Stores the key that is generated

            int secretBinary[50 * BYTE_LENGTH]; // Stores binary of encrypted message

            // Generates random key
            randomKey(length, key);

            // Encrypts the secret message using the key
            messageEncryptor(message, length, key);

            // Converts the encrypted message into binary
            binaryConverter(message, length, secretBinary);

            // Puts the encrypted message binary into the normal string
            messageHide(storyCopy, secretBinary, length * BYTE_LENGTH);

            printf("Encrypted message <%s>\n",storyCopy);
            printf("Your key is: ");
            for (int i = 0; i < length; i++) {
                printf("%02x", (unsigned char)key[i]);
            }
            printf("\n");
            break;
        
        case 2:

            getUserInput(story, "Enter the text you wish to decrypt: \n");
            getUserInput(message, "Enter the key: \n");
            
            length = strlen(message);
            storyLength = strlen(story);
            for (int i = 0; i < length; i++) {
                printf("%02x", (unsigned char)key[i]);
            }
            printf("\n");
            
            int extractedBinary[50 * BYTE_LENGTH]; // Stores the binary once pulled from the text

            char secret[50];

            // Gets the binary data out of the message and stores it into 
            extractMessage(story, extractedBinary, storyLength + length * BYTE_LENGTH, storyLength);
            
            // Reverts the binary into its original state
            binaryReverter(extractedBinary, length, secret);

            // Deccrypts the encrypted message by using the key to get original message
            messageEncryptor(secret, length, message);

            printf("Hidden message is: %s\n", secret);
            break;

        default:
            printf("Invalid, please enter 1-3\n");
    } */
    

    if (argc != 3) {
        printf("Insert the message to hide data into and the message that will be hidden in this format\n");
        printf("./EncApp 'message1' 'message2'\n");
        return 1;
    }
    char* story = argv[1];

    // Takes user input from the terminal
    char* message = argv[2];

    // Stores the length of the message
    int length = strlen(message);

    // Stores the length of story
    int storyLength = strlen(story);
    
    char storyCopy[strlen(story) + length * BYTE_LENGTH];
    strcpy(storyCopy, story);

    // Will store the generated key
    char key[length];

    // Will store the binary of the message
    int secretBinary[length * BYTE_LENGTH];

    // Will store the decrypted message
    char decryptedMessage[length + 1]; 

    // Will store extracted binary
    int extractedBinary[length * BYTE_LENGTH];

    randomKey(length, key);

    messageEncryptor(message, length, key);

    binaryConverter(message, length, secretBinary);

    messageHide(storyCopy, secretBinary, length * BYTE_LENGTH);

    extractMessage(storyCopy, extractedBinary, storyLength + length * BYTE_LENGTH, storyLength);

    binaryReverter(extractedBinary, length, message);

    messageEncryptor(message, length, key);
    
    // printf("Original Message is: %s\n", message);
    printf("Modified Message is: %s\n", storyCopy);
    
    printf("Binary is: ");
    for (int i = 0; i < length * BYTE_LENGTH; i++) {
        printf("%d", secretBinary[i]);
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");

    printf("Extracted binary is: ");
    for (int i = 0; i < length * BYTE_LENGTH; i++) {
        printf("%d", extractedBinary[i]);
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");

    printf("Decrypted message is: %s\n", message);
    return 0;
}
