#include "MyClient.h"
#include <iostream>

MyClient* MyClient::client = nullptr;

MyClient* MyClient::getInstanceClient()
{
    if(client==nullptr)
    {
        client=new MyClient;
    }
    return client;
}

void MyClient::deleteInstanceClient()
{
    if(client!=nullptr)
    {
        delete client;
        client=nullptr;
    }
}

void MyClient::RequestLogIn(std::string credentials)
{
    std::shared_ptr<Packet> message = std::make_shared<Packet>(PacketType::PT_LogIn);
    *message << this->GetPort() << credentials;
    this->connection.pm_outgoing.Append(message);
}

void MyClient::RequestRegister(std::string credentials)
{
    std::shared_ptr<Packet> message = std::make_shared<Packet>(PacketType::PT_Register);
    *message << this->GetPort() << credentials;
    this->connection.pm_outgoing.Append(message);
}

void MyClient::RequestChangeUserData(std::string credentials)
{
    std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_ChangeUserCredentials);
    std::string data = user->name().toStdString();
    data += " " + credentials;
    *packet << this->GetPort() << data;
    this->connection.pm_outgoing.Append(packet);
}

bool MyClient::ProcessPacket(std::shared_ptr<Packet> packet)
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
        uint32_t retCode = 0;
        *packet >> retCode;
        emit loggedIn(retCode);
        break;
    }
    case PacketType::PT_Register:
    {
        uint32_t retCode = 0;
        *packet >> retCode;
        emit registered(retCode);
        break;
    }
    case PacketType::PT_Port:
    {
        uint32_t port;
        *packet >> port;
        SetPort(port);
        std::cout << GetPort() << std::endl;
        break;
    }
    case PacketType::PT_SendUserData_ServerPerspective:
    {
        PopulateTheUser(packet);
        break;
    }
    case PacketType::PT_ChangeUserCredentials:
    {
        uint32_t retCode;
        *packet >> retCode;
        emit credentialsChanged(retCode);
        break;
    }
    default:
        std::cout << "Unrecognized packet type: " << packet->GetPacketType() << std::endl;
        return false;
    }
    return true;
}

void MyClient::OnConnect()
{
    std::cout << "Successfully connected to the server!" << std::endl;

    std::shared_ptr<Packet> helloPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *helloPacket << std::string("Hello from the client!");
    connection.pm_outgoing.Append(helloPacket);
}

bool MyClient::getMainWindowAlreadyOpened() const
{
    return mainWindowAlreadyOpened;
}

void MyClient::setMainWindowAlreadyOpened(bool newMainWindowAlreadyOpened)
{
    mainWindowAlreadyOpened = newMainWindowAlreadyOpened;
}

User *MyClient::getUser() const
{
    return user;
}

void MyClient::setUser(User *newUser)
{
    user = newUser;
}

void MyClient::SendUserData()
{
    std::shared_ptr<Packet> packet = std::make_shared<Packet>(PacketType::PT_SaveUserData_ServerPerspective);
    *packet << GetPort() << getUser()->name().toStdString();
    *packet << getUser()->getVirtualMachines().size();
    for(auto vm : getUser()->getVirtualMachines())
    {
        *packet << vm->name().toStdString();
        *packet << vm->type().toStdString();
        *packet << vm->distribution().toStdString();
        *packet << vm->cores().toStdString();
        *packet << vm->ram().toStdString();
        *packet << vm->storage().toStdString();
        *packet << vm->videoMemory().toStdString();
    }
    connection.pm_outgoing.Append(packet);
}

void MyClient::PopulateTheUser(std::shared_ptr<Packet> packet)
{
    std::string name;
    *packet >> name;
    setUser(new User(QString::fromStdString(name)));

    uint32_t nr_vms;
    *packet >> nr_vms;

    for(int i = 0; i < nr_vms; i++)
    {
        std::string vmNameString;
        *packet >> vmNameString;
        QString vmName = QString::fromStdString(vmNameString);

        std::string vmTypeString;
        *packet >> vmTypeString;
        QString vmType = QString::fromStdString(vmTypeString);

        std::string vmDistroString;
        *packet >> vmDistroString;
        QString vmDistro = QString::fromStdString(vmDistroString);

        std::string vmCoresString;
        *packet >> vmCoresString;
        QString vmCores = QString::fromStdString(vmCoresString);

        std::string vmRamString;
        *packet >> vmRamString;
        QString vmRam = QString::fromStdString(vmRamString);

        std::string vmStorageString;
        *packet >> vmStorageString;
        QString vmStorage = QString::fromStdString(vmStorageString);

        std::string vmVideoMemoryString;
        *packet >> vmVideoMemoryString;
        QString vmVideoMemory = QString::fromStdString(vmVideoMemoryString);


        VirtualMachine* vm = new VirtualMachine(QString::fromStdString(name), vmName, vmType, vmDistro);
        vm->setCores(vmCores);
        vm->setRam(vmRam);
        vm->setStorage(vmStorage);
        vm->setVideoMemory(vmVideoMemory);

        user->addVM(vm);
    }
}
