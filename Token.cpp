#include "Token.hpp"

Token::Token(int type) : t_type(type) {} ;
int Token::getType(void)
{
    return t_type;}