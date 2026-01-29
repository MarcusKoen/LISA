#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cstdint>

enum Opcode {
    OP_MOV = 1,
    OP_ADD,
    OP_SUB,
    OP_CMP,
    OP_JNZ,
    OP_HALT
};

uint8_t regIndex(const std::string& r)
{
    if (r[0] != 'R') throw std::runtime_error("Invalid register");
    return std::stoi(r.substr(1));
}

uint32_t encode(uint8_t op, uint8_t rd = 0, uint8_t rs = 0, uint16_t imm = 0)
{
    return (op << 24) | (rd << 20) | (rs << 16) | imm;
}

int main()
{
    std::ifstream file("program.asm");
    if (!file) return 1;

    std::vector<std::string> lines;
    std::unordered_map<std::string, uint32_t> labels;

    std::string line;
    uint32_t pc = 0;

    // ---- PASS 1: read lines + labels
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        if (line.back() == ':')
        {
            labels[line.substr(0, line.size() - 1)] = pc;
        }
        else
        {
            lines.push_back(line);
            pc += 4;
        }
    }

    // ---- PASS 2: assemble
    std::vector<uint32_t> program;

    for (auto& l : lines)
    {
        std::stringstream ss(l);
        std::string op;
        ss >> op;

        if (op == "MOV")
        {
            std::string r;
            int imm;
            ss >> r;
            ss.ignore(1); // comma
            ss >> imm;
            program.push_back(encode(OP_MOV, regIndex(r), 0, imm));
        }
        else if (op == "ADD")
        {
            std::string r1, r2;
            ss >> r1;
            ss.ignore(1);
            ss >> r2;
            program.push_back(encode(OP_ADD, regIndex(r1), regIndex(r2)));
        }
        else if (op == "SUB")
        {
            std::string r1, r2;
            ss >> r1;
            ss.ignore(1);
            ss >> r2;
            program.push_back(encode(OP_SUB, regIndex(r1), regIndex(r2)));
        }
        else if (op == "CMP")
        {
            std::string r1, r2;
            ss >> r1;
            ss.ignore(1);
            ss >> r2;
            program.push_back(encode(OP_CMP, regIndex(r1), regIndex(r2)));
        }
        else if (op == "JNZ")
        {
            std::string label;
            ss >> label;
            program.push_back(encode(OP_JNZ, 0, 0, labels[label]));
        }
        else if (op == "HALT")
        {
            program.push_back(encode(OP_HALT));
        }
        else
        {
            throw std::runtime_error("Unknown instruction: " + op);
        }
    }

    // ---- Output
    for (auto i : program)
        std::cout << std::hex << i << "\n";
}
