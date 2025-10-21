# DFSM Simulator

Here I provided the implementation of a Deterministic Finite State Machine (DFSM) simulator. The simulator reads a DFSM specification from a file and an input string from another file, processes the input according to the DFSM's rules, and determines whether the input string is accepted or rejected.

## Features

- **DFSM Parsing**: Reads the DFSM definition from a file, including the input alphabet, transition table, and accepting states.
- **Dynamic Memory Allocation**: Handles DFSMs and input strings of varying lengths using dynamic memory allocation.
- **Error Handling**: Validates the structure of the DFSM definition, checks for invalid states, and ensures proper transition rules.
- **Input Processing**: Reads the input string from a file and simulates the DFSM to check if the input is accepted or rejected.
- **Epsilon String Handling**: The simulator handles epsilon (empty) strings based on the DFSM’s accepting states.

## How It Works

1. **DFSM Input Format**: 
   The DFSM specification is provided in a text file with the following format:
   
   - The first section contains the alphabet symbols (e.g., `a b c`).
   - The second section contains the transition table, where each row represents the state transitions for the alphabet symbols.
   - The third section contains the accepting states (e.g., `1 2`).
   
   Example DFSM file:
   ```
   a b

   2 1
   3 2
   1 3

   1
   ```

2. **Input String**: 
   The input string is provided in a separate file, which the simulator reads to determine if it’s accepted by the DFSM.

3. **Output**: 
   The simulator outputs `yes` if the input string is accepted by the DFSM and `no` if it’s rejected.

## Usage

### Command-line Execution

To run the DFSM simulator, use the following command:

```bash
./a.out <dfsm_file> <input_file>
```

- `<dfsm_file>`: Path to the DFSM specification file.
- `<input_file>`: Path to the file containing the input string.

### Example

Given a DFSM specification in `DFSM.txt` and an input string in `input.txt`:

```bash
./a.out DFSM.txt input.txt
```

The program will output `yes` or `no` based on whether the DFSM accepts the input string.

### Error Handling

- If the input DFSM file or input string file is missing or malformed, appropriate error messages will be printed.
- Handles cases where the input string consists of only whitespace (considered an epsilon string) and checks if the initial state is in the list of accepting states.

## Code Overview

- **`readDFSMFile()`**: Parses the DFSM file and dynamically allocates memory for the alphabet, transition table, and accepting states.
- **`readInputString()`**: Reads the input string from a file, handling various edge cases such as empty strings or invalid input.
- **`DFSMsimulator()`**: Simulates the DFSM and determines whether the input string is accepted or rejected.
- **`printDFSM()`**: Prints the details of the parsed DFSM, including the input alphabet, transition table, and accepting states.

## Requirements

- **C Compiler**: Ensure that you have a C compiler (such as GCC) installed to compile the code.
- **Makefile (Optional)**: You can create a `Makefile` for easier compilation.

### Compilation

Use the following command to compile the code:

```bash
gcc -o dfsm_simulator dfsm_simulator.c
```

### Running the Simulator

```bash
./dfsm_simulator DFSM.txt input.txt
```
