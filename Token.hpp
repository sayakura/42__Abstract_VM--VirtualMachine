#ifndef TOKEN_HPP
#define TOKEN_HPP

typedef enum Token_type
{
    tMnemonic,
    tOperand
}           eToken_type;

class Token
{
    Token();
    protected:
        int t_type;
    public:
        Token(int);
        int getType(void);
};

#endif