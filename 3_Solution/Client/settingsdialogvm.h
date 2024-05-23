#ifndef SETTINGSDIALOGVM_H
#define SETTINGSDIALOGVM_H

#include <QMainWindow>

namespace Ui {
class settingsDialogVM;
}

class settingsDialogVM : public QMainWindow
{
    Q_OBJECT

public:
    explicit settingsDialogVM(QString ram, QString cores, QString videoMemory, QWidget *parent = nullptr);
    ~settingsDialogVM();

signals:
    void vmData(QString name, int cores, int ram, int videoMemory);
    void cancel();
    void windowClosed();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::settingsDialogVM *ui;
};

#endif // SETTINGSDIALOGVM_H
