//Name: Vamsi Krishna Pavuluri
//CWID: A20449747
//Author: Vamsi Krishna Pavuluri

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The below one is the Structure to hold the DFSM information!
typedef struct {
    char* alphabet;                 // Dynamically allocated array for storing the input alphabet symbols
    int** transitionTable;          // Dynamically allocated 2D array for storing the transition table
    int* acceptingStates;           // Dynamically allocated array for storing the accepting/final states
    int numStates;                  // Total number of states in the given DFSM
    int numAlphabet;                // Total number of input alphabet symbols in the given DFSM
    int numAcceptingStates;         // Total number of accepting states in the given DFSM
} DFSM;






// In the below function, I written logic to deal with whitespaces for each line of the DFSM file given to this trimWhitespace function!
void trimWhitespace(char *str) 
{
    char *end;

    // Trim leading space
    while (*str == ' ') str++;

    // All spaces?
    if (*str == 0) return;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ') end--;

    // Write new null terminator
    *(end + 1) = 0;
}






// In the below function--> I had written the logic to read the DFSM machine from the input DFSM file!
void readDFSMFile(const char* filename, DFSM* dfsm) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Error: Could not open DFSM file %s\n", filename);
        exit(1);
    }

    char *line = NULL;  // Using dynamically allocated buffer for reading each line of the DFSM
    size_t len = 0;     // Length of the allocated buffer
    ssize_t read;       // Number of characters read from the file

    int section = 0; // 0: for input alphabets, 1: for transition table, 2: for accepting/final states
    int stateIndex = 0;
    dfsm->numAlphabet = 0;
    dfsm->numStates = 0;
    dfsm->numAcceptingStates = 0;

    while ((read = getline(&line, &len, file)) != -1) 
    {
        // Trim newline characters and any leading/trailing whitespace
        line[strcspn(line, "\n")] = '\0';
        trimWhitespace(line);

        // Check for empty lines to transition sections
        if (strlen(line) == 0) 
        {
            section++;
            continue;
        }

        // Parse based on section
        if (section == 0) 
        {
            // Section 1: Alphabet
            char* lineCopy = strdup(line);
            char* token = strtok(lineCopy, " ");
            while (token != NULL) 
            {
                dfsm->numAlphabet++;
                token = strtok(NULL, " ");
            }
            free(lineCopy);

            // Allocate memory for the alphabet based on the number of symbols
            dfsm->alphabet = (char*)malloc(dfsm->numAlphabet * sizeof(char));
            if (dfsm->alphabet == NULL) 
            {
                printf("Error: Memory allocation failed for alphabet.\n");
                exit(1);
            }

            // Re-tokenize to store the alphabet symbols
            token = strtok(line, " ");
            int index = 0;
            while (token != NULL) 
            {
                dfsm->alphabet[index++] = token[0];
                token = strtok(NULL, " ");
            }

            if (dfsm->numAlphabet == 0) 
            {
                printf("Error: Input alphabet is missing.\n");
                exit(1);
            }
        } 
        else if (section == 1) 
        {
            // Section 2: Transition Table

            // First, determine the number of states (rows) and allocate memory for the transition table
            dfsm->numStates++;
            if (stateIndex == 0) 
            {
                dfsm->transitionTable = (int**)malloc(dfsm->numStates * sizeof(int*));
                if (dfsm->transitionTable == NULL) 
                {
                    printf("Error: Memory allocation failed for transition table rows.\n");
                    exit(1);
                }
            } 
            else 
            {
                dfsm->transitionTable = (int**)realloc(dfsm->transitionTable, dfsm->numStates * sizeof(int*));
                if (dfsm->transitionTable == NULL) 
                {
                    printf("Error: Memory reallocation failed for transition table rows.\n");
                    exit(1);
                }
            }

            // Allocate memory for each row (number of columns equals the number of alphabet symbols)
            dfsm->transitionTable[stateIndex] = (int*)malloc(dfsm->numAlphabet * sizeof(int));
            if (dfsm->transitionTable[stateIndex] == NULL) 
            {
                printf("Error: Memory allocation failed for transition table columns.\n");
                exit(1);
            }

            // Now, populate the transition table row
            char* token = strtok(line, " ");
            int colIndex = 0;
            while (token != NULL) 
            {
                int state = atoi(token);
                dfsm->transitionTable[stateIndex][colIndex++] = state;
                token = strtok(NULL, " ");
            }
            if (colIndex != dfsm->numAlphabet) 
            {
                printf("Error: Invalid DFSM, transition table is missing or malformed.\n");
                exit(1);
            }
            stateIndex++;
        }
        else if (section == 2) 
        {
            // Section 3: Accepting States
            char* token = strtok(line, " ");
            while (token != NULL) 
            {
                dfsm->numAcceptingStates++;
                dfsm->acceptingStates = (int*)realloc(dfsm->acceptingStates, dfsm->numAcceptingStates * sizeof(int));
                if (dfsm->acceptingStates == NULL) 
                {
                    printf("Error: Memory reallocation failed for accepting states.\n");
                    exit(1);
                }
                dfsm->acceptingStates[dfsm->numAcceptingStates - 1] = atoi(token);
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(file);
    free(line); // Free the dynamically allocated line buffer

    // In the below , I have written code for Error handling after parsing for different possible kinds of errors in the DFSM file!

    // Checking if transition table is missing or not 
    if (dfsm->numStates == 0) 
    {
        printf("Error: Invalid DFSM, transition table is missing.\n");
        exit(1);
    }

    // Validating each transition table state after all states are known
    for (int i = 0; i < dfsm->numStates; i++) 
    {
        for (int j = 0; j < dfsm->numAlphabet; j++) 
        {
            int state = dfsm->transitionTable[i][j];
            if (state < 1 || state > dfsm->numStates) 
            {
                printf("Error: Invalid state %d in the transition table at row %d, column %d. It must be between 1 and %d.\n", state, i + 1, j + 1, dfsm->numStates);
                exit(1);
            }
        }
    }

    // Checking if no accepting states were provided
    if (dfsm->numAcceptingStates == 0) 
    {
        printf("Error: No accepting states provided. Can be treated as Invalid DFSM in practical!\n");
        exit(1);
    }

    // Validating each accepting state
    for (int i = 0; i < dfsm->numAcceptingStates; i++) 
    {
        int state = dfsm->acceptingStates[i];
        if (state < 1 || state > dfsm->numStates) 
        {
            printf("Error: Invalid accepting state %d. It must be between 1 and %d.\n", state, dfsm->numStates);
            exit(1);
        }
    }

    // Checking whether the number of accepting states exceeds the number of actual states
    if (dfsm->numAcceptingStates > dfsm->numStates) 
    {
        printf("Error: The number of accepting states (%d) exceeds the number of actual states (%d).\n", dfsm->numAcceptingStates, dfsm->numStates);
        exit(1);
    }
}






// In the below function--> I had written the logic to read the input string from the input text file!
char* readInputString(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Error: Could not open input file %s\n", filename);
        exit(1);
    }

    char* inputString = NULL;
    size_t length = 0;
    ssize_t read;

    read = getline(&inputString, &length, file);
    if (read == -1) 
    {
        printf("Error: Input file is empty or missing valid content. Invalid string.\n");
        exit(1);
    } 
    else if (strspn(inputString, " \t\n") == strlen(inputString)) 
    {
        // Checking if input is entirely white spaces or newlines
        if (strlen(inputString) == 1) 
        {
            // Single white space is treated as an epsilon string
            inputString = strdup(""); // Set to empty string
        } 
        else 
        {
            // Multiple white spaces or empty string
            printf("Error: Input file contains multiple white spaces or is invalid.\n");
            exit(1);
        }
    } 
    else 
    {
        // Trimming the newline character from the input string
        inputString[strcspn(inputString, "\n")] = '\0';
    }

    fclose(file);
    return inputString;
}






// DFSM simulator for telling us whether the given input string is accepted or not!
int DFSMsimulator(DFSM* dfsm, const char* inputString) 
{
    int currentState = 1; // Start at state 1 (assumed to be the initial state)
    
    // If the input string is empty (epsilon), check if the initial state is accepting
    if (strlen(inputString) == 0) 
    {
        for (int i = 0; i < dfsm->numAcceptingStates; i++) 
        {
            if (dfsm->acceptingStates[i] == currentState) 
            {
                return 1; // Epsilon string accepted
            }
        }
        return 0; // Epsilon string not accepted
    }

    // Process each character in the input string
    for (int i = 0; inputString[i] != '\0'; i++) 
    {
        char inputSymbol = inputString[i];

        // Find the corresponding column in the alphabet
        int symbolIndex = -1;
        for (int j = 0; j < dfsm->numAlphabet; j++) 
        {
            if (dfsm->alphabet[j] == inputSymbol) 
            {
                symbolIndex = j;
                break;
            }
        }

        // If the symbol is not in the alphabet, reject the input
        if (symbolIndex == -1) 
        {
            printf("Error: Input symbol '%c' is not in the alphabet.\n", inputSymbol);
            return 0;
        }

        // Transition to the next state
        currentState = dfsm->transitionTable[currentState - 1][symbolIndex];
    }

    // Check if the final state is one of the accepting states
    for (int i = 0; i < dfsm->numAcceptingStates; i++) 
    {
        if (dfsm->acceptingStates[i] == currentState) 
        {
            return 1; // The string is accepted
        }
    }

    return 0; // The string is rejected
}






// Function to print the given input DFSM file contents 
void printDFSM(DFSM* dfsm) 
{
    printf("Input Alphabet: ");
    for (int i = 0; i < dfsm->numAlphabet; i++) 
    {
        printf("%c ", dfsm->alphabet[i]);
    }
    printf("\n");

    printf("Transition Table:\n");
    for (int i = 0; i < dfsm->numStates; i++) 
    {
        for (int j = 0; j < dfsm->numAlphabet; j++) 
        {
            printf("%d ", dfsm->transitionTable[i][j]);
        }
        printf("\n");
    }

    printf("Accepting States: ");
    for (int i = 0; i < dfsm->numAcceptingStates; i++) 
    {
        printf("%d ", dfsm->acceptingStates[i]);
    }
    printf("\n");
}






// Main Function of my code
int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        printf("Usage: %s <dfsm_file> <input_file>\n", argv[0]);
        return 1;
    }

    DFSM dfsm;
    memset(&dfsm, 0, sizeof(DFSM));

    readDFSMFile(argv[1], &dfsm);

    char* inputString = readInputString(argv[2]);

    printf("DFSM Description:\n");
    printDFSM(&dfsm);

    printf("Input String: %s\n", inputString);

    int result = DFSMsimulator(&dfsm, inputString);

    if (result) 
    {
        printf("yes\n");
    } 
    else 
    {
        printf("no\n");
    }

    free(inputString);

    // Free dynamically allocated memory
    free(dfsm.alphabet);
    for (int i = 0; i < dfsm.numStates; i++) 
    {
        free(dfsm.transitionTable[i]);
    }
    free(dfsm.transitionTable);
    free(dfsm.acceptingStates);

    return 0;
}
