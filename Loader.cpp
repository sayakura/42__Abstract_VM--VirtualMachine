
#include <vector>
#include <iostream>

#include "unistd.h"
#include "CPU.hpp"
#include "Instruction.hpp"


std::stack<IOperand const *> STACK;
std::queue<Instruction *> CODE_SECTION;

void loader(std::vector<Instruction *> *list)
{
    if (list->size() && (*list)[list->size() - 1]->_mnemonic != EXIT)
    {
        std::cout << "\x1b[31mERROR: " << "No Exit instruction at the end" << "\x1b[0m" << std::endl;
        ::exit(EXIT_FAILURE);
    }
    CPU cpu;   
    for (auto i : *list)
        CODE_SECTION.push(i);
    cpu.run();
}