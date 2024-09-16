# NDFSM to DFSM Converter

## Overview
This project involves converting a **Non-Deterministic Finite State Machine (NDFSM)** into an equivalent **Deterministic Finite State Machine (DFSM)**. Given an NDFSM and its transition table, this program generates the equivalent DFSM that accepts the same language, ensuring the same pattern matching functionality but with deterministic transitions.

## Features
- Converts any valid NDFSM into an equivalent DFSM.
- Handles multiple states in NDFSM and efficiently consolidates them into single DFSM states.
- Produces a minimized DFSM transition table for simpler, deterministic pattern matching.
- Outputs the DFSM in a structured format that can be used for simulations or further analysis.

## How It Works
1. **Input**: You provide the NDFSM in a file (`NDFSM.txt`) that includes:
   - A set of input symbols (alphabet).
   - A transition table for each state in the NDFSM.
   - A list of accepting states.
   
2. **Conversion**: The program converts the NDFSM into a DFSM by:
   - Creating deterministic states based on the union of NDFSM states.
   - Mapping each transition to a specific state in the DFSM.
   - Ensuring that each DFSM state has a single transition for every input symbol.

3. **Output**: The DFSM is written to `DFSM.txt` with:
   - The alphabet used in the DFSM.
   - A transition table that defines deterministic transitions for each state and input symbol.
   - The list of accepting states in the DFSM.

## Input Format
The input file (`NDFSM.txt`) must follow a specific structure:
1. A line listing the input symbols (alphabet), separated by spaces.
2. A transition table where each state’s transitions are enclosed in square brackets. Multiple transitions for a symbol are separated by commas.
3. A final line listing the accepting states.

### Example NDFSM Input:
```
a b c

[1,2] [1] [1] 
[-1] [3] [-1] 
[-1] [-1] [4] 
[-1] [5] [-1] 
[5] [5] [5]

5
```

## Output Format
The output DFSM is written to `DFSM.txt` and includes:
1. **Alphabet**: The input symbols used by the DFSM.
    ```
    a b c
    ```

2. **Transition Table**: A deterministic transition table where each entry represents the state to which the DFSM transitions for each input symbol.
    ```
    2 1 1
    2 3 1
    2 1 4
    2 5 1
    6 5 5
    ```

3. **Accepting States**: The list of accepting states in the DFSM.
    ```
    5
    ```

## Usage
1. Provide an NDFSM in `NDFSM.txt`.
2. Compile and run the program. It will generate an equivalent DFSM and save it to `DFSM.txt`.

### Example Usage:
```bash
gcc NDFSMToDFSM.c -o NDFSMToDFSM
./NDFSMToDFSM NDFSM.txt DFSM.txt
```

## Error Handling
- If the input NDFSM file is malformed or contains invalid transitions (e.g., references to non-existent states), the program will throw an error and terminate.
- If no accepting states are provided, the program will return an error.
- The program validates that all states and transitions in the NDFSM are within the valid range and ensures proper handling of transition table formats.

## Files
- **NDFSMToDFSM.c**: The main program file responsible for converting NDFSM to DFSM.
- **NDFSM.txt**: The input file containing the NDFSM definition.
- **DFSM.txt**: The output file containing the equivalent DFSM definition.

## Example
For the given input `NDFSM.txt`:
```
a b c

[1,2] [1] [1] 
[-1] [3] [-1] 
[-1] [-1] [4] 
[-1] [5] [-1] 
[5] [5] [5]

5
```

The program generates the following `DFSM.txt`:
```
a b c

2 1 1
2 3 1
2 1 4
2 5 1
5 5 5

5
```

