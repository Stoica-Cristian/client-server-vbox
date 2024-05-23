#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <QCoreApplication>
#include <QObject>
#include <QDir>
#include <QDebug>

class VirtualMachine: public QObject
{
    Q_OBJECT
public:
    VirtualMachine() = default;
    VirtualMachine(QString username, QString vmName, QString type, QString distribution);
    ~VirtualMachine();

    QString distribution() const;
    void setDistribution(const QString &newDistribution);

    QString type() const;
    void setType(const QString &newType);

    QString name() const;
    void setName(const QString &newName);

    QString videoMemory() const;
    void setVideoMemory(const QString &newVideoMemory);

    QString ram() const;
    void setRam(const QString &newRam);

    QString cores() const;
    void setCores(const QString &newCores);

    QString storage() const;
    void setStorage(const QString &newStorage);

    QString status() const;
    void setStatus(const QString &newStatus);

    void setUsername(const QString &newUsername);

    QString getPath() const;

private:
    QString m_name;
    QString m_type;
    QString m_distribution;
    QString m_videoMemory;
    QString m_ram;
    QString m_cores;
    QString m_storage;
    QString m_status;

    QString username;
    QString path;
public slots:
    void CreateVM(QString clientName);
};

#endif // VIRTUALMACHINE_H
