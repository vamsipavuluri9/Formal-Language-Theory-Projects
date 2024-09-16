// Name: Vamsi Krishna Pavuluri
// CWID: A20449747
// Author: Vamsi Krishna Pavuluri

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 26
#define MAX_TRANSITIONS 10



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







// Function to print the NDFSM to the console
void printNDFSM(char* alphabets, int transitions[MAX_STATES][MAX_ALPHABET][MAX_TRANSITIONS], int* acceptingStates, int numAlphabet, int numStates, int numAcceptingStates) 
{
    printf("Alphabets:\n");
    for (int i = 0; i < numAlphabet; i++) 
    {
        printf("%c ", alphabets[i]);
    }
    printf("\n\nNDFSM Transition Table:\n");

    for (int i = 0; i < numStates; i++) 
    {
        for (int j = 0; j < numAlphabet; j++) 
        {
            printf("[");
            if (transitions[i][j][0] == -1) 
            {
                printf("-1");
            } 
            else 
            {
                for (int k = 0; transitions[i][j][k] != -2; k++) 
                {
                    if (k > 0) printf(",");
                    printf("%d", transitions[i][j][k]);
                }
            }
            printf("] ");
        }
        printf("\n");
    }

    printf("\nAccepting States:\n");
    for (int i = 0; i < numAcceptingStates; i++) 
    {
        printf("%d ", acceptingStates[i]);
    }
    printf("\n");
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







// Function to print DFSM to the console
void printDFSMConsole(char* alphabets, int transitionTable[MAX_STATES][MAX_ALPHABET], int* dfsmAcceptingStates, int stateCount, int numAlphabet, int dfsmAcceptingCount) 
{
    printf("\nDFSM Transition Table:\n");
    for (int i = 0; i < stateCount; i++) 
    {
        for (int j = 0; j < numAlphabet; j++) 
        {
            printf("%d ", transitionTable[i][j] + 1);  // +1 to display state numbering from 1
        }
        printf("\n");
    }

    // Print accepting states of the DFSM
    printf("\nDFSM Accepting States:\n");
    for (int i = 0; i < dfsmAcceptingCount; i++) 
    {
        printf("%d ", dfsmAcceptingStates[i] + 1);  // +1 to display accepting states starting from 1
    }
    printf("\n");
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

    // Print DFSM to the console without brackets
    printDFSMConsole(alphabets, transitionTable, dfsmAcceptingStates, stateCount, numAlphabet, dfsmAcceptingCount);
}








// Main Function
int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        printf("Usage: %s <NDFSM_file> <DFSM_file>\n", argv[0]);
        return 1;
    }

    // Arrays for NDFSM storage
    char alphabets[MAX_ALPHABET];
    int transitions[MAX_STATES][MAX_ALPHABET][MAX_TRANSITIONS];
    int acceptingStates[MAX_STATES];

    // Variables to track the number of states, alphabets, and accepting states
    int numAlphabet, numStates, numAcceptingStates;

    // Read the NDFSM from the file and store it in arrays
    readNDFSM(argv[1], alphabets, transitions, acceptingStates, &numAlphabet, &numStates, &numAcceptingStates);

    // Print the NDFSM
    printNDFSM(alphabets, transitions, acceptingStates, numAlphabet, numStates, numAcceptingStates);

    // Convert NDFSM to DFSM and write it to the output file
    convertNDFSMtoDFSM(alphabets, transitions, acceptingStates, numAlphabet, numStates, numAcceptingStates, argv[2]);

    return 0;
}
