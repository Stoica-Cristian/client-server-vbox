#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{
    qInfo() << this << "Constructed" << QThread::currentThread();
}

Worker::~Worker()
{
    qInfo() << this << "Deconstructed" << QThread::currentThread();
}

void Worker::run()
{
    try
    {
        if (Network::Initialize())
        {
            if (MyClient::getInstanceClient()->Connect(IPEndpoint("::1", 6112)))
            {
                while (MyClient::getInstanceClient()->IsConnected())
                {
                    MyClient::getInstanceClient()->Frame();
                }
            }
        }
        Network::Shutdown();

    }
    catch (PacketException exception)
    {
        qInfo() << exception.what();
    }
}

void Worker::stop()
{
    QThread::currentThread()->quit();
}
