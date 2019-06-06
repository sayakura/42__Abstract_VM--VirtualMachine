#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "IOperand.hpp"
#include <limits>
#include <exception>
#include <sstream>
#include <iostream>
#include <vector>

class Factory {
	public:
		typedef std::vector<IOperand const *(Factory::*)(std::string const & value) const> factory_ftptr;
		Factory();
		~Factory();
		Factory( Factory const & );
		Factory &operator=( Factory const & );
		factory_ftptr ftptr;
		IOperand const * createOperand( eOperandType type, std::string const & value ) const;
	private:
		IOperand const * createInt8( std::string const & value ) const;
		IOperand const * createInt16( std::string const & value ) const;
		IOperand const * createInt32( std::string const & value ) const;
		IOperand const * createFloat( std::string const & value ) const;
		IOperand const * createDouble( std::string const & value ) const;
};

#endif