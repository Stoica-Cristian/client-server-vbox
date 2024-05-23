#ifndef TERMINALLINE_H
#define TERMINALLINE_H

#include "qlineedit.h"
#include <QWidget>

namespace Ui {
class TerminalLine;
}

class TerminalLine : public QWidget
{
    Q_OBJECT

public:
    explicit TerminalLine(QWidget *parent = nullptr);
    TerminalLine(QString user, QString hostname, QString path, QWidget *parent = nullptr);
    ~TerminalLine();

    QLineEdit* getCommand() const;

signals:
    void execute(QString command);
private slots:
    void on_command_returnPressed();
private:
    Ui::TerminalLine *ui;
};

#endif // TERMINALLINE_H
