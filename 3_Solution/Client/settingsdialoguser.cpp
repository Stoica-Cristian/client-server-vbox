#include "settingsdialoguser.h"
#include "qevent.h"
#include "ui_settingsdialoguser.h"

settingsDialogUser::settingsDialogUser(QString currentUsername, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::settingsDialogUser)
{
    ui->setupUi(this);
    ui->Username->setText(currentUsername);
}

settingsDialogUser::~settingsDialogUser()
{
    delete ui;
}
void settingsDialogUser::on_buttonBox_accepted()
{
    if(ui->Password->text() == "" && ui->ConfirmPassword->text() == "")
    {
        ui->Message->setText("Both password fields are empty.");
        return;
    }

    if(ui->Password->text() == "" || ui->ConfirmPassword->text() == "")
    {
        ui->Message->setText("One of the password fields is empty.");
        return;
    }

    if(ui->Password->text() != ui->ConfirmPassword->text())
    {
        ui->Password->clear();
        ui->ConfirmPassword->clear();
        ui->Message->setText("The passwords entered do not match.");
    }
    else{
        QString username = ui->Username->text();
        QString password = ui->Password->text();
        emit request(username, password);
        this->deleteLater();
    }
}

void settingsDialogUser::on_buttonBox_rejected()
{
    emit cancel();
    this->deleteLater();
}

void settingsDialogUser::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}
