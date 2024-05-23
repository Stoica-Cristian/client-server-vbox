#include "responseforchangedcredentialswindow.h"
#include "ui_responseforchangedcredentialswindow.h"

ResponseForChangedCredentialsWindow::ResponseForChangedCredentialsWindow(uint32_t retCode, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ResponseForChangedCredentialsWindow)
{
    ui->setupUi(this);

    if(retCode == 0)
        ui->Message->setText("Your credentials has been successfully saved.");
    else
        ui->Message->setText("An error occurred when modifying the data. Please try again.");
}

ResponseForChangedCredentialsWindow::~ResponseForChangedCredentialsWindow()
{
    delete ui;
}

void ResponseForChangedCredentialsWindow::on_buttonBox_accepted()
{
    this->deleteLater();
}

void ResponseForChangedCredentialsWindow::on_buttonBox_rejected()
{
    this->deleteLater();
}
