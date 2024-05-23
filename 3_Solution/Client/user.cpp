#include "user.h"

User::User(QString name)
    : m_name(name)
{

}

int User::addVM(VirtualMachine *virtualMachine)
{
    if(virtualMachines.size() + 1 > maximum_number_of_vms)
        return 1;
    virtualMachines.append(virtualMachine);
    return 0;
}

void User::removeVM(int index)
{
    VirtualMachine* vm = virtualMachines.at(index);
    virtualMachines.erase(virtualMachines.begin() + index);
    vm->~VirtualMachine();
}

void User::updateVMData(int index, QString name, QString cores, QString ram, QString videoMemory)
{
    VirtualMachine* currentVM = virtualMachines.at(index);
    currentVM->setName(name);
    currentVM->setCores(cores);
    currentVM->setRam(ram);
    currentVM->setVideoMemory(videoMemory);
}

QString User::name() const
{
    return m_name;
}

QString User::stats() const
{
    return m_stats;
}

void User::setStats(const QString &newStats)
{
    m_stats = newStats;
}

QVector<VirtualMachine *> User::getVirtualMachines() const
{
    return virtualMachines;
}

int User::getMaximum_number_of_vms() const
{
    return maximum_number_of_vms;
}

void User::setName(const QString &newName)
{
    m_name = newName;
}
