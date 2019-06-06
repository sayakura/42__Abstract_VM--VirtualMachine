#include "Factory.hpp"
#include "Operand.hpp"

Factory::Factory() {
    ftptr.push_back(&Factory::createInt8);
    ftptr.push_back(&Factory::createInt16);
    ftptr.push_back(&Factory::createInt32);
    ftptr.push_back(&Factory::createFloat);
    ftptr.push_back(&Factory::createDouble);
};

Factory::~Factory() {};

IOperand const * Factory::createOperand( eOperandType type, std::string const & value ) const {
    return (this->*ftptr[type])(value);
}

IOperand const * Factory::createInt8( std::string const & value ) const {
    std::stringstream ss(value);
    short tmp;

    ss >> tmp;
    if (tmp < std::numeric_limits<char>::min())
		  throw (std::underflow_error("Underflow error in creating int8"));
	  if (tmp > std::numeric_limits<char>::max())
		  throw (std::overflow_error("Overflow error in creating int8"));  
    return new Operand<int8_t>(tmp, INT8);
};

IOperand const * Factory::createInt16( std::string const & value ) const {
    std::stringstream ss(value);
    int tmp;

    ss >> tmp;
    if (tmp < std::numeric_limits<short>::min())
		  throw (std::underflow_error("Underflow error in creating int16"));
	  if (tmp > std::numeric_limits<short>::max())
		  throw (std::overflow_error("Overflow error in creating int16"));  
    return new Operand<int16_t>(tmp, INT16);
};

IOperand const * Factory::createInt32( std::string const & value ) const {
    std::stringstream ss(value);
    long long tmp;

    ss >> tmp;
    if (tmp < std::numeric_limits<int32_t>::min())
		  throw (std::underflow_error("Underflow error in creating int32"));
	  if (tmp > std::numeric_limits<int32_t>::max())
		  throw (std::overflow_error("Overflow error in creating int32"));  
    return new Operand<int32_t>(tmp, INT32);
};

IOperand const * Factory::createFloat( std::string const & value ) const {
    std::stringstream ss(value);
    double tmp;

    ss >> tmp;
    if (tmp < std::numeric_limits<float>::min())
		  throw (std::underflow_error("Underflow error in creating float"));
	  if (tmp > std::numeric_limits<float>::max())
		  throw (std::overflow_error("Overflow error in creating float"));  
    return new Operand<float>(tmp, FLOAT);
};

IOperand const * Factory::createDouble( std::string const & value ) const {
    std::stringstream ss(value);
    long double tmp;

    ss >> tmp;
    if (tmp < std::numeric_limits<double>::min())
		  throw (std::underflow_error("Underflow error in creating double"));
    if (tmp > std::numeric_limits<double>::max())
        throw (std::overflow_error("Overflow error in creating double"));  
    return new Operand<double>(tmp, DOUBLE);
};