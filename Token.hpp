#ifndef TOKEN_HPP
#define TOKEN_HPP

enum eToken_type
{
    tMnemonic,
    tOperand
};

class Token
{
    public:
        virtual eToken_type getTokenType(void) = 0;
};

#endif