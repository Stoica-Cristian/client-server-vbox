#ifndef RESPONSECHANGEDCREDENTIALS_H
#define RESPONSECHANGEDCREDENTIALS_H

#include <QDialog>

namespace Ui {
class ResponseChangedCredentials;
}

class ResponseChangedCredentials : public QDialog
{
    Q_OBJECT

public:
    explicit ResponseChangedCredentials(QWidget *parent = nullptr);
    ~ResponseChangedCredentials();

private:
    Ui::ResponseChangedCredentials *ui;
};

#endif // RESPONSECHANGEDCREDENTIALS_H
