#include "DataBase.h"
#include "Pnet/Packet.h"

using namespace PNet;

void DataBase::Connect()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle) != SQL_SUCCESS)
		// Allocates the environment
		Disconnect();

	if (SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
		// Sets attributes that govern aspects of environments
		Disconnect();

	if (SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle) != SQL_SUCCESS)
		// Allocates the connection
		Disconnect();

	if (SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0) != SQL_SUCCESS)
		// Sets attributes that govern aspects of connections
		Disconnect();

	SQLCHAR retConString[1024];
	switch (SQLDriverConnect(SQLConnectionHandle, NULL,
		(SQLCHAR*)"DRIVER={SQL Server}; SERVER=localhost, 61547; DATABASE=Users;Trusted=true;",
		SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
	{
	case SQL_SUCCESS:
		std::cout << "Successfully connected to SQL Server" << std::endl;
		break;
	case SQL_SUCCESS_WITH_INFO:
		std::cout << "Successfully connected to SQL Server" << std::endl;
		break;
	case SQL_NO_DATA_FOUND:
		showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
		Disconnect();
		break;
	case SQL_INVALID_HANDLE:
		showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
		Disconnect();
		break;
	case SQL_ERROR:
		showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
		Disconnect();
		break;
	default:
		break;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle) != SQL_SUCCESS)
		Disconnect();
}

void DataBase::Disconnect()
{
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
/*
Return codes:
	0 - success
	1 - incorrect password # removed !!!
	2 - the user is not registered
	3 - error (any type)
*/
int DataBase::LogIn(std::string credentials)
{
	size_t index = credentials.find(' ');
	std::string usernameToVerify = credentials.substr(0, index);
	std::string passwordToVerify = credentials.substr(index + 1);

	std::string SQLQuery;
	SQLQuery += "SELECT * FROM Users WHERE Username = '" + usernameToVerify + "'" + " AND Password = '" + passwordToVerify + "'";

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 3;
	}

	int id = 0;
	if (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
	{
		SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
	}
	else
	{
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 2;
	}

	if (id != 0)
		return 0;
	else
		return 1;
}

/*
Return codes:
	0 - success
	1 - already registered
	2 - error (any type)
*/
int DataBase::Register(std::string credentials)
{
	size_t index = credentials.find(' ');
	std::string usernameToInsert = credentials.substr(0, index);
	std::string passwordToInsert = credentials.substr(index + 1);

	//verify if username/email is already in the database
	std::string SQLQueryForUsername = "SELECT Username FROM Users";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQueryForUsername.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 2;
	}
	else
	{
		char username[256];
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &username, sizeof(username), NULL);

			std::string usernameFromDataBase(username);
			if (usernameFromDataBase == usernameToInsert)
				return 1;
		}
	}
	//reset the cursor
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);


	std::string SQLQuery("INSERT INTO Users VALUES ('");
	SQLQuery += usernameToInsert;
	SQLQuery += "', '";
	SQLQuery += passwordToInsert;
	SQLQuery += "');";

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 2;
	}
	else
		return 0;
}

uint32_t DataBase::ChangeUserCredentials(std::string credentials)
{
	size_t index = credentials.find(' ');
	std::string username = credentials.substr(0, index);
	std::string newUsernameAndNewPassword = credentials.substr(index + 1);
	index = newUsernameAndNewPassword.find(' ');
	std::string newUsername = newUsernameAndNewPassword.substr(0, index);
	std::string newPassword = newUsernameAndNewPassword.substr(index + 1);

	std::string SQLQuery("SELECT * FROM Users WHERE Username = '");
	SQLQuery += username;
	SQLQuery += "'";

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 1;
	}

	int id = 0;
	if (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
	{
		SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
	}
	else // there is no registration for this username
	{
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 1;
	}

	if (id == 0)
		return 1;

	//reset the cursor
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);

	SQLQuery = "UPDATE Users SET Username = '";
	SQLQuery += newUsername + "', " + "Password = '" + newPassword + "' WHERE ID = " + std::to_string(id);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 1;
	}

	return 0;
}

std::shared_ptr<Packet> DataBase::SendUserData(std::string credentials)
{
	std::shared_ptr<Packet> userData = std::make_shared<Packet>(PacketType::PT_SendUserData_ServerPerspective);

	size_t index = credentials.find(' ');
	std::string username = credentials.substr(0, index);
	std::string password = credentials.substr(index + 1);

	*userData << username;

	//reset the cursor
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);

	std::string SQLQuery("SELECT * FROM Users WHERE Username = '");
	SQLQuery += username;
	SQLQuery += "'";

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return nullptr;
	}

	int id = 0;
	if (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
	{
		SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
	}
	else // and there is no registration for this username
	{
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return nullptr;
	}

	if (id == 0)
		return nullptr;

	//reset the cursor
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);


	std::string countQuery = "SELECT COUNT(*) FROM VirtualMachines WHERE UserID = " + std::to_string(id);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)countQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return nullptr;
	}

	uint32_t nr_vms = 0;
	if (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
		SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &nr_vms, sizeof(nr_vms), NULL);
	}
	*userData << nr_vms;

	//reset the cursor
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);


	std::string selectQuery = "SELECT * FROM VirtualMachines WHERE UserID = " + std::to_string(id);
	char name[256]{}, type[256]{}, distribution[256]{}, cores[256]{}, ram[256]{}, storage[256]{}, videoMemory[256]{};

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)selectQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return nullptr;
	}

	while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
		SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
		SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &name, sizeof(name), NULL);
		SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &type, sizeof(type), NULL);
		SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &distribution, sizeof(distribution), NULL);
		SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &cores, sizeof(cores), NULL);
		SQLGetData(SQLStatementHandle, 6, SQL_C_DEFAULT, &ram, sizeof(ram), NULL);
		SQLGetData(SQLStatementHandle, 7, SQL_C_DEFAULT, &storage, sizeof(storage), NULL);
		SQLGetData(SQLStatementHandle, 8, SQL_C_DEFAULT, &videoMemory, sizeof(videoMemory), NULL);
		SQLGetData(SQLStatementHandle, 9, SQL_C_DEFAULT, &id, sizeof(id), NULL);

		std::string nameStr(name);
		std::string typeStr(type);
		std::string distributionStr(distribution);
		std::string coresStr(cores);
		std::string ramStr(ram);
		std::string storageStr(storage);
		std::string videoMemoryStr(videoMemory);

		*userData << nameStr << typeStr << distributionStr << coresStr << ramStr << storageStr << videoMemoryStr;
	}

	return userData;
}

/*
Return codes:
	0 - success
	1 - error
*/
uint32_t DataBase::SaveUserData(std::shared_ptr<Packet> packet, std::string username)
{
	std::string SQLQuery("SELECT * FROM Users WHERE Username = '");
	SQLQuery += username;
	SQLQuery += "'";

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 1;
	}

	int id = 0;
	if (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
	{
		SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
	}
	else
	{
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		Disconnect();
		return 1;
	}
	//reset the cursor
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);

	// delete vms already stored
	SQLQuery = "DELETE FROM VirtualMachines WHERE UserID = " + std::to_string(id);

	SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS);

	uint32_t nr_vms;
	*packet >> nr_vms;

	std::string name, type, distribution, cores, ram, storage, videoMemory;

	for (uint32_t i = 0; i < nr_vms; ++i)
	{
		*packet >> name >> type >> distribution >> cores >> ram >> storage >> videoMemory;

		SQLQuery = "INSERT INTO VirtualMachines VALUES('";
		SQLQuery += name + "', '";
		SQLQuery += type + "', '";
		SQLQuery += distribution + "', '";
		SQLQuery += cores + "', '";
		SQLQuery += ram + "', '";
		SQLQuery += storage + "', '";
		SQLQuery += videoMemory + "', '";
		SQLQuery += std::to_string(id) + "');";

		//reset the cursor
		SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
		SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);

		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
			Disconnect();
			return 1;
		}
	}
	return 0;
}

void DataBase::showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
}
