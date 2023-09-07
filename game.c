#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_GUESSES 10

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to check if a character is a letter
int isAlpha(char c) {
    return isalpha(c);
}

// Function to check if a letter has been guessed before
int isUniqueLetter(char letter, char guessedLetters[]) {
    int i;
    for (i = 0; guessedLetters[i] != '\0'; i++) {
        if (guessedLetters[i] == letter) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

// Function to display the word with guessed letters
void displayWord(char word[], char guessedLetters[]) {
    int i;
    for (i = 0; word[i] != '\0'; i++) {
        int found = 0;
        for (int j = 0; guessedLetters[j] != '\0'; j++) {
            if (guessedLetters[j] == word[i]) {
                found = 1;
                break;
            }
        }
        if (found) {
            printf("%c ", word[i]); // Display guessed letter
        } else {
            printf("_ "); // Display underscore for unguessed letters
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <dictionary_file> <word_length> <num_guesses>\n", argv[0]);
        return 1; // Error
    }

    char *dictionaryFile = argv[1];
    int wordLength = atoi(argv[2]);
    int numGuesses = atoi(argv[3]);

    if (wordLength < 1 || numGuesses < 1) {
        printf("Invalid word length or number of guesses.\n");
        return 1; // Error
    }

    char guessedLetters[MAX_GUESSES]; // Store guessed letters
    for (int i = 0; i < MAX_GUESSES; i++) {
        guessedLetters[i] = '\0'; // Initialize with null characters
    }

    char word[MAX_WORD_LENGTH]; // Store the chosen word
    char correctWord[MAX_WORD_LENGTH]; // Store the word with guessed letters
    for (int i = 0; i < wordLength; i++) {
        correctWord[i] = '_'; // Initialize with underscores
    }
    correctWord[wordLength] = '\0';

    FILE *file = fopen(dictionaryFile, "r");
    if (!file) {
        printf("Unable to open dictionary file.\n");
        return 1; // Error
    }

    char validWords[10000][MAX_WORD_LENGTH]; // Store valid words of the specified length
    int numValidWords = 0;

    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0'; // Remove newline character
        if (strlen(word) == wordLength && strspn(word, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == wordLength) {
            strcpy(validWords[numValidWords], word); // Copy the valid word
            numValidWords++;
        }
    }

    if (numValidWords == 0) {
        printf("No valid words of length %d found in the dictionary.\n", wordLength);
        fclose(file);
        return 1; // Error
    }

    fclose(file);

    srand(time(NULL)); // Seed the random number generator

    int randomIndex = rand() % numValidWords; // Generate a random index
    strcpy(word, validWords[randomIndex]); // Select a random word

    printf("Welcome to the Word Guessing game!\n");
    printf("You have %d guesses to guess a word of length %d.\n", numGuesses, wordLength);

    while (numGuesses > 0) {
        printf("Guessed letters: %s\n", guessedLetters);
        printf("Word: ");
        displayWord(correctWord, guessedLetters);

        printf("Enter a letter: ");
        char guess;
        scanf(" %c", &guess);

        if (!isAlpha(guess)) {
            printf("Invalid input. Please enter a letter.\n");
            clearInputBuffer();
            continue;
        }

        guess = tolower(guess); // Convert to lowercase

        if (!isUniqueLetter(guess, guessedLetters)) {
            printf("You've already guessed '%c'.\n", guess);
            continue;
        }

        guessedLetters[strlen(guessedLetters)] = guess;
        guessedLetters[strlen(guessedLetters)] = '\0';

        int found = 0;

        for (int i = 0; word[i] != '\0'; i++) {
            if (word[i] == guess) {
                correctWord[i] = guess; // Update the correctWord
                found = 1;
            }
        }

        if (!found) {
            numGuesses--;
            printf("Incorrect guess. %d guesses remaining.\n", numGuesses);
        } else {
            printf("Correct guess!\n");
        }

        if (strcmp(correctWord, word) == 0) {
            printf("Congratulations! You've guessed the word: %s\n", word);
            break;
        }
    }

    if (numGuesses == 0) {
        printf("You've run out of guesses. The word was: %s\n", word);
    }

    return 0; // Success
}
