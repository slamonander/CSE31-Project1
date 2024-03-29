#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
int bSize;
int **foundGrid; // Resulting 2d array of the word search, declaring as global to make accessing data

// Declaring functions:
int check_surrounding_indexes(char **arr, char *word, int index, int row, int column, int wordLength);
void upperCase(char* word);
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void printSolution(int** arr);

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}



// ---- All functions that will be implemented for the project ----

// Function to check on cases of characters

// Turning the word into uppercase

void upperCase(char* word) {
    while (*word != '\0') {
        if (*word >= 'a' && *word <= 'z') {
            *word = *word - ('a' - 'A'); // Turning lowercase char into upper by subtracting ASCII values, 32 constant value difference from lowercase to uppercase
        }
        word++;
    }
}

// Printing the initial puzzle from the text files

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c\t", *(*(arr+i) + j)); // Printing second row of 2D array and jth column 
        }
        printf("\n");
    }
    printf("\n");
}

// Printing the resulting 2D array of the puzzle, which is the foundGrid array

void printSolution(int** arr) {
	// This function will print out the complete puzzle grid (arr). 
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%d\t", *(*(arr+i) + j)); // Printing the array into a table
        }
        printf("\n");
    }
    printf("\n");
}

int check_surrounding_indexes(char **arr, char *word, int index, int row, int column, int wordLength) {

    if (index == wordLength - 1) {  // If the index reaches the end of the word, return 1 and exit
        return 1;
    }

    index++; // Increment next character to search 

    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) { //Search around the surrounding elements
            if (i >= 0 && j >= 0 && i < bSize && j < bSize && !(i == row && j == column)) { // Checking the bounds and making sure it does not go out of bound
                if (*(*(arr + i) + j) == *(word + index)) { // See if the current character in arr matches the current character of word
                    if (check_surrounding_indexes(arr, word, index, i, j, wordLength)) { // If matches, recursively call back to the function and search for the next character
                        if (*(*(foundGrid + i) + j) > 0) { // Checks if the current element in the resulting array has been marked/found
                            *(*(foundGrid + i) + j) = *(*(foundGrid + i) + j) * 10 + (index + 1); // Update the current element in the resulting array to the appropriate index(es)
                        } else {
                            *(*(foundGrid + i) + j) = index + 1;
                        }
                        return 1; // Word is found
                    }
                }
            }
        }
    }
    return 0; // Word is not found
}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.

    /*
        1. Turn the word into uppercase
        2. Figure out search algo
        3. Traverse the grid
        4. If a character of the word is found, figure out the position the 
        character it is in with the word
        5. At arr[i][j] with that character, input the word[i] position.
        6. All other characters not in the word has their arr[i][j] = 0

        * All characters in the word must be 1 index away from each other (row, column, both)
        * All directions allowed in the search
        
    */

    upperCase(word); // Turn word into uppercase

    int i = 0, j = 0; // i = row, j = column
    foundGrid = (int**) malloc(bSize*sizeof(int*)); // The result 2D array 
    int wordLength = strlen(word);
    int found = 0, currentElement = 0;

    // Traversing the grid
    // Check if a char in the grid is also in the word.
    // 1. If char at arr[i][j] matches char in word, arr[i][j] = word[matching char pos]
    // 2. Check surrounding chars within 1 index
    // 3. Else, arr[i][j] = 0

    // Turning resulting grid into a 2D array of 0s

    for (i = 0; i < bSize; i++) {
        *(foundGrid + i) = (int*)malloc(bSize*sizeof(int));
        for (j = 0; j < bSize; j++) {
            *(*(foundGrid + i) + j) = 0;
        }
    }

    // Starting the search for the word to see if the word exists in the puzzle. 

    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            if (*(*(arr + i) + j) == *(word + currentElement)) { // Checks if current character in the array matches the current character in the word
                if (check_surrounding_indexes(arr, word, currentElement, i, j, wordLength)) { // Calls the recursive function to check surrounding indexes to see if the word does exist around the current character
                    if (*(*(foundGrid + i) + j) > 0) { // Checks to see if the current element in the resulting grid is marked
                        *(*(foundGrid + i) + j) = *(*(foundGrid + i) + j) * 10 + (currentElement + 1); // Update the current element of the resulting grid to the appropriate index(es)
                    } else {
                        *(*(foundGrid + i) + j) = 1;
                    }
                    found = 1;
                } 
            }
        }
    }


    if (found == 1) {
        printf("Word found!\n");
        printSolution(foundGrid);
    } else {
        printf("Word not found!\n");
    }
}

