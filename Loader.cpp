
#include <vector>
#include <iostream>

#include "CPU.hpp"
#include "Instruction.hpp"


KStack<IOperand const *> STACK;
std::queue<Instruction *> CODE_SECTION;

void loader(std::vector<Instruction *> *list)
{
    CPU cpu;   
    for (auto i : *list)
        CODE_SECTION.push(i);
    cpu.run();
}