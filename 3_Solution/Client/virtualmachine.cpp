#include "virtualmachine.h"

VirtualMachine::VirtualMachine(QString username, QString vmName, QString type, QString distribution)
    : m_name(vmName), m_type(type), m_distribution(distribution)
{
    this->username = username;

    QString projectPath = QCoreApplication::applicationDirPath();
    path = projectPath + "/" + "Users/" + username + "/" + m_name + "-" + m_type + "-" + m_distribution;

    CreateVM(username);
}

VirtualMachine::~VirtualMachine()
{
    QDir dir(path);

    if(dir.removeRecursively())
        qDebug() << "Directorul a fost sters cu succes.";
    else
        qDebug() << "Eroare la ștergerea directorului.";
}

QString VirtualMachine::distribution() const
{
    return m_distribution;
}

void VirtualMachine::setDistribution(const QString &newDistribution)
{
    m_distribution = newDistribution;
}

QString VirtualMachine::type() const
{
    return m_type;
}

void VirtualMachine::setType(const QString &newType)
{
    m_type = newType;
}

QString VirtualMachine::name() const
{
    return m_name;
}

void VirtualMachine::setName(const QString &newName)
{
    m_name = newName;
}

QString VirtualMachine::videoMemory() const
{
    return m_videoMemory;
}

void VirtualMachine::setVideoMemory(const QString &newVideoMemory)
{
    m_videoMemory = newVideoMemory;
}

QString VirtualMachine::ram() const
{
    return m_ram;
}

void VirtualMachine::setRam(const QString &newRam)
{
    m_ram = newRam;
}

QString VirtualMachine::cores() const
{
    return m_cores;
}

void VirtualMachine::setCores(const QString &newCores)
{
    m_cores = newCores;
}

QString VirtualMachine::storage() const
{
    return m_storage;
}

void VirtualMachine::setStorage(const QString &newStorage)
{
    m_storage = newStorage;
}

QString VirtualMachine::status() const
{
    return m_status;
}

void VirtualMachine::setStatus(const QString &newStatus)
{
    m_status = newStatus;
}

QString VirtualMachine::getPath() const
{
    return path;
}

void VirtualMachine::CreateVM(QString clientName)
{
    QDir dir(path);
    if (!dir.exists()) {
        if (dir.mkpath(path)) {
            qDebug() << "Directorul a fost creat cu succes.";
        } else {
            qDebug() << "Eroare la crearea directorului.";
        }
    }

    QString Desktop = path + "/Desktop";
    QDir DesktopDir(Desktop);
    if (!DesktopDir.exists()) {
        DesktopDir.mkpath(Desktop);
    }

    QString Documents = path + "/Documents";
    QDir DocumentsDir(Documents);
    if (!DocumentsDir.exists()) {
        DocumentsDir.mkpath(Documents);
    }

    QString Downloads = path + "/Downloads";
    QDir DownloadsDir(Downloads);
    if (!DownloadsDir.exists()) {
        DownloadsDir.mkpath(Downloads);
    }

    QString Music = path + "/Music";
    QDir MusicDir(Music);
    if (!MusicDir.exists()) {
        MusicDir.mkpath(Music);
    }

    QString Pictures = path + "/Pictures";
    QDir PicturesDir(Pictures);
    if (!PicturesDir.exists()) {
        PicturesDir.mkpath(Pictures);
    }

    QString Videos  = path + "/Videos";
    QDir VideosDir(Videos);
    if (!VideosDir.exists()) {
        VideosDir.mkpath(Videos);
    }
}
