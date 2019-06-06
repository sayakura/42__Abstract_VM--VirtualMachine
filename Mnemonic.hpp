#ifndef MNEMONIC_HPP

#define MNEMONIC_HPP
#include "Token.hpp"

enum eMnemonic_type
{
    ADD, // 0
    SUB, // 1
    MUL, // 2
    DIV, // 3
    MOD, // 4
    PUSH,
    POP,
    DUMP,
    ASSERT,
    PRINT,
    EXIT
};

class Mnemonic: public Token
{
    eMnemonic_type  m_type;
    Mnemonic(void);
    public:
        Mnemonic(eMnemonic_type);
        eToken_type getTokenType(void);
        eMnemonic_type  getMnemonicType(void);
};

#endif