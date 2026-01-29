#include <iostream>
#include <stdint-gcc.h>
using namespace std;


#include <cstdint>
#include <iostream>

enum Opcode {
    OP_MOV = 1,
    OP_ADD,
    OP_SUB,
    OP_CMP,
    OP_JNZ,
    OP_HALT
};

struct VM
{
    uint32_t regs[8]{};
    uint8_t memory[65536]{};
    uint32_t pc = 0;
    int32_t cmp = 0;
    bool running = true;

    // --- Helpers ---
    uint32_t fetch32()
    {
        uint32_t val = 0;
        val |= memory[pc];
        val |= memory[pc + 1] << 8;
        val |= memory[pc + 2] << 16;
        val |= memory[pc + 3] << 24;
        return val;
    }

    void step()
    {
        uint32_t instr = fetch32();

        uint8_t opcode = instr >> 24;
        uint8_t rd     = (instr >> 20) & 0xF;
        uint8_t rs     = (instr >> 16) & 0xF;
        uint16_t imm   = instr & 0xFFFF;

        pc += 4;

        switch (opcode)
        {
            case OP_MOV:
                regs[rd] = imm;
                break;

            case OP_ADD:
                regs[rd] += regs[rs];
                break;

            case OP_SUB:
                regs[rd] -= regs[rs];
                break;

            case OP_CMP:
                cmp = regs[rd] - regs[rs];
                break;

            case OP_JNZ:
                if (cmp != 0)
                    pc = imm;
                break;

            case OP_HALT:
                running = false;
                break;

            default:
                std::cerr << "Unknown opcode: " << int(opcode) << "\n";
                running = false;
        }
    }

    void run()
    {
        while (running)
            step();
    }
};


int main()
{

    VM vm;

    uint32_t program[] = {
        (OP_MOV << 24) | (0 << 20) | 0,
        (OP_MOV << 24) | (1 << 20) | 5,
        (OP_ADD << 24) | (0 << 20) | (1 << 16),
        (OP_HALT << 24)
    };

    memcpy(vm.memory, program, sizeof(program));
    vm.run();

    std::cout << "R0 = " << vm.regs[0] << "\n"; // should print 5


}
