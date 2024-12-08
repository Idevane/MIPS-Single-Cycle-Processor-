# MIPS Single-Cycle Processor Emulator

## Overview

This project is a MIPS single-cycle processor emulator written in C++. It simulates and disassembles MIPS instructions from a binary input file. The emulator supports a range of instructions, including arithmetic, logical, and memory operations, and outputs the disassembled instructions into a specified text file.

## Features

- Reads binary MIPS instructions from a file.
- Decodes instructions into human-readable MIPS assembly.
- Supports the following MIPS instruction types:
  - R-Type: ADD, SUB, SLL, SRL, AND, OR, JR, NOP
  - I-Type: ADDI, LW, SW, BEQ, BLTZ
  - J-Type: J
  - Special: MUL
- Outputs the disassembled instructions to a file in a formatted manner.
- Handles invalid instructions gracefully.

### Build

To compile the program, use the provided `Makefile`. Run the following command:

```bash
make
```

This generates an executable named `mipssim`.

### Run

To run the program, use the following command:

```bash
./mipssim -i INPUTFILENAME -o OUTPUTFILENAME
```

- `INPUTFILENAME`: The file containing binary MIPS instructions.
- `OUTPUTFILENAME`: The base name of the output file. The program appends `_dis.txt` to create the disassembly output file.

### Example

```bash
./mipssim -i test1bin -o test1
```

This command processes the `test1bin` file and outputs the disassembly to `test1_dis.txt`.

## Code Structure

1. **Instruction Decoding**: Functions like `decodeLWInstruction`, `decodeSWInstruction`, etc., handle specific instruction types.
2. **Main Processing**: The `main` function parses arguments, reads the binary file, decodes instructions, and writes the output to a file.

## Requirements

- C++11 or higher
- A Makefile-compatible build environment

## Notes

- Ensure the binary input file follows the format expected by the program.
- Invalid instructions will be labeled as `Invalid Instruction` in the output.

## Contributing

Feel free to fork this repository, submit issues, or contribute by creating pull requests.

## License

This project is licensed under the MIT License.

## Contact

- **Author:** Isaac DeVaney  
- **Email:** isaacdevaney@gmail.com
- **GitHub:** idevane

