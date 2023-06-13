//--------------------------------------------------
// Implementation of class Email
//
// @author: Wild Boar
//
// @date: 2022-11-15
//--------------------------------------------------

#include "Email.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Initializer Constructor
 * @param server Initialize variable <server>
 * @param port Initialize variable <port>
 * @param from Initialize variable <from>
 * @param password Initialize variable <password>
 */
Email::Email(const string& server, int port, const string& from, const string& password)
{
	_server = server;
	_port = port;
	_from = from;
	_password = password;
}

/**
 * @brief Main Terminator
 */
Email::~Email()
{
	// Termination logic can go here
}

//--------------------------------------------------
// Send
//--------------------------------------------------

/**
 * @brief Send an email
 * @param recipient Who we are sending to
 * @param subject The subject of the email that is being sent
 * @param message The message that we are sending
 */
void Email::Send(const string& recipient, const string& subject, const string& message)
{
	auto command = stringstream();
	command << "sendemail -f " << _from << " -t " << recipient << " ";
	command << "-u \"" << subject << "\" -m \"" << message << "\" ";
	command << "-s " << _server << ":" << _port << " -o tls=yes ";
	command << "-xu " << _from << " -xp " << _password;

	NVLib::CmdLineUtils::Execute(command.str());
}
