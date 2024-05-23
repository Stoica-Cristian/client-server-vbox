#include "operatingsystemwindow.h"
#include "qlineedit.h"
#include "ui_operatingsystemwindow.h"
#include <QScrollBar>

OperatingSystemWindow::OperatingSystemWindow(QString path, QWidget *parent)
    : QMainWindow(parent), path(path)
    , ui(new Ui::OperatingSystemWindow)
{
    ui->setupUi(this);

    setWindowTitle(" ");
    setWindowIcon(QIcon(":/Icons100/Icons100/categories.png"));

    computePath();
    saveCommands();

    //clock
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OperatingSystemWindow::updateClock);
    timer->start(1000);
    updateClock();
    //

    ui->Desktop->setCurrentIndex(0);

    connect(ui->ShutDown, &QPushButton::clicked, this, &QMainWindow::close);

    TerminalLine* line = new TerminalLine(user, distribution, terminalPath);
    connect(line, &TerminalLine::execute, this, &OperatingSystemWindow::executeCommand);

    ui->terminal->layout()->setAlignment(Qt::AlignTop);
    ui->terminal->layout()->addWidget(line);
    line->getCommand()->setFocus();
}

OperatingSystemWindow::~OperatingSystemWindow()
{
    delete ui;
}

void OperatingSystemWindow::updateClock()
{
    QTime time = QTime::currentTime();

    ui->Clock->setText(time.toString("hh:mm:ss"));
}


void OperatingSystemWindow::on_Restart_clicked()
{
    this->hide();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    emit restart();
    this->deleteLater();
}

void OperatingSystemWindow::closeEvent(QCloseEvent *event)
{
    emit close();
    QMainWindow::closeEvent(event);
}

void OperatingSystemWindow::executeLsCommand()
{
    QDir dir(currentRealPath);

    QStringList lsOutput = dir.entryList();

    QLabel* output = new QLabel;
    output->setText(lsOutput.join("\n"));
    output->setStyleSheet("font-size: 20px; padding-left: 10px;");
    output->adjustSize();
    ui->terminal->layout()->addWidget(output);
}

void OperatingSystemWindow::executeCdCommand(QString directory)
{
    QDir dir(currentRealPath + "/" + directory);

    if(directory == "")
    {
        currentRealPath = path;
        terminalPath = homePath;
        return;
    }

    if(directory == ".")
        return;

    if(directory == "..")
    {
        QString currentRealPathCopy = currentRealPath;

        QDir dir(currentRealPath);
        QStringList dirs = dir.path().split("/", Qt::SkipEmptyParts);
        dirs.removeLast();

        if(dirs.last() == user)
            return;

        currentRealPath = dirs.join("/");

        int indexOfUser = dirs.indexOf(user);


        QDir parentDir(currentRealPath);

        if (parentDir.exists()) {
            terminalPath = homePath;
            for(int i = indexOfUser + 2; i < dirs.size(); ++i)
                terminalPath += "/" + dirs.at(i);
        } else {
            currentRealPath = currentRealPathCopy;

            QLabel* output = new QLabel;
            output->setText(QString("Directory does not exist: %1").arg(directory));
            output->setStyleSheet("font-size: 20px; padding-left: 10px;");
            output->adjustSize();
            ui->terminal->layout()->addWidget(output);
        }
        return;
    }

    if (dir.exists()) {
        terminalPath += "/" + directory;
        currentRealPath += "/" + directory;
    } else {
        QLabel* output = new QLabel;
        output->setText(QString("Directory does not exist: %1").arg(directory));
        output->setStyleSheet("font-size: 20px; padding-left: 10px;");
        output->adjustSize();
        ui->terminal->layout()->addWidget(output);
    }
}

void OperatingSystemWindow::executeMkdirCommand(QString directory)
{
    QDir dir(currentRealPath);
    dir.mkdir(directory);
}

void OperatingSystemWindow::executeTouchCommand(QString filename)
{
    QFile file(currentRealPath + "/" + filename);

    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
        }
    } else {
        // Actualizează data și ora de acces și modificare ale fișierului
        file.setFileTime(QDateTime::currentDateTime(), QFileDevice::FileAccessTime);
        file.setFileTime(QDateTime::currentDateTime(), QFileDevice::FileModificationTime);
    }
}

void OperatingSystemWindow::executeCatCommand(QString filename)
{
    QFile file(currentRealPath + "/" + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QLabel* output = new QLabel;
        output->setText("The file cannot be opened");
        output->setStyleSheet("font-size: 20px; padding-left: 10px;");
        output->adjustSize();
        ui->terminal->layout()->addWidget(output);

        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();

    QLabel* output = new QLabel;
    output->setText(content);
    output->setStyleSheet("font-size: 20px; padding-left: 10px;");
    output->adjustSize();
    ui->terminal->layout()->addWidget(output);

    file.close();
}

void OperatingSystemWindow::executeExitCommand()
{
    ui->ShutDown->click();
}

void OperatingSystemWindow::executeHelpCommand()
{
    QString commands;

    commands.append("Comenzi disponibile:\n");

    for (const auto& entry : commandDescriptions) {
        commands.append(entry.first + " - " + entry.second + "\n");
    }
    commands.chop(1);

    QLabel* output = new QLabel;
    output->setText(commands);
    output->setStyleSheet("font-size: 20px; padding-left: 10px;");
    output->adjustSize();
    ui->terminal->layout()->addWidget(output);
}

void OperatingSystemWindow::executeRmCommand(QString toDelete)
{
    QFileInfo fileInfo(currentRealPath + "/" + toDelete);
    if (!fileInfo.exists()) {
        QLabel* output = new QLabel;
        output->setText("The path does not exist.");
        output->setStyleSheet("font-size: 20px; padding-left: 10px;");
        output->adjustSize();
        ui->terminal->layout()->addWidget(output);
        return;
    }

    if (fileInfo.isDir()) {
        QDir dir(currentRealPath + "/" + toDelete);
        if (dir.removeRecursively()) {
        }
    } else {
        QFile file(currentRealPath + "/" + toDelete);
        if (file.remove()) {
        }
    }
}

void OperatingSystemWindow::executeLlCommand()
{
    QDir dir(currentRealPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::AllEntries, QDir::DirsFirst);

    QString output;
    foreach (const QFileInfo &fileInfo, fileList) {
        output.append(fileInfo.isDir() ? "d" : "-");
        output.append(permissionsToString(fileInfo.permissions()) + " ");
        output.append(user + " ");
        output.append(QString::number(fileInfo.size()) + " ");
        output.append(fileInfo.lastModified().toString("dd.MM.yyyy hh:mm:ss") + " ");
        output.append(fileInfo.fileName()  + "\n");
    }
    output.chop(1);
    QLabel* outputLabel = new QLabel;
    outputLabel->setText(output);
    outputLabel->setStyleSheet("font-size: 20px; padding-left: 10px;");
    outputLabel->adjustSize();
    ui->terminal->layout()->addWidget(outputLabel);
}

void OperatingSystemWindow::computePath()
{
    QDir dir(path);
    QStringList dirs = dir.path().split("/", Qt::SkipEmptyParts);
    user = dirs.at(dirs.size() - 2);
    terminalPath = homePath;
    currentRealPath = path;
    QString vmInfo = dirs.at(dirs.size() - 1);
    QStringList vmInfoList = vmInfo.split("-", Qt::SkipEmptyParts);
    distribution = vmInfoList.last();
}

void OperatingSystemWindow::saveCommands()
{
    commandDescriptions.insert({"ls", "Listați conținutul directorului curent."});
    commandDescriptions.insert({"cd", " Schimbați directorul curent într-un alt director specificat."});
    commandDescriptions.insert({"mkdir", "Creează un director nou în directorul curent."});
    commandDescriptions.insert({"touch", "Creează un fișier nou sau actualizează data de modificare a unui fișier existent."});
    commandDescriptions.insert({"cat", "Afișează conținutul unui fișier pe ecran."});
    commandDescriptions.insert({"rm", "Șterge un fișier sau un director."});
    commandDescriptions.insert({"ll", "ls -l"});
}

void OperatingSystemWindow::on_FIleExplorer_clicked()
{
    FileExplorer* fileExplorer = new FileExplorer(path);
    fileExplorer->show();
}

void OperatingSystemWindow::on_ClearDesktop_clicked()
{
    ui->Desktop->setCurrentIndex(0);
}

void OperatingSystemWindow::on_Terminal_clicked()
{
    ui->Desktop->setCurrentIndex(1);
}

void OperatingSystemWindow::executeCommand(QString commandAndArs)
{
    if(commandAndArs.isEmpty() || commandAndArs == " ")
        return;

    QStringList parts = commandAndArs.split(" ", Qt::SkipEmptyParts);

    QString command = parts.at(0);
    QString argument;
    if (parts.size() >= 2)
        argument =  parts.at(1);

    if (command == "ls") {
        if(argument == "-l")
            executeLlCommand();
        else
            executeLsCommand();
    }
    else if (command == "ll") {
        executeLlCommand();
    }
    else if (command == "cd") {
        executeCdCommand(argument);
    }
    else if (command == "mkdir") {
        // handle error cases
        executeMkdirCommand(argument);
    }
    else if (command == "touch") {
        // handle error cases
        executeTouchCommand(argument);
    }
    else if (command == "cat") {
        // handle error cases
        executeCatCommand(argument);
    }
    else if (command == "exit") {
        executeExitCommand();
    }
    else if (command == "rm") {
        executeRmCommand(argument);
    }
    else if (command == "help") {
        executeHelpCommand();
    } else {
        QLabel* output = new QLabel;
        output->setText("Command not found: " + command);
        output->setStyleSheet("font-size: 20px; padding-left: 10px;");
        output->adjustSize();
        ui->terminal->layout()->addWidget(output);
    }

    TerminalLine* line = new TerminalLine(user, distribution, terminalPath);
    connect(line, &TerminalLine::execute, this, &OperatingSystemWindow::executeCommand);
    ui->terminal->layout()->addWidget(line);
    line->getCommand()->setFocus();

    QTimer::singleShot(15, [=]() {
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    });
}

QString permissionsToString(QFile::Permissions permissions) {
    QString result;

    if (permissions & QFile::ReadOwner) result += "r";
    else result += "-";

    if (permissions & QFile::WriteOwner) result += "w";
    else result += "-";

    if (permissions & QFile::ExeOwner) result += "x";
    else result += "-";

    if (permissions & QFile::ReadGroup) result += "r";
    else result += "-";

    if (permissions & QFile::WriteGroup) result += "w";
    else result += "-";

    if (permissions & QFile::ExeGroup) result += "x";
    else result += "-";

    if (permissions & QFile::ReadOther) result += "r";
    else result += "-";

    if (permissions & QFile::WriteOther) result += "w";
    else result += "-";

    if (permissions & QFile::ExeOther) result += "x";
    else result += "-";

    return result;
}
