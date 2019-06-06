#include "Helper.hpp"

std::vector<std::string>    split(const std::string & s, std::string rgx_str)
{
      std::vector<std::string> elems;
      std::regex rgx (rgx_str);
      std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
      std::sregex_token_iterator end;

      while (iter != end)
      {
          elems.push_back(*iter);
          ++iter;
      }
      return elems;
  }

void ErrorWithLineNumber(uint32_t lineNum, uint32_t rowNum, std::string &line, std::string &filename, std::string &reason)
{
    std::stringstream ss;
    auto len = line.size();
    auto howmany = len - rowNum;

    ss << filename << ":" << std::to_string(lineNum) << ":" << rowNum << ": " << reason << ": " << std::endl; 
    ss << "\t" << line.insert(rowNum - 1, "\x1b[31m") << "\x1b[0m" << std::endl;
    ss << "\t";
    for (unsigned int i = 0; i < rowNum - 1; i++)
        ss << " ";
    ss << "\x1b[31m^";
    for (unsigned int i = 0; i < howmany; i++)
        ss << "~";
    ss << "\x1b[0m";
    throw std::runtime_error(ss.str());
}

std::string trim(const std::string &s)
{
	auto start = s.begin();
	while (start != s.end() && std::isspace(*start)) {
		start++;
	}

	auto end = s.end();
	do {
		end--;
	} while (std::distance(start, end) > 0 && std::isspace(*end));

	return std::string(start, end + 1);
}
