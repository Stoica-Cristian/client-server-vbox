#include "MyServer.h"
#include <iostream>

void MyServer::LogIn(std::shared_ptr<Packet> packet)
{
	uint32_t portToResend;
	std::string credentials;
	*packet >> portToResend;
	*packet >> credentials;

	database.Connect();

	std::shared_ptr<Packet> logInPacket = std::make_shared<Packet>(PacketType::PT_LogIn);
	std::shared_ptr<Packet> userData = std::make_shared<Packet>(PacketType::PT_SendUserData_ServerPerspective);

	uint32_t retCode = database.LogIn(credentials);
	if (retCode == 0)
	{
		userData = database.SendUserData(credentials);
	}
	*logInPacket << retCode;

	for (auto& connection : connections)
		if (connection.Port() == portToResend)
		{
			if (retCode == 0)
				connection.pm_outgoing.Append(userData);

			connection.pm_outgoing.Append(logInPacket);
		}

	database.Disconnect();
}

void MyServer::Register(std::shared_ptr<Packet> packet)
{
	uint32_t portToResend;
	std::string credentials;
	*packet >> portToResend;
	*packet >> credentials;

	database.Connect();

	std::shared_ptr<Packet> registerPacket = std::make_shared<Packet>(PacketType::PT_Register);

	uint32_t retCode = database.Register(credentials);

	*registerPacket << retCode;

	for (auto& connection : connections)
		if (connection.Port() == portToResend)
			connection.pm_outgoing.Append(registerPacket);

	database.Disconnect();
}

void MyServer::SaveUserData(std::shared_ptr<Packet> packet)
{
	uint32_t portToResend;
	std::string username;
	*packet >> portToResend;
	*packet >> username;

	database.Connect();
	uint32_t retCode = database.SaveUserData(packet, username);

	std::shared_ptr<Packet> respond = std::make_shared<Packet>(PacketType::PT_SaveUserData_ServerPerspective);
	*respond << retCode;

	for (auto& connection : connections)
		if (connection.Port() == portToResend)
			connection.pm_outgoing.Append(respond);

	database.Disconnect();
}

void MyServer::ChangeUserCredentials(std::shared_ptr<Packet> packet)
{
	uint32_t portToResend;
	std::string credentials;
	*packet >> portToResend;
	*packet >> credentials;

	database.Connect();
	uint32_t retCode = database.ChangeUserCredentials(credentials);

	if (retCode == 0)
	{
		std::shared_ptr<Packet> response = std::make_shared<Packet>(PacketType::PT_ChangeUserCredentials);

		*response << retCode;

		for (auto& connection : connections)
			if (connection.Port() == portToResend)
			{
				connection.pm_outgoing.Append(response);
			}

	}
	database.Disconnect();
}

void MyServer::OnConnect(TCPConnection& newConnection)
{
	std::cout << newConnection.ToString() << " - New connection accepted." << std::endl;

	std::shared_ptr<Packet> welcomeMessagePacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
	*welcomeMessagePacket << std::string("Welcome!");
	newConnection.pm_outgoing.Append(welcomeMessagePacket);

	std::shared_ptr<Packet> portMessagePacket = std::make_shared<Packet>(PacketType::PT_Port);
	uint32_t port = newConnection.Port();
	*portMessagePacket << port;
	newConnection.pm_outgoing.Append(portMessagePacket);

	//std::shared_ptr<Packet> newUserMessagePacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
	//*newUserMessagePacket << std::string("New user connected!");
	//for (auto& connection : connections)
	//{
	//	if (&connection == &newConnection)
	//		continue;

	//	connection.pm_outgoing.Append(newUserMessagePacket);
	//}
}

void MyServer::OnDisconnect(TCPConnection& lostConnection, std::string reason)
{
	std::cout << "[" << reason << "]" << " Connection lost: " << lostConnection.ToString() << "." << std::endl;

	//std::shared_ptr<Packet> connectionLostPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
	//*connectionLostPacket << std::string("A user disconnected!");
	//for (auto& connection : connections)
	//{
	//	if (&connection == &lostConnection)
	//		continue;

	//	connection.pm_outgoing.Append(connectionLostPacket);
	//}
}

bool MyServer::ProcessPacket(std::shared_ptr<Packet> packet)
{
	switch (packet->GetPacketType())
	{
	case PacketType::PT_ChatMessage:
	{
		std::string chatmessage;
		*packet >> chatmessage;
		std::cout << "Chat Message: " << chatmessage << std::endl;
		break;
	}
	case PacketType::PT_Integer:
	{
		uint32_t integer = 0;
		*packet >> integer;
		std::cout << "Integer: " << integer << std::endl;
		break;
	}
	case PacketType::PT_IntegerArray:
	{
		uint32_t arraySize = 0;
		*packet >> arraySize;
		std::cout << "Array Size: " << arraySize << std::endl;
		for (uint32_t i = 0; i < arraySize; i++)
		{
			uint32_t element = 0;
			*packet >> element;
			std::cout << "Element[" << i << "] - " << element << std::endl;
		}
		break;
	}
	case PacketType::PT_LogIn:
	{
		LogIn(packet);
		break;
	}
	case PacketType::PT_Register:
	{
		Register(packet);
		break;
	}
	case PacketType::PT_SaveUserData_ServerPerspective:
	{
		SaveUserData(packet);
		break;
	}
	case PacketType::PT_ChangeUserCredentials:
	{
		ChangeUserCredentials(packet);
		break;
	}
	default:
		std::cout << "Unrecognized packet type: " << packet->GetPacketType() << std::endl;
		return false;
	}

	return true;
}
