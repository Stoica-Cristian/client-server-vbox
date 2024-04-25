#include "mainwindow.h"

#include <QApplication>
#include "MyClient.h"
#include <unistd.h>
#include<windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

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
                sleep(2);
                client.connection.pm_outgoing.Append(message);
            }
        }
    }
    Network::Shutdown();
    //system("pause");

    return a.exec();
}
