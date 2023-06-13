//--------------------------------------------------
// A utility for create a parameterized string 
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <stdexcept>
#include <sstream>
using namespace std;

namespace NVLib
{
	class Formatter
	{
    private:
        std::stringstream _stream;

        Formatter(const Formatter &);
        Formatter & operator = (Formatter &);
    public:
        Formatter() {}
        ~Formatter() {}

        template <typename Type>
        Formatter & operator << (const Type & value)
        {
            _stream << value;
            return *this;
        }

        std::string str() const         { return _stream.str(); }
        operator std::string () const   { return _stream.str(); }

        enum ConvertToString 
        {
            to_str
        };        
        
        std::string operator >> (ConvertToString) { return _stream.str(); }
    };   
}