//--------------------------------------------------
// The email that we are dealing with
//
// @author: Wild Boar
//
// @date: 2022-11-15
//--------------------------------------------------

#pragma once

#include <sstream>
#include <iostream>
using namespace std;

#include "CmdLineUtils.h"

namespace NVLib
{
	class Email
	{
	private:
		string _server;
		int _port;
		string _from;
		string _password;

	public:
		Email(const string& server, int port, const string& from, const string& password);
		~Email();

		void Send(const string& recipient, const string& subject, const string& message);

		inline string& GetServer() { return _server; }
		inline int& GetPort() { return _port; }
		inline string& GetFrom() { return _from; }
		inline string& GetPassword() { return _password; }
	};
}
