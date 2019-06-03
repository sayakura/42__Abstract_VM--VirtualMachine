#include "Mnemonic.hpp"

#include <iostream>
#include <string>
Mnemonic::Mnemonic(int type) : Token(tMnemonic), m_type(type)  {
};

int  Mnemonic::getMnemonicType(void) {
    return m_type; }