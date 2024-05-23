#pragma once

#include "IncludeMe.h"
#include <QObject>
#include <QVector>
#include "user.h"

class MyClient : public QObject, public Client
{
     Q_OBJECT
public:
    void RequestLogIn(std::string credentials);
    void RequestRegister(std::string credentials);
    void RequestChangeUserData(std::string credentials);
    void SetPort(uint32_t port) { this->port = port; };
    uint32_t GetPort() const { return port; }

    static MyClient* getInstanceClient();
    static void deleteInstanceClient();

    User *getUser() const;
    void setUser(User *newUser);

    bool getMainWindowAlreadyOpened() const;
    void setMainWindowAlreadyOpened(bool newMainWindowAlreadyOpened);

signals:
    void loggedIn(uint32_t retCode);
    void registered(uint32_t retCode);
    void credentialsChanged(uint32_t retCode);
public slots:
    void SendUserData();
private:
    static MyClient* client;
    MyClient() {};
    virtual ~MyClient() {};
    MyClient(MyClient& obj) = delete;

    void PopulateTheUser(std::shared_ptr<Packet> packet);
    bool ProcessPacket(std::shared_ptr<Packet> packet) override;
    void OnConnect() override;

    bool  mainWindowAlreadyOpened = false;
    User* user;
    uint32_t port = 0;
};
