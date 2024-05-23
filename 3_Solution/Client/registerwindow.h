#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "loginwindow.h"

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

signals:

private slots:
    void on_BackToLogInButton_clicked();
    void on_RegisterButton_clicked();
    void handleRegister(uint32_t retCode);
private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
