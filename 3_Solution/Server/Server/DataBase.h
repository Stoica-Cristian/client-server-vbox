#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <Pnet/Packet.h>;

using namespace PNet;

class DataBase
{
public:
	void Connect();
	void Disconnect();
	int LogIn(std::string credentials);
	int Register(std::string credentials);
	uint32_t ChangeUserCredentials(std::string credentials);
	std::shared_ptr<Packet> SendUserData(std::string credentials);
	uint32_t SaveUserData(std::shared_ptr<Packet> packet, std::string credentials);
private:
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLCHAR retConString[1024];

	void showSQLError(unsigned int handleType, const SQLHANDLE& handle);
};
