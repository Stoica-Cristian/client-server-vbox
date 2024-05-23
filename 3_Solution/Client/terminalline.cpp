#include "terminalline.h"
#include "ui_terminalline.h"

TerminalLine::TerminalLine(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TerminalLine)
{
    ui->setupUi(this);
}

TerminalLine::TerminalLine(QString user, QString hostname, QString path, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TerminalLine)
{
    ui->setupUi(this);

    ui->user->setText(user + "@");
    ui->hostname->setText(hostname);
    ui->path->setText(path);
    ui->command->setFocus();
    ui->command->selectAll();
}

TerminalLine::~TerminalLine()
{
    delete ui;
}

QLineEdit *TerminalLine::getCommand() const
{
    return ui->command;
}

void TerminalLine::on_command_returnPressed()
{
    ui->command->setReadOnly(true);
    emit execute(ui->command->text());
}

