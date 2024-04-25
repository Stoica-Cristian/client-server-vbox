//Client Code

#include "MyClient.h"
#include <iostream>

int main()
{
	if (Network::Initialize())
	{
		MyClient client;
		if (client.Connect(IPEndpoint("::1", 6112)))
		{
			while (client.IsConnected())
			{
				client.Frame();
				std::shared_ptr<Packet> message = std::make_shared<Packet>(PacketType::PT_ChatMessage);
				*message << "Buna, Andrei!";
				Sleep(1000);
				client.connection.pm_outgoing.Append(message);
			}
		}
	}
	Network::Shutdown();
	system("pause");
	return 0;
}