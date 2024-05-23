#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "MyClient.h"
#include "mainwindow.h"
#include "registerwindow.h"

namespace Ui {
class LogInWindow;
}

class LogInWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogInWindow(QWidget *parent = nullptr);
    ~LogInWindow();

signals:
    void windowClosed();
private slots:
    void on_LogInButton_clicked();
    void handleLogIn(uint32_t);
    void on_RegisterButton_clicked();

    void on_CloseButton_clicked();
private:
    Ui::LogInWindow *ui;
};

#endif // LOGINWINDOW_H
