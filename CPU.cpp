#include "CPU.hpp"

extern KStack<IOperand const *> STACK;
extern std::queue<Instruction *> CODE_SECTION;
const IOperand *CPU::ALU(eMnemonic_type _mnemonic, const IOperand *a, const IOperand *b)
{
    static std::vector<std::function<const IOperand *(const IOperand *,const IOperand *)>> cal = {
        [](const IOperand *a, const IOperand *b){ return *a + *b;},
        [](const IOperand *a, const IOperand *b){ return *a - *b;},
        [](const IOperand *a, const IOperand *b){ return *a * *b;},
        [](const IOperand *a, const IOperand *b){ return *a / *b;},
        [](const IOperand *a, const IOperand *b){ return *a % *b;}
    };
    return cal[_mnemonic](a, b);
}

void        CPU::CU(void) {
    static Factory  _factory;
    const IOperand *rax;
    const IOperand *rdi;
    const IOperand *rsi;
   
    while (!CODE_SECTION.empty())
    {
        Instruction     *cInstruction = CODE_SECTION.front();
        std::vector<std::string>        print_list;
        CODE_SECTION.pop();
        eMnemonic_type  cMnemonic = cInstruction->_mnemonic;
        eOperandType    cOperandType;
    
        switch (cMnemonic)
        {
            case ADD:
            case DIV:
            case SUB:
            case MOD:
            case MUL:
                if (STACK.size() < 2)
                    throw std::runtime_error("Stack has less than 2 elements");
                if (STACK.size())
                    rdi = STACK.top();
                else
                    throw std::runtime_error("Invalid Instruction");
                STACK.pop();
                if (STACK.size())
                    rsi = STACK.top();
                else
                    throw std::runtime_error("Invalid Instruction");
                STACK.pop();
                rax = ALU(cMnemonic, rsi, rdi);
                STACK.push(rax);
                break;
            case PUSH:
                cOperandType = cInstruction->_operand_type;
                if (cOperandType == INT8)
                    STACK.push(_factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int8)));
                else if (cOperandType == INT16)
                    STACK.push(_factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int16)));
                else if (cOperandType == INT32)
                    STACK.push(_factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int32)));
                else if (cOperandType == FLOAT)
                    STACK.push(_factory.createOperand(cOperandType, std::to_string(cInstruction->_register._float)));
                else if (cOperandType == DOUBLE)
                    STACK.push(_factory.createOperand(cOperandType, std::to_string(cInstruction->_register._double)));
                else
                    std::runtime_error("Wrong Operand type");
                break ;
            case POP:
                if (!STACK.empty())
                    STACK.pop();
                else
                    throw std::runtime_error("pop on empty stack");
                break ;
            case DUMP:
                for (auto i : STACK)
                    print_list.push_back(i->toString());
                for (auto it = print_list.rbegin(); it != print_list.rend(); ++it)
                {   
                    if ((*it).find('.') != std::string::npos)
                        (*it).erase ((*it).find_last_not_of('0') + 1, std::string::npos );
                    std::cout << *it << std::endl;
                }
                break ;
            case ASSERT:
                cOperandType = cInstruction->_operand_type;
                if (STACK.size())
                    rdi = STACK.top();
                else
                    throw std::runtime_error("Invalid Instruction");
                if (cOperandType == INT8)
                    rsi = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int8));
                else if (cOperandType == INT16)
                    rsi = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int16));
                else if (cOperandType == INT32)
                    rsi = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int32));
                else if (cOperandType == FLOAT)
                    rsi = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._float));
                else if (cOperandType == DOUBLE)
                    rsi = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._double));
                else
                    throw std::runtime_error("Wrong Operand type");
                if (rdi->getVal() != rsi->getVal())
                    throw std::runtime_error("Assert failed");
                break ;
            case PRINT:
                 if (STACK.size())
                    rdi = STACK.top();
                else
                    throw std::runtime_error("Invalid Instruction");
                if (rdi->getPrecision() == INT8)
                {
                    char c = static_cast<int8_t>(rdi->getVal());
                    std::cout << c;
                }
                else
                    std::runtime_error("Print error");

                break ;
            case EXIT:
                if (CODE_SECTION.size() != 0)
                    throw std::runtime_error("Exit error");
                ::exit(0);
                break ;
            default:
                break ;
        }
    }
}

void        CPU::run(void)
{
    int8_t maxLine = CODE_SECTION.size();
    if (CODE_SECTION.size() == 0)
        std::cout << "\x1b[31mLine " <<  0 << " ERROR: " << "No Exit instruction" << "\x1b[0m" << std::endl;
    try {
        CU();
    } catch(std::exception &e) {
        std::cout << "\x1b[31mLine " <<  (maxLine - CODE_SECTION.size()) << " ERROR: " << e.what() << "\x1b[0m" << std::endl;
    }
}