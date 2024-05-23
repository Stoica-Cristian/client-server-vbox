#ifndef SETTINGSDIALOGUSER_H
#define SETTINGSDIALOGUSER_H

#include <QMainWindow>

namespace Ui {
class settingsDialogUser;
}

class settingsDialogUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit settingsDialogUser(QString currentUsername, QWidget *parent = nullptr);
    ~settingsDialogUser();

signals:
    void request(QString newUsername, QString newPassword);
    void cancel();
    void windowClosed();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::settingsDialogUser *ui;
};

#endif // SETTINGSDIALOGUSER_H
