#include "responsechangedcredentials.h"
#include "ui_responsechangedcredentials.h"

ResponseChangedCredentials::ResponseChangedCredentials(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResponseChangedCredentials)
{
    ui->setupUi(this);
}

ResponseChangedCredentials::~ResponseChangedCredentials()
{
    delete ui;
}
