#ifndef INTERPRETER_HPP
# define INTERPRETER_HPP

#include <istream>
#include <exception>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <regex>
#include <string.h>
#include <queue>
#include <unordered_map>
#include <algorithm> // any_of 

#include "Token.hpp"
#include "Helper.hpp"
#include "Mnemonic.hpp"
#include "IOperand.hpp"
#include "Operand.hpp"
#include "Instruction.hpp"

class Compiler
{

private:
    int status;
    std::string _filename;
public:
    std::vector<Instruction *> *run(std::istream*, char *filename);
    std::vector<Token *>    *tokenization(std::istream &in);
    std::vector<Token *>    *lexer(std::string &str);
    std::vector<Instruction *> *parser(std::vector<Token *> *);
};

#endif