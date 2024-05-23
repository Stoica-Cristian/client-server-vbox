#include "registerwindow.h"
#include "ui_loginwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);

    QObject::connect(MyClient::getInstanceClient(), &MyClient::registered, this, &RegisterWindow::handleRegister);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background: transparent;");
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_BackToLogInButton_clicked()
{
    this->hide();
    LogInWindow* logInWindow = new LogInWindow;
    logInWindow->show();
}

void RegisterWindow::on_RegisterButton_clicked()
{
    if(ui->Username->text() == "")
    {
        ui->StatusRegister->setText("Username field is empty.");
        return;
    }

    if(ui->Password->text() == "" && ui->RepeatPassword->text() == "")
    {
        ui->StatusRegister->setText("Both password fields are empty.");
        return;
    }


    if(ui->Password->text() == "" || ui->RepeatPassword->text() == "")
    {
        ui->StatusRegister->setText("One of the password fields is empty.");
        return;
    }

    if(ui->Password->text() != ui->RepeatPassword->text())
    {
        ui->Password->clear();
        ui->RepeatPassword->clear();
        ui->StatusRegister->setText("The passwords entered do not match.");
    }
    else
    {
        QString username = ui->Username->text();
        QString password = ui->Password->text();

        std::string userPassStr = username.toStdString();
        userPassStr += ' ';
        userPassStr += password.toStdString();

        MyClient::getInstanceClient()->RequestRegister(userPassStr);
    }
}

/*
Return codes:
    0 - success
    1 - already registered
    2 - error (any type)
*/
void RegisterWindow::handleRegister(uint32_t retCode)
{
    if (retCode == 0)
    {
        // implement delay and show message or a dialog
        this->hide();
        LogInWindow* logIn = new LogInWindow;
        logIn->show();
    }
    else if (retCode == 1)
    {
        ui->StatusRegister->setText("Already registered!");
    }
    else
    {
        ui->StatusRegister->setText("Another type of error!");
    }

}

