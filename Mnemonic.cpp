#include "Mnemonic.hpp"

#include <iostream>
#include <string>
Mnemonic::Mnemonic(eMnemonic_type type) : m_type(type) {};

eMnemonic_type  Mnemonic::getMnemonicType(void) {
    return m_type; }

eToken_type     Mnemonic::getTokenType(void) {
    std::cout << "m ";
    return tMnemonic; }