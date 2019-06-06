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
    private:
        // general purpose registers
        int64_t     _rdi;
        int64_t     _rsi;
    public:
        // arithmetic logic unit 
        int64_t     ALU(eMnemonic_type, IOperand*, IOperand*);
        // control unit 
        void        CU(void);

        void        run(void);
};
#endif