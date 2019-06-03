#ifndef INTERPRETER_HPP
# define INTERPRETER_HPP

#include <istream>
#include <exception>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <algorithm> // any_of 

#include "Token.hpp"
#include "Helper.hpp"
#include "Mnemonic.hpp"

class Compiler
{

private:
    int status;
    std::string _filename;
public:
    int                     reader(std::istream*, char *filename);
    std::vector<Token *>    *tokenization(std::istream &in);
    std::vector<Token *>    *lexer(std::string &str);
};

#endif