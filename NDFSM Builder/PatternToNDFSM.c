// Name: Vamsi Krishna Pavuluri
// CWID: A20449747
// Author: Vamsi Krishna Pavuluri

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        printf("Should pass 3 arguments: %s <NDFSM_file> <Pattern_file>\n", argv[0]);
        return 1;
    }

    // File pointers for input NDFSM.txt and Pattern.txt files
    FILE *ndfsmFile = fopen(argv[1], "w");  // Opening in "write" mode to overwrite the file
    FILE *patternFile = fopen(argv[2], "r");

    // Check if NDFSM.txt was successfully opened
    if (!ndfsmFile) 
    {
        printf("Error: NDFSM file %s could not be opened.\n", argv[1]);
        return 1;
    }
    
    // Check if Pattern.txt exists and is valid
    if (!patternFile) 
    {
        printf("Error: Pattern file %s does not exist.\n", argv[2]);
        fclose(ndfsmFile);
        return 1;
    }

    char pattern[1000];  // Buffer for pattern (maximum size = 1000 say)
    if (!fgets(pattern, sizeof(pattern), patternFile)) 
    {
        printf("Error: Pattern file %s is empty or missing the pattern.\n", argv[2]);
        fclose(ndfsmFile);
        fclose(patternFile);
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
        return 1;
    }

    // Print the input string (pattern) before processing
    printf("Input String: %s\n", pattern);

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

    printf("NDFSM written to %s successfully!\n", argv[1]);

    // Close files
    fclose(ndfsmFile);
    fclose(patternFile);

    return 0;
}
