// Name: Vamsi Krishna Pavuluri
// CWID: A20449747
// Author: Vamsi Krishna Pavuluri

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_STATES 100
#define MAX_ALPHABET 26
#define MAX_TRANSITIONS 10



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
            printf("Error: Input symbol '%c' is not in the alphabet. Invalid Input symbol for the DFSM machine\n", inputSymbol);
            exit(1);
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











// Function to write the DFSM to a file
void writeDFSMtoFile(const char* filename, char* alphabets, int transitionTable[MAX_STATES][MAX_ALPHABET], int* dfsmAcceptingStates, int stateCount, int numAlphabet, int dfsmAcceptingCount) 
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) 
    {
        printf("Error: Could not open DFSM output file %s.\n", filename);
        exit(1);
    }

    // Write section 1: input alphabets
    for (int i = 0; i < numAlphabet; i++) 
    {
        fprintf(file, "%c ", alphabets[i]);
    }
    fprintf(file, "\n\n");

    // Write section 2: DFSM transition table
    for (int i = 0; i < stateCount; i++) 
    {
        for (int j = 0; j < numAlphabet; j++) 
        {
            if (transitionTable[i][j] == -1) 
            {
                fprintf(file, "-1 ");
            } 
            else 
            {
                // +1 to start state numbering from 1 instead of 0
                fprintf(file, "%d ", transitionTable[i][j] + 1);
            }
        }
        fprintf(file, "\n");
    }

    // Write an empty line
    fprintf(file, "\n");

    // Write section 3: Accepting states
    for (int i = 0; i < dfsmAcceptingCount; i++) 
    {
        fprintf(file, "%d ", dfsmAcceptingStates[i] + 1);  // +1 to start from state 1
    }
    fprintf(file, "\n");

    fclose(file);
    printf("DFSM has been successfully written to %s.\n", filename);
}





// Function to convert the NDFSM to DFSM
void convertNDFSMtoDFSM(char* alphabets, int transitions[MAX_STATES][MAX_ALPHABET][MAX_TRANSITIONS], int acceptingStates[MAX_STATES], int numAlphabet, int numStates, int numAcceptingStates, const char* outputFile) 
{
    int dfsmStates[MAX_STATES][MAX_STATES];  // 2D array to store DFSM states
    int transitionTable[MAX_STATES][MAX_ALPHABET];  // DFSM transition table
    int dfsmAcceptingStates[MAX_STATES];  // DFSM accepting states
    int stateCount = 0;  // Number of DFSM states

    // Initialize the DFSM with the start state [1]
    dfsmStates[0][0] = 1;  // DFSM start state contains NDFSM state 1
    for (int i = 1; i < numStates; i++) dfsmStates[0][i] = -1;  // Fill the rest of the row with -1
    stateCount++;

    int queue[MAX_STATES];  // Queue for states to process
    int front = 0, rear = 1;
    queue[0] = 0;  // Start processing from the first DFSM state

    
    // Process each DFSM state

    while (front < rear) 
    {
        int currentDFSMState = queue[front++];
   

        for (int alphaIdx = 0; alphaIdx < numAlphabet; alphaIdx++) 
        {
            int newState[MAX_STATES];
            for (int i = 0; i < numStates; i++) newState[i] = -1;  // Initialize new state with -1

            int newStateIdx = 0;
            for (int ndStateIdx = 0; dfsmStates[currentDFSMState][ndStateIdx] != -1; ndStateIdx++) 
            {
                int ndState = dfsmStates[currentDFSMState][ndStateIdx];
           
                // Check if ndState is out of bounds
                if (ndState < 1 || ndState > numStates) 
                {
                    continue;  // Skip invalid states
                }

                for (int t = 0; transitions[ndState - 1][alphaIdx][t] != -2; t++) 
                {
                    int nextState = transitions[ndState - 1][alphaIdx][t];
                    if (nextState != -1) 
                    {
                        // Check if nextState is valid
                        if (nextState < 1 || nextState > numStates) 
                        {
                            continue;  // Skip invalid transitions
                        }

                        // Add the state if it's not already in the new state
                        int alreadyExists = 0;
                        for (int k = 0; k < newStateIdx; k++) 
                        {
                            if (newState[k] == nextState) 
                            {
                                alreadyExists = 1;
                                break;
                            }
                        }

                        if (!alreadyExists) 
                        {
                            newState[newStateIdx++] = nextState;
                        }
                    }
                }
            }

            // Check if the new state already exists in the DFSM
            int foundIdx = -1;
    
            for (int i = 0; i < stateCount; i++) 
            {
                int found = 1;
                for (int j = 0; j < numStates; j++) 
                {
                    if (dfsmStates[i][j] != newState[j]) 
                    {
                        found = 0;
                        break;
                    }
                }
                if (found) 
                {
                    foundIdx = i;
                    break;
                }
            }

            if (foundIdx == -1) 
            {
                // Add the new state to the DFSM
                for (int i = 0; i < numStates; i++) 
                {
                    dfsmStates[stateCount][i] = newState[i];
                }
                transitionTable[currentDFSMState][alphaIdx] = stateCount;  // Transition to the new state
                queue[rear++] = stateCount;  // Add the new state to the queue
                stateCount++;
            } 
            else 
            {
                // Transition to the existing state
                transitionTable[currentDFSMState][alphaIdx] = foundIdx;
            }
        }
    }

    // Handle accepting states
    int dfsmAcceptingCount = 0;
    for (int i = 0; i < stateCount; i++) 
    {
        for (int j = 0; j < numAcceptingStates; j++) 
        {
            for (int k = 0; dfsmStates[i][k] != -1; k++) 
            {
                if (dfsmStates[i][k] == acceptingStates[j]) 
                {
                    dfsmAcceptingStates[dfsmAcceptingCount++] = i;
                    break;
                }
            }
        }
    }

    // Write the DFSM to the output file
    writeDFSMtoFile(outputFile, alphabets, transitionTable, dfsmAcceptingStates, stateCount, numAlphabet, dfsmAcceptingCount);

}







// In this function , I had written the logic to read the NDFSM from file and store it in arrays
void readNDFSM(const char* filename, char* alphabets, int transitions[MAX_STATES][MAX_ALPHABET][MAX_TRANSITIONS], int* acceptingStates, int* numAlphabet, int* numStates, int* numAcceptingStates) 
{
    FILE* file = fopen(filename, "r");

    // Check if the file exists
    if (file == NULL) 
    {
        printf("Error: Could not open NDFSM file %s. File may not exist.\n", filename);
        exit(1);
    }

    char line[256];
    char* token;

    // Initialize number of states, alphabets, and accepting states
    *numAlphabet = 0;
    *numStates = 0;
    *numAcceptingStates = 0;

    // Read the first line containing the alphabet symbols
    if (fgets(line, sizeof(line), file) == NULL || strlen(line) == 0) 
    {
        printf("Error: Input file %s is empty.\n", filename);
        fclose(file);
        exit(1);
    }

    token = strtok(line, " \n");
    while (token != NULL) 
    {
        if (strlen(token) == 1 && token[0] >= 'a' && token[0] <= 'z') 
        {
            alphabets[*numAlphabet] = token[0];
            (*numAlphabet)++;
        }
        token = strtok(NULL, " \n");
    }

    if (*numAlphabet == 0) 
    {
        printf("Error: No alphabets found in the input file.\n");
        fclose(file);
        exit(1);
    }

    // Initialize the transitions array
    for (int i = 0; i < MAX_STATES; i++) 
    {
        for (int j = 0; j < MAX_ALPHABET; j++) 
        {
            for (int k = 0; k < MAX_TRANSITIONS; k++) 
            {
                transitions[i][j][k] = -2;  // Initialize to -2 indicating no transition
            }
        }
    }

    // Skip the empty line
    if (fgets(line, sizeof(line), file) == NULL) 
    {
        printf("Error: Unexpected end of file after the alphabet line.\n");
        fclose(file);
        exit(1);
    }

    // Read the transition table
    int currentState = 0;
    while (fgets(line, sizeof(line), file)) 
    {
        if (strlen(line) == 1) break;  // Stop at the empty line after the transition table

        if (currentState >= MAX_STATES) 
        {
            printf("Error: Number of states exceeds the maximum limit of %d.\n", MAX_STATES);
            fclose(file);
            exit(1);
        }

        int alphabetIndex = 0;
        char* ptr = line;

        while (alphabetIndex < *numAlphabet) 
        {
            // Find the opening bracket
            char* openBracket = strchr(ptr, '[');
            char* closeBracket = strchr(ptr, ']');

            if (openBracket && closeBracket && openBracket < closeBracket) 
            {
                // Check if it's an empty transition
                if (closeBracket - openBracket == 1) 
                {
                    transitions[currentState][alphabetIndex][0] = -1;  // Empty transition -> [-1]
                } 
                else 
                {
                    // Parse the contents between [ and ]
                    int transitionCount = 0;
                    char* innerToken = strtok(openBracket + 1, ",");
                    while (innerToken != NULL) 
                    {
                        int nextState = atoi(innerToken);
                        if (nextState == 0 || nextState < -1 || nextState > MAX_STATES) 
                        {
                            printf("Error: State %d exceeds the maximum allowed number of states (%d).\n", nextState, MAX_STATES);
                            fclose(file);
                            exit(1);
                        }
                        transitions[currentState][alphabetIndex][transitionCount++] = nextState;
                        if (transitionCount >= MAX_TRANSITIONS) 
                        {
                            printf("Error: Number of transitions exceeds the maximum limit of %d per input symbol.\n", MAX_TRANSITIONS);
                            fclose(file);
                            exit(1);
                        }
                        innerToken = strtok(NULL, ",");
                    }
                    transitions[currentState][alphabetIndex][transitionCount] = -2;  // End transition list
                }
                ptr = closeBracket + 1;  // Move past the closing bracket for the next transition
                alphabetIndex++;
            } 
            else 
            {
                printf("Error: Transition table format is incorrect.\n");
                fclose(file);
                exit(1);
            }
        }
        currentState++;
        (*numStates)++;
    }

    // Check the validity of transition states
    for (int i = 0; i < *numStates; i++) 
    {
        for (int j = 0; j < *numAlphabet; j++) 
        {
            for (int k = 0; transitions[i][j][k] != -2; k++) 
            {
                int state = transitions[i][j][k];
                if (state > *numStates) 
                {
                    printf("Error: Transition to state %d is invalid because it is beyond the defined states (valid states: 1 to %d).\n", state, *numStates);
                    fclose(file);
                    exit(1);
                }
            }
        }
    }

    // Read accepting states
    if (fgets(line, sizeof(line), file) != NULL) 
    {
        token = strtok(line, " ");
        while (token != NULL) 
        {
            int acceptingState = atoi(token);
            if (acceptingState < 1 || acceptingState > *numStates) 
            {  // Error: state out of range
                printf("Error: Accepting state %d exceeds the valid range of states (1 to %d).\n", acceptingState, *numStates);
                fclose(file);
                exit(1);
            }
            acceptingStates[*numAcceptingStates] = acceptingState;
            (*numAcceptingStates)++;
            token = strtok(NULL, " ");
        }
    } 
    else 
    {
        printf("Error: No accepting states found in the input file.\n");
        fclose(file);
        exit(1);
    }

    fclose(file);
}








// In this Function I written logic to remove duplicates and get unique characters from the input pattern
void getUniqueAlphabets(char *pattern, char *uniqueAlphabets) 
{
    int uniqueIndex = 0;
    int alphabetSeen[26] = {0};  // To track which alphabets have been seen

    for (int i = 0; pattern[i] != '\0'; i++) 
    {
        if (!alphabetSeen[pattern[i] - 'a']) 
        {
            uniqueAlphabets[uniqueIndex++] = pattern[i];
            alphabetSeen[pattern[i] - 'a'] = 1;
        }
    }
    uniqueAlphabets[uniqueIndex] = '\0';  // Null-terminate the string
}







// This is the function to check if the string contains only lowercase letters
int isValidPattern(const char *pattern) 
{
    for (int i = 0; pattern[i] != '\0'; i++) 
    {
        if (!islower(pattern[i])) return 0;
    }
    return 1;
}








// Main function
int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        printf("Should pass 3 arguments: %s <Pattern_file> <Input_file>\n", argv[0]);
        return 1;
    }

    // File pointers for input NDFSM.txt and Pattern.txt files
    FILE *ndfsmFile = fopen("NDFSM.txt", "w");  // Opening in "write" mode to overwrite the file
    FILE *patternFile = fopen(argv[1], "r");  
    FILE *inputFile = fopen(argv[2], "r");


    // Check if NDFSM.txt was successfully opened
    if (!ndfsmFile) 
    {
        printf("Error: NDFSM.txt could not be opened.\n");
        return 1;
    }
    
    // Check if Pattern.txt exists and is valid
    if (!patternFile) 
    {
        printf("Error: Pattern file %s does not exist.\n", argv[1]);
        fclose(ndfsmFile);
        return 1;
    }

    // Check if Input.txt exists and is valid
    if (!inputFile) 
    {
        printf("Error: Input file %s does not exist.\n", argv[2]);
        fclose(ndfsmFile);
        return 1;
    }

    char pattern[1000];  // Buffer for pattern (maximum size = 1000 say)
    if (!fgets(pattern, sizeof(pattern), patternFile)) 
    {
        printf("Error: Pattern file %s is empty or missing the pattern.\n", argv[2]);
        fclose(ndfsmFile);
        fclose(patternFile);
        fclose(inputFile);
        return 1;
    }

    // Remove newline from pattern, if it exists
    pattern[strcspn(pattern, "\n")] = '\0';

    // Validate that the pattern contains only lowercase alphabets
    if (!isValidPattern(pattern)) 
    {
        printf("Error: Pattern contains invalid characters. Only lowercase letters are allowed. Even spaces are not allowed\n");
        fclose(ndfsmFile);
        fclose(patternFile);
        fclose(inputFile);
        return 1;
    }

    char input[1000];  // Buffer for input string (maximum size = 1000 say)
    if (!fgets(input, sizeof(input), inputFile)) 
    {
        printf("Error: Input file %s is empty or missing the Input.\n", argv[2]);
        fclose(ndfsmFile);
        fclose(patternFile);
        fclose(inputFile);
        return 1;
    }

    // Remove newline from Input, if it exists
    input[strcspn(input, "\n")] = '\0';

    // Validate that the Input string contains only lowercase alphabets
    if (!isValidPattern(input)) 
    {
        printf("Error: Input string contains invalid characters. Only lowercase letters are allowed. Even spaces are not allowed\n");
        fclose(ndfsmFile);
        fclose(patternFile);
        fclose(inputFile);
        return 1;
    }


    // Print the input pattern before processing
    printf("Input Pattern: %s\n", pattern);

    // Print the input string before processing
    printf("Input String: %s\n", input);

    // Get unique alphabets from the pattern
    char uniqueAlphabets[27];  // Maximum 26 unique characters
    getUniqueAlphabets(pattern, uniqueAlphabets);

    // Section 1: Writing the unique alphabets to NDFSM.txt
    for (int i = 0; uniqueAlphabets[i] != '\0'; i++) 
    {
        fprintf(ndfsmFile, "%c ", uniqueAlphabets[i]);
    }
    fprintf(ndfsmFile, "\n\n");  // Two newlines: one after the alphabet, one empty line

    // Section 2: Writing the transition table
    int len = strlen(pattern);
    for (int i = 0; i <= len; i++) 
    {
        for (int j = 0; uniqueAlphabets[j] != '\0'; j++) 
        {
            char currentAlphabet = uniqueAlphabets[j];

            if (i == len) 
            {
                fprintf(ndfsmFile, "[%d] ", len + 1);  // Final state transitions to itself
            } 
            else 
            {
                if (pattern[i] == currentAlphabet) 
                {
                    if (i == 0) 
                    {
                        // First state (1): transitions to itself for all alphabets and to state 2 for the first alphabet
                        fprintf(ndfsmFile, "[%d,%d] ", i + 1, i + 2);  // Transition to self and next state
                    } 
                    else 
                    {
                        fprintf(ndfsmFile, "[%d] ", i + 2);  // Transition to the next state
                    }
                } 
                else 
                {
                    if (i == 0) 
                    {
                        fprintf(ndfsmFile, "[1] ");  // Initial state loops for non-matching characters
                    } 
                    else 
                    {
                        fprintf(ndfsmFile, "[-1] ");  // No transition for non-matching characters, replace [] with [-1]
                    }
                }
            }
        }
        fprintf(ndfsmFile, "\n");  // Newline after each row
    }

    // Section 3: Writing the accepting state
    fprintf(ndfsmFile, "\n%d\n", len + 1);

    printf("NDFSM written to %s successfully!\n", "NDFSM.txt");

    // Close files
    fclose(ndfsmFile);
    fclose(patternFile);

    //-----------------------------------------------------------------------------------------------------------------------------//


    // Arrays for NDFSM storage
    char alphabets[MAX_ALPHABET];
    int transitions[MAX_STATES][MAX_ALPHABET][MAX_TRANSITIONS];
    int acceptingStates[MAX_STATES];

    // Variables to track the number of states, alphabets, and accepting states
    int numAlphabet, numStates, numAcceptingStates;

    // Read the NDFSM from the file and store it in arrays
    readNDFSM("NDFSM.txt", alphabets, transitions, acceptingStates, &numAlphabet, &numStates, &numAcceptingStates);
    printf("Read NDFSM.txt successfully\n");


    //-----------------------------------------------------------------------------------------------------------------------------//

    // Converting NDFSM to DFSM and writing it to the DFSM.txt file
    convertNDFSMtoDFSM(alphabets, transitions, acceptingStates, numAlphabet, numStates, numAcceptingStates, "DFSM.txt");
    printf("Constructed DFSM.txt from the NDFSM.txt successfully\n");
    printf("DFSM written to %s successfully!\n", "DFSM.txt");

    //----------------------------------------------------------------------------------------------------------------------------//

    DFSM dfsm;
    memset(&dfsm, 0, sizeof(DFSM));

    readDFSMFile("DFSM.txt", &dfsm);

    char* inputString = readInputString(argv[2]);

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
