#include "createvmwindow.h"
#include "qboxlayout.h"
#include "ui_createvmwindow.h"

CreateVMWindow::CreateVMWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CreateVMWindow)
{
    ui->setupUi(this);
}

CreateVMWindow::~CreateVMWindow()
{
    delete ui;
}
