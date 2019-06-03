#ifndef HELPER_HPP
#define HELPER_HPP

#include <vector>
#include <sstream>
#include <regex>
#include <iomanip>  

std::vector<std::string>    split(const std::string & s,\
                             std::string rgx_str = "\\s+");
void ErrorWithLineNumber(uint32_t lineNum, uint32_t rowNum, std::string &line, std::string &filename, std::string &reason);

#endif