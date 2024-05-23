#pragma once

#include <Pnet/IncludeMe.h>
#include "DataBase.h"

using namespace std;

class MyServer : public Server
{
private:
	DataBase database;
	void LogIn(std::shared_ptr<Packet> packet);
	void Register(std::shared_ptr<Packet> packet);
	void SaveUserData(std::shared_ptr<Packet> packet);
	void ChangeUserCredentials(std::shared_ptr<Packet> packet);

	void OnConnect(TCPConnection& newConnection) override;
	void OnDisconnect(TCPConnection& lostConnection, std::string reason) override;
	bool ProcessPacket(std::shared_ptr<Packet> packet) override;
};