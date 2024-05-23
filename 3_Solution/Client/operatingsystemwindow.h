#ifndef OPERATINGSYSTEMWINDOW_H
#define OPERATINGSYSTEMWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <map>
#include <QTimer>
#include <QTime>
#include <thread>
#include <chrono>

#include "fileexplorer.h"
#include "qlabel.h"
#include "terminalline.h"

namespace Ui {
class OperatingSystemWindow;
}

class OperatingSystemWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit OperatingSystemWindow(QString path, QWidget *parent = nullptr);
    ~OperatingSystemWindow();
signals:
    void restart();
    void close();
private slots:
    void updateClock();
    void on_Restart_clicked();
    void on_FIleExplorer_clicked();
    void on_ClearDesktop_clicked();
    void on_Terminal_clicked();
    void executeCommand(QString commandAndArs);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::OperatingSystemWindow *ui;

    QString path;
    QString user;
    QString terminalPath;
    QString currentRealPath;
    QString homePath = "~";
    QString distribution;

    void executeLsCommand();
    void executeCdCommand(QString directory);
    void executeMkdirCommand(QString directory);
    void executeTouchCommand(QString filename);
    void executeCatCommand(QString filename);
    void executeExitCommand();
    void executeHelpCommand();
    void executeRmCommand(QString toDelete);
    void executeLlCommand();

    void computePath();
    void saveCommands();

    std::map<std::string, std::string> commandDescriptions;
};

QString permissionsToString(QFile::Permissions permissions);

#endif // OPERATINGSYSTEMWINDOW_H
