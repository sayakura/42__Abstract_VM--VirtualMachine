#include <istream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "Compiler.hpp"
#include "Instruction.hpp"

bool objectMode = false;
bool loaderMode = false;
extern bool stdinMode;

void loader(std::vector<Instruction *> *list);

void    createObjectFile(std::vector<Instruction *> *in, std::string filename)
{
    filename += ".o";
    int fd = open(filename.c_str() , O_CREAT | O_RDWR | O_TRUNC, 0666);

    for (auto line : *in)
    {
        char *ptr = reinterpret_cast<char *>(line);
        ssize_t bwrite = write(fd, ptr, sizeof(Instruction));
        (void)bwrite;
    }
    close(fd);
}

std::vector<Instruction *> *parseObjectFile(const char *filename)
{
    std::ifstream   infile;
    char            buffer[sizeof(Instruction)] = {0};
    std::vector<Instruction *> *_instr = new std::vector<Instruction *>();
    int fd = open(filename, O_RDONLY);

    if (fd > 0)
    {
        int br; 
        while ((br = read(fd, buffer, sizeof(Instruction))) == sizeof(Instruction))
        {
            Instruction *ins = new Instruction;
            memcpy(ins, buffer, sizeof(Instruction));
            _instr->push_back(ins);
        }
    }
    else
    {
        std::cout << "\x1b[31mError with reading file \x1b[0m" << filename << std::endl;
        ::exit(EXIT_FAILURE);
    }
    return _instr;
}

void    readFlags(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (std::strcmp(argv[i], "-c") == 0)
        {
            objectMode = true;
            if (argc == 2)
            {
                std::cout << "-c [filenname]" << std::endl;
                ::exit(EXIT_SUCCESS);
            }
        }
        else if (std::strcmp(argv[i], "-l") == 0)
            loaderMode = true;
        else if (std::strcmp(argv[i], "-h") == 0)
        {
            std::cout << "NAME" << std::endl;
            std::cout << "\tAbstract_VM" << std::endl;
            std::cout << "SYNOPSIS  -c -l filename/objectfile" << std::endl;
            std::cout << "OPTIONS" << std::endl;
            std::cout << "\t-c compiler object files" << std::endl;
            std::cout << "\t-l run the object file directly" << std::endl;
            ::exit(EXIT_SUCCESS);
        }
    }
}

int     main(int argc, char **argv)
{
    std::ifstream   inputFile;
    std::istream*   in;
    Compiler        myCompiler;
    std::string     obfile;
    std::vector<Instruction *> *ins;

    if (argc < 2)
    {
        stdinMode = true;
        in = &std::cin;
        ins = myCompiler.run(in, const_cast<char *>("Stdin"));
        loader(ins);
    }
    else 
    {
        readFlags(argc, argv);
        if (loaderMode)
        {
            if (argc < 3)
            {
                std::cout << "-l [filenname(object file)]" << std::endl;
                ::exit(EXIT_SUCCESS);
            }
            ins = parseObjectFile(argv[argc - 1]);
        }
        else
        {
            inputFile.open(argv[argc - 1], std::ifstream::in);
            in = &inputFile;
            ins = myCompiler.run(in, argv[argc - 1]);
            if (objectMode)
            {
                obfile = argv[argc - 1];
                createObjectFile(ins, obfile);
                return (0);
            }
        }
        loader(ins);
	}
    return (0);
}
