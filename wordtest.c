#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
const int MAX_SPACING_LEN = 8;
int bSize;

void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);

//STUDENT MADE FUNCTIONS
void toUpperCase(char* word);
void printSpacing(const char* word, int spacing);
void printCompletedGrid(int*** paths, int size, int pathLength);
int searchPuzzleRep(char **arr, int** path, char* word, int len, int index, int row, int col); //recursive search method


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


void printPuzzle(char** arr) {
    for(int i = 0; i < bSize; i++) { // bSize is a global variable, thus we can call upon it rn
        for(int j = 0; j < bSize; j++) {
            printf("%c", *(*(arr + i) + j));
        }
        printf("\n");
    }

    printf("\n");
}

void searchPuzzle(char** arr, char* word) { //Tom fixed my logic pathing, see v0.1 for my (Karina's) attempt
    int wordLength = strlen(word);

    toUpperCase(word); // First, uppercase the entire word
    
    int*** paths = (int***)malloc(bSize * bSize); // bSize^2 starting locations
    int size = 0; // number of paths found
    for(int row = 0; row < bSize; row++) { // Process through all possible starting positions
        for(int col = 0; col < bSize; col++) {
            int** path = (int**)malloc(wordLength *sizeof(int*)); // one path will have wordLength chars
            *(paths + size) = path;
            int isFound = searchPuzzleRep(arr, path, word, wordLength, 0, row, col);  
            if (isFound)                                                                   
                size++;     //if found, then value assigned to size increases, creating the path order
        }
    }
    
    if(size == 0) {
        printf("Word not found!\n");
    }
    else{
        printf("Word found!\n");
        printf("Printing the search path:\n");
        printCompletedGrid(paths, size, wordLength);
    }
}


//NOTES:
    // Capitalize all letters for searching convenience
    // Additional void print for the spacing in the search path result (refer to samples in instructions)

void toUpperCase(char* word) { // capitalizes string
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] >= 'a' && word[i] <= 'z') {
            word[i] = word[i] - 'a' + 'A';
        }
    }
}

void printSpacing(const char* word, int spacing) { // fit the style of the sample runs
    int i = 0;

    while (*(word + i) != '\0') {   //while "word" is not null
        printf("%c", *(word + i));
        i++;
    }

    for(int j = 0; j < spacing - i; j++) {
        printf(" ");
    }
}

int searchPuzzleRep(char **arr, int** path, char* word, int len, int index, int row, int col) { // Recursive function searches for a word fragment, starting at the index for the word
    // Returns 1 if the fragment is found, 0 if not

    if (*(*(arr + row) + col) != *(word + index))
        return 0;       // If the current character doesn't match, this path can't continue

    // add current index
    int* coord = (int*)malloc(2 * sizeof(int));
    *(coord + 0) = row;
    *(coord + 1) = col;
    *(path + index) = coord;

    if (index + 1 >= len)// exit condition
        return 1;

    for (int x = row - 1; x <= row + 1; x++) {   // checking the neighboring chars
        for (int  y = col - 1; y <= col + 1; y++) {
            if(
                x >= 0 &&   // must be in the grid, establishing boundaries
                y >= 0 && 
                x < bSize && 
                y < bSize && 
                !(x == row && y == col) &&// cannot be current index
                *(word + index + 1) == *(*(arr + x) + y)    // (x,y) matches
            ) {
                int* coord = (int*)malloc(2 * sizeof(int)); // establish current index
                *(coord + 0) = x;
                *(coord + 1) = y;
                int isFound = searchPuzzleRep(arr, path, word, len, (index + 1), x, y); 
                if (isFound)
                    return 1;
                //else continue to search and override whatever is already stored at *(path + index)
            }
        }
    }

    return 0;

}

void printCompletedGrid(int*** paths, int size, int pathLength) { // prints out merged output/path grid
    // wordLength and pathLength are the same size
    for (int row = 0; row < bSize; row++) {
        for (int col = 0; col < bSize; col++) {
            char* string = malloc(MAX_SPACING_LEN * sizeof(char));  //MAX_SPACING_LEN is a user made global var of 8, this was determined to make the output grid have the same format as the examples

            int str_size = 0;

            for (int i = 0; i < size; i++) { //append the string, hope we don't go over 8
                int** currentPath = *(paths + i);
                for (int j = 0; j < pathLength; j++) {
                    int* coord = *(currentPath + j); 
                    int x = *(coord + 0);
                    int y = *(coord + 1);
                    if (x == row && y == col) { // if the current index is valid
                        int isExist = 0;
                        for (int k = 0; *(string + k) !='\0'; k++){
                            if (*(string + k) == j + 1){  
                                isExist = 1;
                                break;
                            }
                        }
                        if (!isExist)
                            *(string + (str_size++)) = (j + 1) + '0';   // converts index to char and adds to string
                    }
                }
            }

            *(string + str_size) = '\0'; // terminate string

            if (str_size == 0) { // if elem is never visited, place a zero
                printSpacing("0", MAX_SPACING_LEN);
            } else {
                printSpacing(string, MAX_SPACING_LEN);
            }
        }
        printf("\n");
    }

}


