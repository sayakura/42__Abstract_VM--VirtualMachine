#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Operand.hpp"
#include "Mnemonic.hpp"

struct Instruction
{
    eMnemonic_type  _mnemonic;
    eOperandType    _operand_type;
    union REGISTER {
        int8_t _int8;
        int16_t _int16;
        int32_t _int32;
        float _float;
        double _double;
    }               _register;
};

#endif