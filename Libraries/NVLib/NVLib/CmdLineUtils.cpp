//--------------------------------------------------
// Implementation of class CmdLineUtils
//
// @author: Wild Boar
//
// @date: 2021-08-31
//--------------------------------------------------

#include "CmdLineUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Launcher
//--------------------------------------------------

/**
 * @brief Executes a cmdLine tool and return result
 * @param command The cmdline call we want to invoke
 * @return string Returns a string
 */
string CmdLineUtils::Execute(const string& command)
{
	auto pipe = unique_ptr<FILE, decltype(&pclose)> (popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!"); 
    
	auto buffer = array<char, 500>(); auto result = stringstream();
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
	{
        result << buffer.data();
    }
	
    return result.str();
}
