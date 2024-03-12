#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
int bSize;

// Declaring functions:
int check_surrounding_indexes(char **arr, char *word, int index, int row, int column, int size);
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

void upperCase(char* word) {
    while (*word != '\0') {
        if (*word >= 'a' && *word <= 'z') {
            *word = *word - ('a' - 'A'); // Turning lowercase char into upper by subtracting ASCII values, 32 constant value difference from lowercase to uppercase
        }
        word++;
    }
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr+i) + j)); // Printing second row of 2D array and jth column 
        }
        printf("\n");
    }
    printf("\n");
}

void printSolution(int** arr) {
	// This function will print out the complete puzzle grid (arr). 
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%d ", *(*(arr+i) + j)); // Printing second row of 2D array and jth column 
        }
        printf("\n");
    }
    printf("\n");
}

int check_surrounding_indexes(char **arr, char *word, int index, int row, int column, int size) {
    int **currentGrid = (int**) malloc(bSize*sizeof(int*));

    if (index == size - 1) {  // If the index reaches the end of the word, return 1 and exit
        return 1;
    }

    index++;

    if (*(*(arr + row) + column) != *(word + index)) { // If the current character doesn't match, exit
        return 0;
    }

    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) {
            if (i >= 0 && j >= 0) {
                if (i < bSize && j < bSize) {
                    if (!(i == row && j == column)) {
                        if (*(*(arr + i) + j) == *(word + index)) {
                            if (check_surrounding_indexes(arr, word, index, i, j, size) == 1) {
                            *(*(currentGrid + i) + j) = *(*(arr + i) + j) * 10 + (index + 1);
                            return 1;
                            }
                        }
                    }
                }
            }
        }
    }



    return 0;
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
   int **foundGrid = (int**) malloc(bSize*sizeof(int*)); // The result 2D array 
   int wordLength = strlen(word);
   int found = 0, index = 0;

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

   for (i = 0; i < bSize; i++) {
    for (j = 0; j < bSize; j++) {
        if (*(*(arr + i) + j) == *(word + index)) {
            if (check_surrounding_indexes(arr, word, index, i, j, wordLength)) {
                if (*(*(foundGrid + i) + j) > 0) {
                    *(*(foundGrid + i) + j) = *(*(foundGrid + i) + j) * 10 + (index + 1);
                } else {
                    *(*(foundGrid + i) + j) = 1;
                }
                found = 1;
            }
        }
    }
   }
   

   if (found) {
    printf("Word found!\n");
    printSolution(foundGrid);
   } else {
    printf("Word not found!\n");
   }
}

