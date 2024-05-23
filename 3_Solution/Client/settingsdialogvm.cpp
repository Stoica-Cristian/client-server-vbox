#include "settingsdialogvm.h"
#include "qevent.h"
#include "ui_settingsdialogvm.h"

settingsDialogVM::settingsDialogVM(QString ram, QString cores, QString videoMemory, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::settingsDialogVM)
{
    ui->setupUi(this);

    connect(ui->MemorySlider, &QSlider::valueChanged, this, [this](int value) {
        ui->SelectedRAM->setText(QString::number(value));
    });

    connect(ui->CoresSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->SelectedCores->setText(QString::number(value));
    });

    connect(ui->VideoMemorySlider, &QSlider::valueChanged, this, [this](int value) {
        ui->SelectedVideoMemory->setText(QString::number(value));
    });

    ui->MemorySlider->setValue(ram.toInt());
    ui->SelectedRAM->setText(ram);
    ui->CoresSlider->setValue(cores.toInt());
    ui->SelectedCores->setText(cores);
    ui->VideoMemorySlider->setValue(videoMemory.toInt());
    ui->SelectedVideoMemory->setText(videoMemory);
}

settingsDialogVM::~settingsDialogVM()
{
    delete ui;
}

void settingsDialogVM::on_buttonBox_accepted()
{
    QString name = ui->Name->text();
    int ram = ui->MemorySlider->value();
    int cores = ui->CoresSlider->value();
    int videoMemory = ui->VideoMemorySlider->value();
    emit vmData(name, cores, ram, videoMemory);
    this->deleteLater();

}

void settingsDialogVM::on_buttonBox_rejected()
{
    emit cancel();
    this->deleteLater();
}

void settingsDialogVM::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}
