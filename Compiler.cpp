#include "Compiler.hpp"

bool stdinMode = false;

Compiler::SyntaxError::SyntaxError(const char *message) : _message(message) {};

const char * Compiler::SyntaxError::what () const throw () {
    return _message;
};

std::vector<Token *> *Compiler::lexer(std::string &str)
{
    std::vector<Token *> *tokens = new std::vector<Token*>();
    static uint32_t lineNum = 0;
    static Factory  _factory;
    static std::regex intr_w_opr("(push|assert)[ \\t]+((float|int(8|16|32)|double)\\([-]?[0-9]*[\\.]?[0-9]*\\))[ \\t]*(;.*)?");
    static std::regex intr("(pop|dump|add|sub|mul|div|mod|print|exit)[ \\t]*(;.*)?");
    static std::regex push_assert("(push|assert)[ \\t]*(;.*)?");
    static std::unordered_map<std::string, eMnemonic_type> mne_map = {
        { "push", PUSH }, 
        { "pop", POP },
        { "dump", DUMP },
        { "assert", ASSERT }, 
        { "add", ADD },
        { "sub", SUB }, 
        { "mul", MUL }, 
        { "div", DIV },
        { "mod", MOD },
        { "print", PRINT },
        { "exit", EXIT }
    };
    static std::regex rint8("int8\\(([-]?[0-9]+)\\)[ \\t]*(;.*)?");
    static std::regex rint16("int16\\(([-]?[0-9]+)\\)[ \\t]*(;.*)?");
    static std::regex rint32("int32\\(([-]?[0-9]+)\\)[ \\t]*(;.*)?");
    static std::regex rfloat("float\\(([-]?[0-9]+\\.[0-9]+)\\)[ \\t]*(;.*)?");
    static std::regex rdouble("double\\(([-]?[0-9]+\\.[0-9]+)\\)[ \\t]*(;.*)?");
    static std::vector<std::regex *> rgex_list = {
        &rint8,
        &rint16,
        &rint32,
        &rfloat,
        &rdouble,
    };

    ++lineNum;
    str = trim(str);
    if (str[0] == ';')
        return tokens;
    if (std::regex_match(str, intr_w_opr))
    {
        bool found = false;
        std::vector<std::string> ret = split(str, "[ \\t]");
        tokens->push_back(new Mnemonic(mne_map[ret[0]]));
        try
        { 
            for (uint8_t i = 0; i < rgex_list.size(); i++)
            {
                if (std::regex_match(ret[1], *rgex_list[i]))
                {
                    std::string     oper;
                    eOperandType    type = static_cast<eOperandType>(i);
                    IOperand        *ptr;
                    Token           *t;

                    found = true;
                    if (type == INT8)
                        oper = ret[1].substr(5);
                    else if (type == DOUBLE)
                        oper = ret[1].substr(7);
                    else
                        oper = ret[1].substr(6);
                    ptr = const_cast<IOperand*>(_factory.createOperand(type, oper));
                    t = reinterpret_cast<Token *>(ptr);
                    tokens->push_back(t);
                }
            }
            if (!found)
                throw Compiler::SyntaxError("Invalid grammer for the number representation");
        }
        catch(std::exception &e)
        {
            delete tokens;
            std::string reason = e.what();
            std::size_t found = ret[1].find("(");
            std::size_t found1 = str.find(ret[1]);
            ErrorWithLineNumber(lineNum, found1 + found + 2, str, _filename, reason);
        }
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
            reason = std::string("\x1b[31merror\x1b[0m: Invalid use of mnemonic");
        else
            reason = std::string("\x1b[31merror\x1b[0m: Unknown mnemonic");
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
            if (line == ";;" && stdinMode)
                break ;
            std::vector<Token *> *tokens = lexer(line);
            if (tokens->size() != 0)
                tokens_all->insert(tokens_all->end(), tokens->begin(), tokens->end());
            delete tokens;
        } catch(std::exception &e) {
            errors << e.what() << std::endl;
        }
    }
    std::cout << errors.str();
    if (errors.str().size())
        ::exit(EXIT_FAILURE);
    return tokens_all;
}

std::vector<Instruction *> *Compiler::parser(std::vector<Token *> *tokens)
{
    std::vector<Instruction *> *instructions = new std::vector<Instruction *>();
    std::queue<Token *> q; 

    for (auto _t : *tokens)
        q.push(_t);
    while (!q.empty())
    {
        Token *current_token = q.front();
        q.pop();

        Instruction *_in = new Instruction();
        Mnemonic    *_mn = reinterpret_cast<Mnemonic *>(current_token);

        bzero(_in, sizeof(Instruction));
        _in->_mnemonic = _mn->getMnemonicType();
        if (_in->_mnemonic == PUSH || _in->_mnemonic == ASSERT)
        {
            Token *operand_token = q.front();
            q.pop();
            IOperand *_op = reinterpret_cast<IOperand *>(operand_token);
            _in->_operand_type = _op->getType();
            switch(_in->_operand_type)
            {
                case INT8:
                    _in->_register._int8 = static_cast<int8_t>(_op->getVal());
                    break ;
                case INT16:
                    _in->_register._int16 = static_cast<int16_t>(_op->getVal());
                    break ;
                case INT32:
                    _in->_register._int32 = static_cast<int32_t>(_op->getVal());
                    break ;
                case FLOAT:
                    _in->_register._float = static_cast<float>(_op->getVal());
                    break ;
                case DOUBLE:
                    _in->_register._double = static_cast<double>(_op->getVal());
                    break ;
                default:
                    ;
            };
        }
        instructions->push_back(_in);
    }
    return instructions;
}

std::vector<Instruction *> *Compiler::run(std::istream *in, char *filename)
{
    std::string line;
    _filename = std::string(filename);

    std::vector<Token *> *tokens = tokenization(*in);
    std::vector<Instruction *> *instructions = parser(tokens);
    return instructions; 
}