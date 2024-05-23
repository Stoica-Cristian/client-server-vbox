//Server Code

#include "MyServer.h"
#include <iostream>

int main()
{
	if (Network::Initialize())
	{
		MyServer server;
		if (server.Initialize(IPEndpoint("::", 6112)))
		{
			while (true)
			{
				server.Frame();
			}
		}
	}
	Network::Shutdown();
	system("pause");
	return 0;
}