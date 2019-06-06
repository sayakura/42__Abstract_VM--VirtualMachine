#include "CPU.hpp"

extern KStack<IOperand const *> STACK;
extern std::queue<Instruction *> CODE_SECTION;
// arithmetic logic unit 
const IOperand *ALU(eMnemonic_type _mnemonic, const IOperand *a, const IOperand *b)
{
    const IOperand *ret;
    static std::unordered_map< eMnemonic_type , std::function<const IOperand *(IOperand *,IOperand *)> > dispTable{
    {ADD,[](IOperand *a, IOperand *b){ return *a + *b;} },
    {SUB,[](IOperand *a, IOperand *b){ return *a - *b;} },
    {MUL,[](IOperand *a, IOperand *b){ return *a * *b;} },
    {MOD,[](IOperand *a, IOperand *b){ return *a % *b;} },
    {DIV,[](IOperand *a, IOperand *b){ return *a / *b;} } };

    return dispTable[_mnemonic](a, b);
}

// control unit 
void        CPU::CU(void) {
    static Factory  _factory;
    eMnemonic_type cMnemonic;
    eOperandType cOperandType;
    IOperand *a;
    IOperand *b;
    // if (STACK.size() < 2)
    //     throw std::runtime_error("Not enough elements on the stack.");
    // const IOperand *a = STACK.top();
    // STACK.pop();
    // const IOperand *b = STACK.top();
    // STACK.pop();
    
    // while the code section is not empty

    // get instruction from the code_section
    std::cout << "CU is running :)" << std::endl;
    std::cout << "instruction cnt :" << CODE_SECTION.size() << std::endl;
    while (!CODE_SECTION.empty())
    {
        Instruction *cInstruction = CODE_SECTION.front();
        CODE_SECTION.pop();
        cMnemonic = cInstruction->_mnemonic;
        switch (cMnemonic)
        {
            case ADD:
            case DIV:
            case SUB:
            case MOD:
            case MUL:
                if (STACK.size() < 2)
                    throw std::runtime_error("");
                const IOperand *tmpA = STACK.top();
                STACK.pop();
                const IOperand *tmpB = STACK.top();
                STACK.pop();
                ALU(cMnemonic, tmpB, tmpA);
    
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
                    std::runtime_error("Stack is empty");
                break ;
            case DUMP:
                for (auto i : STACK)
                    std::cout << i->toString() << std::endl;
                break ;
            case ASSERT:
                cOperandType = cInstruction->_operand_type;
                const IOperand *tmpA = STACK.top();
                const IOperand *tmpB;

                if (cOperandType == INT8)
                    tmpB = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int8));
                else if (cOperandType == INT16)
                    tmpB = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int16));
                else if (cOperandType == INT32)
                    tmpB = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._int32));
                else if (cOperandType == FLOAT)
                    tmpB = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._float));
                else if (cOperandType == DOUBLE)
                    tmpB = _factory.createOperand(cOperandType, std::to_string(cInstruction->_register._double));
                else
                    std::runtime_error("Wrong Operand type");
                if (tmpA != tmpB)
                    std::runtime_error("Assert failed");
                break ;
            case PRINT:
                const IOperand *tmpA = STACK.top();
                if (tmpA->getPrecision() == INT8)
                {
                    char c = static_cast<int8_t>(tmpA->getVal());
                    std::cout << c << std::endl;
                }
                else
                    std::runtime_error("Print error");
                break ;
            case EXIT:
                if (STACK.size() != 1)
                    std::runtime_error("Exit error");
                ::exit(0);
                break ;
            default:
                break ;
        }
        STACK.pop();
    }
}


void        CPU::run(void)
{
    CU();
}