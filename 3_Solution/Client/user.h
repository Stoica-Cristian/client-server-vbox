#ifndef USER_H
#define USER_H

#include <QObject>
#include "virtualmachine.h"

class User: public QObject
{
    Q_OBJECT
public:
    User(){};
    User(QString name);

    int addVM(VirtualMachine* VM);
    void removeVM(int index);
    void updateVMData(int index, QString name, QString cores, QString ram, QString videoMemory);

    QString name() const;
    QString stats() const;
    void setStats(const QString &newStats);

    QVector<VirtualMachine *> getVirtualMachines() const;

    int getMaximum_number_of_vms() const;

    void setName(const QString &newName);

private:
    QString m_name;
    QString m_stats;
    QVector<VirtualMachine*> virtualMachines;

    int maximum_number_of_vms = 7;
};

#endif // USER_H
