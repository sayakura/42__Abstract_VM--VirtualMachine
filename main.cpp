#include <istream>
#include <iostream>
#include <fstream>
#include "Compiler.hpp"

void loader(std::vector<Instruction *> *list);

int     main(int argc, char **argv)
{
    std::ifstream inputFile;
    std::istream* in;
    Compiler      kc;

    if (argc < 2)
        in = &std::cin;
    if (argv[1])
    {
        inputFile.open(argv[1], std::ifstream::in);
        in = &inputFile;
	}
    loader(kc.run(in, argv[1]));
    return (0);
}