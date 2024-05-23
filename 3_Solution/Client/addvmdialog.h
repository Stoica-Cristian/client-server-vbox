#ifndef ADDVMDIALOG_H
#define ADDVMDIALOG_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class AddVMDialog;
}

class AddVMDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddVMDialog(QWidget *parent = nullptr);
    ~AddVMDialog();

signals:
    void vmData(QString name, QString distro, int storage, int ram, int videoMemory);
    void cancel();
    void windowClosed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::AddVMDialog *ui;
};

#endif // ADDVMDIALOG_H
