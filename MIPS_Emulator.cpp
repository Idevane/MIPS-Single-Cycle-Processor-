#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

int binaryToRegister(const string& binary) {
    return stoi(binary, nullptr, 2);
}

string formatRTypeInstruction(const string& opcode, const string& rs,
                                   const string& rt, const string& rd,
                                   const string& shamt, const string& funct) {
    int rs_num = binaryToRegister(rs);
    int rt_num = binaryToRegister(rt);
    int rd_num = binaryToRegister(rd);
    int shamt_num = binaryToRegister(shamt);

    string instruction;
    if (funct == "100100") instruction = "AND";
    else if (funct == "100101") instruction = "OR";
    else if (funct == "000000" && rs == "00000" && rt == "00000" && rd == "00000" && shamt == "00000") instruction = "NOP"; // Identifying NOP
    else if (funct == "000000") instruction = "SLL";
    else if (funct == "000010") instruction = "SRL";
    else if (funct == "100000") instruction = "ADD";
    else if (funct == "100010") instruction = "SUB";
    else if (funct == "001000") instruction = "JR";
    else instruction = "BREAK";

    char result[50];
    if (instruction == "NOP") {
        snprintf(result, sizeof(result), "%s", instruction.c_str());
    } else if (instruction == "JR") {
        snprintf(result, sizeof(result), "%s $%d", instruction.c_str(), rs_num);
    } else {
        snprintf(result, sizeof(result), "%s $%d, $%d, $%d", instruction.c_str(), rd_num, rs_num, rt_num);
        if (instruction == "SLL" || instruction == "SRL") {
            snprintf(result, sizeof(result), "%s R%d, $R%d, #%d", instruction.c_str(), rd_num, rt_num, shamt_num);
        }
    }
    return string(result);
}

string decodeSWInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string base = binary.substr(6, 5);
    string rt = binary.substr(11, 5);
    string offset = binary.substr(16, 16);

    int base_num = binaryToRegister(base);
    int rt_num = binaryToRegister(rt);
    int offset_num = stoi(offset, nullptr, 2);

    char result[50];
    snprintf(result, sizeof(result), "SW R%d, #%d(R%d)", rt_num, offset_num, base_num);
    return string(result);
}
// Decod eJ instruction
string decodeJInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string index = binary.substr(6, 26);
    int index_num = binaryToRegister(index);
    char result[50];
    snprintf(result, sizeof(result), "j #%d", index_num);
    return string(result);
}

// Decode BEQinstruction
string decodeBEQInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string rs = binary.substr(6, 5);
    string rt = binary.substr(11, 5);
    string offset = binary.substr(16, 16);
    int rs_num = binaryToRegister(rs);
    int rt_num = binaryToRegister(rt);
    int offset_num = binaryToRegister(offset);
    char result[50];
    snprintf(result, sizeof(result), "beq R%d, R%d, #%d", rs_num, rt_num, offset_num);
    return string(result);
}

// Decode BLTZ instruction
string decodeBLTZInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string rs = binary.substr(6, 5);
    string offset = binary.substr(16, 16);
    int rs_num = binaryToRegister(rs);
    int offset_num = binaryToRegister(offset);
    char result[50];
    snprintf(result, sizeof(result), "bltz R%d, #%d", rs_num, offset_num);
    return string(result);
}

// Decode oad word 
string decodeLWInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string base = binary.substr(6, 5); 
    string rt = binary.substr(11, 5); 
 string offset = binary.substr(16, 16); 

    int base_num = binaryToRegister(base); 
    int rt_num = binaryToRegister(rt); 
    int offset_num = stoi(offset, nullptr, 2); 

    if (offset_num & (1 << 15)) {
        offset_num -= (1 << 16); 
    }

    char result[50];
    snprintf(result, sizeof(result), "lw R%d, %d(R%d)", rt_num, offset_num, base_num);
    return string(result);
}

string decodeMULInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string rs = binary.substr(6, 5);
    string rt = binary.substr(11, 5);
    string rd = binary.substr(16, 5);
    int rs_num = binaryToRegister(rs);
    int rt_num = binaryToRegister(rt);
    int rd_num = binaryToRegister(rd);
    char result[50];
    snprintf(result, sizeof(result), "mul R%d, R%d, R%d", rd_num, rs_num, rt_num);
    return string(result);
}
string decodeADDIInstruction(const string& binary) {
    if (binary[0] == '0') return "NOP";
    string rs = binary.substr(6, 5);
    string rt = binary.substr(11, 5);
    string immediate = binary.substr(16, 16);

    int rs_num = binaryToRegister(rs);
    int rt_num = binaryToRegister(rt);
    int immediate_num = stoi(immediate, nullptr, 2);
    // negative number
    if (immediate_num & (1 << 15)) { 
        immediate_num -= (1 << 16); 
    }

    char result[50];
    snprintf(result, sizeof(result), "addi R%d, R%d, $%d", rt_num, rs_num, immediate_num);
    return string(result);
}

string binaryToMIPS(const string& binary) {
    char validBit = binary[0]; // valid bit

    string opcode = binary.substr(1, 5);
    string rs = binary.substr(6, 5);
    string rt = binary.substr(11, 5);
    string rd = binary.substr(16, 5);
    string shamt = binary.substr(21, 5);
    string funct = binary.substr(26, 6);


    if (validBit == '0') {
        return "NOP";
    } else if (opcode == "00000") { // R-type 
        return formatRTypeInstruction(opcode, rs, rt, rd, shamt, funct);
    } else if (opcode == "01011") { // SW 
        return decodeSWInstruction(binary);
    } else if (opcode == "00011") { // LW 
        return decodeLWInstruction(binary);
    } else if (opcode == "00010") { // J 
        return decodeJInstruction(binary);
    } else if (opcode == "00100") { // BEQ 
        return decodeBEQInstruction(binary);
    } else if (opcode == "00001") { // BLTZ 
        return decodeBLTZInstruction(binary);
    } else if (opcode == "00011") { // LW 
        return decodeLWInstruction(binary);
    } else if (opcode == "11100") { // MUL 
        return decodeMULInstruction(binary);
    } else if (opcode == "01000") { // ADDI 
        return decodeADDIInstruction(binary);
        }else {
        return "INVALID INSTRUCTION";
    }
}



int main(int argc, char* argv[]) {
    string inputFileName;
    string outputFileName;

    // command-line arguments
    for (int i = 1; i < argc; i += 2) {
        if (string(argv[i]) == "-i" && i + 1 < argc) {
            inputFileName = argv[i + 1];

 } else if (string(argv[i]) == "-o" && i + 1 < argc) {
            outputFileName = argv[i + 1] + string("_dis.txt");
        } else {
            cerr << "Invalid arguments." << endl;
            cerr << "Usage: " << argv[0] << " -i INPUTFILENAME -o OUTPUTFILENAME" << endl;
            return 1;
        }
    }

    if (inputFileName.empty() || outputFileName.empty()) {
        cerr << "Both input file name and output file base name are required." << endl;
        cerr << "Usage: " << argv[0] << " -i INPUTFILENAME -o OUTPUTFILENAME" << endl;
        return 1;
    }

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    string binInstruction;
    int address = 96; // Starting address
    while (getline(inputFile, binInstruction)) {
        char validBit = binInstruction[0]; 

        // Process the binary instruction and get the decoded MIPS instruction
        string decodedInstruction = binaryToMIPS(binInstruction);

        string formattedOutput;
        if (validBit == '0' || decodedInstruction == "INVALID INSTRUCTION") {
            formattedOutput = "0 " + binInstruction.substr(1) + " " + to_string(address) + " Invalid Instruction";
        } else {
            formattedOutput = "1 " + binInstruction.substr(1) + " " + to_string(address) + " " + decodedInstruction;
        }

        outputFile << formattedOutput << endl; // Output the decoded instruction to the file
        address += 4; // Update the address for the next instruction
    }

    inputFile.close();
    outputFile.close();

    cout << "Disassembly written to " << outputFileName << endl;
    return 0;
}