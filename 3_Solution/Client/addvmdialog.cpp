#include "addvmdialog.h"
#include "ui_addvmdialog.h"

AddVMDialog::AddVMDialog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddVMDialog)
{
    ui->setupUi(this);

    QStringList list;
    list << "Debian" << "Arch" << "Fedora" << "Ubuntu"  << "Linux Mint" << "Kali" << "Manjaro";
    ui->Distro->addItems(list);

    connect(ui->MemorySlider, &QSlider::valueChanged, this, [this](int value) {
        ui->SelectedRAM->setText(QString::number(value));
    });

    connect(ui->StorageSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->SelectedStorage->setText(QString::number(value));
    });

    connect(ui->VideoMemorySlider, &QSlider::valueChanged, this, [this](int value) {
        ui->SelectedVideoMemory->setText(QString::number(value));
    });
}

AddVMDialog::~AddVMDialog()
{
    delete ui;
}

void AddVMDialog::on_buttonBox_accepted()
{
    QString name = ui->Name->text();
    QString distribution = ui->Distro->currentText();
    int storage = ui->StorageSlider->value();
    int ram = ui->MemorySlider->value();
    int videoMemory = ui->VideoMemorySlider->value();
    emit vmData(name, distribution, storage, ram, videoMemory);
    this->deleteLater();
}


void AddVMDialog::on_buttonBox_rejected()
{
    emit cancel();
    this->deleteLater();
}

void AddVMDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}

