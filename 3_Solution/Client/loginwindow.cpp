#include "loginwindow.h"
#include "ui_loginwindow.h"

LogInWindow::LogInWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LogInWindow)
{
    ui->setupUi(this);

    QObject::connect(MyClient::getInstanceClient(), &MyClient::loggedIn, this, &LogInWindow::handleLogIn);


    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background: transparent;");
}

LogInWindow::~LogInWindow()
{
    delete ui;
}

void LogInWindow::on_LogInButton_clicked()
{
    if(ui->Username->text() == "")
    {
        ui->StatusLogIn->setText("Username field is empty.");
        return;
    }

    if(ui->Password->text() == "")
    {
        ui->StatusLogIn->setText("Password field is empty.");
        return;
    }

    QString username = ui->Username->text();
    QString password = ui->Password->text();

    std::string usernameStr = username.toStdString();
    usernameStr += ' ';
    usernameStr += password.toStdString();

    MyClient::getInstanceClient()->RequestLogIn(usernameStr);
}
/*
Return codes:
    0 - success
    1 - incorrect password # removed
    2 - the user is not registered
    3 - error (any type)
*/
void LogInWindow::handleLogIn(uint32_t retCode)
{
    if (retCode == 0)
    {
        this->hide();

        if( MyClient::getInstanceClient()->getMainWindowAlreadyOpened() == false)
        {
            MainWindow* mainWindow = new MainWindow;
            MyClient::getInstanceClient()->setMainWindowAlreadyOpened(true);
            mainWindow->showMaximized();
        }
    }
    // removed for safety resons
    // else if (retCode == 1)
    // {
    //     ui->StatusLogIn->setText("Incorrect password, try again!");
    // }
    else if (retCode == 2)
    {
        ui->StatusLogIn->setText("The user is not registered!");
        ui->Username->setText("");
        ui->Password->setText("");
    }
    else
    {
        ui->StatusLogIn->setText("Another type of error!");
        ui->Username->setText("");
        ui->Password->setText("");
    }
}
void LogInWindow::on_RegisterButton_clicked()
{
    this->hide();
    this->setDisabled(true);
    RegisterWindow* registerWindow = new RegisterWindow;
    registerWindow->show();
}


void LogInWindow::on_CloseButton_clicked()
{
    this->deleteLater();
}

