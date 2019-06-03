#include "Compiler.hpp"

std::vector<Token *> *Compiler::lexer(std::string &str)
{
    std::vector<Token *> *tokens = new std::vector<Token*>();
    static uint32_t lineNum = -1;
    static std::regex intr_w_opr("(push|assert)[ \\t]+((float|int(8|16|32)|double)\\([-]?[0-9]*[\\.]?[0-9]*\\))[ \\t]*(;.*)?");
    static std::regex intr("(pop|dump|add|sub|mul|div|mod|print|exit)[ \\t]*(;.*)?");
    static std::regex push_assert("(push|assert)[ \\t]*(;.*)?");
    static std::unordered_map<std::string, int> mne_map = {
        { "push", PUSH }, 
        { "pop", POP },
        { "dump", DUMP },
        { "assert", ASSERT }, 
        { "sub", SUB }, 
        { "mul", MUL }, 
        { "div", DIV },
        { "mod", MOD },
        { "print", PRINT },
        { "exit", EXIT }
    };
    static std::regex rint8("int8\\(([-]?[0-9]+)\\)");
    static std::regex rint16("int16\\(([-]?[0-9]+)\\)");
    static std::regex rint32("int32\\(([-]?[0-9]+)\\)");
    static std::regex rfloat("float\\(([-]?[0-9]+\\.[0-9]+)\\)");
    static std::regex rdouble("double\\(([-]?[0-9]+\\.[0-9]+)\\)");
    // static std::unordered_map<std::regex, int> reg_map = {
    //     { rint8, INT8 },
    //     { rint16, INT16 },
    //     { rint32, INT32 },
    //     { rfloat, FLOAT },
    //     { rdouble, DOUBLE }
    // };

    ++lineNum;
    if (str[0] == ';')
        return tokens;
    if (std::regex_match(str, intr_w_opr))
    {
        std::vector<std::string> ret = split(str, "[ \\t]");
        tokens->push_back(new Mnemonic(mne_map[ret[0]]));
        // for (auto& p : reg_map)
        // {
        //     if (std::regex_match(ret[1], p.first))
        //     {
        //         std::string oper;
        //         int         type = p.second;

        //         if (type == INT8)
        //             oper = ret[1].substr(5);
        //         else if (type == DOUBLE)
        //             oper = ret[1].substr(7);
        //         else
        //             oper = ret[1].substr(6);
        //         tokens->push_back(fac_.createOperand(type, oper));
        //         break ;
        //     }
        // }

        // for (auto p : ret)
        //     delete p;
    }
    else if (std::regex_match(str, intr))
    {
        for (auto &p : mne_map)
        {
            std::size_t found = str.find(p.first);
            if (found != std::string::npos)
                tokens->push_back(new Mnemonic(p.second));
        }
    }
    else if (std::all_of(str.begin(), str.end(), [](char c) { return std::isspace(c); }))
        return tokens;
    else
    {
        delete tokens;
        std::string reason;
        if (std::regex_match(str, push_assert))
            reason = std::string(" \x1b[31merror\x1b[0m: Invalid use of mnemonic");
        else
            reason = std::string(" \x1b[31merror\x1b[0m: Unknown mnemonic");
        ErrorWithLineNumber(lineNum, 1, str, _filename, reason);
    }
    return tokens;
}


std::vector<Token *> *Compiler::tokenization(std::istream &in)
{
    std::string         line;
    std::stringstream   errors;
    std::vector<Token *> *tokens_all = new std::vector<Token*>();

    while (std::getline(in, line))
    {
        try {
            std::vector<Token *> *tokens = lexer(line);
            if (tokens->size() != 0)
                tokens_all->insert(tokens_all->end(), tokens->begin(), tokens->end());
            delete tokens;
        } catch(std::exception &e) {
            errors << e.what() << std::endl;
        }
    }
    std::cout << errors.str();
    return tokens_all;
}

int     Compiler::reader(std::istream *in, char *filename)
{
    std::string line;
    _filename = std::string(filename);

    try {
        std::vector<Token *> *tokens = tokenization(*in);
        for (auto _t : *tokens) {
            std::cout << std::to_string(_t->getType()) << std::endl;
        }
        std::cout << "it works!" << std::endl;
        // try {
        //     std::vector<Instruction *> instructions = parser(tokens);
        //     try {
        //         execute(instructions);
        //     } catch(std::exception &e) {
        //         std::cout << "[execution]" << std::endl;
        //         std::cout << e.what() << std::endl;
        //         ::exit(EXIT_FAILURE);
        //     }
        // } catch(std::exception &e) {
        //     std::cout << "[paser]" << std::endl;
        //     std::cout << e.what() << std::endl;
        //     ::exit(EXIT_FAILURE);
        // }
    } catch(std::exception &e) {
        std::cout << "[lexer]" << std::endl;
        std::cout << e.what() << std::endl;
        ::exit(EXIT_FAILURE);
    }
    return (1);
}