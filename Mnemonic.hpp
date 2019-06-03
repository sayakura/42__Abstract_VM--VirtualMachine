#ifndef MNEMONIC_HPP

#define MNEMONIC_HPP
#include "Token.hpp"

typedef enum Mnemonic_type
{
    ADD,
    SUB,
    MUL,
    DIV,
    PUSH,
    POP,
    DUMP,
    ASSERT,
    MOD,
    PRINT,
    EXIT
}            eMnemonic_type;

class Mnemonic: public Token
{
    int  m_type;
    Mnemonic(void);
    public:
        Mnemonic(int);
        int  getMnemonicType(void);
};

#endif