#ifndef CPU_HPP
#define CPU_HPP
#include "Mnemonic.hpp"
#include "IOperand.hpp"
#include "Instruction.hpp"
#include "stdint.h"
#include "KStack.hpp"
#include <stack>
#include <exception>
#include <queue>
#include <functional>
#include <unordered_map>

class CPU {
    public:
        // arithmetic logic unit 
        const IOperand *ALU(eMnemonic_type, const IOperand *, const IOperand *);
        // control unit 
        void        CU(void);
        void        run(void);
};
#endif