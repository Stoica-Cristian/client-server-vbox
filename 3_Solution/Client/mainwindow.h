#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtWidgets/QGraphicsView>

#include "qpushbutton.h"
#include "virtualmachine.h"
#include "addvmdialog.h"
#include "settingsdialogvm.h"
#include "settingsdialoguser.h"
#include "operatingsystemwindow.h"
#include "responseforchangedcredentialswindow.h"

#include <thread>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void windowClosed();
private slots:
    void toggleButtons(QPushButton *clickedButton);
    void toggleData(QPushButton *clickedButton);
    void on_AddButton_clicked();
    void on_RemoveButton_clicked();
    void on_SettingsButton_clicked();
    void on_StartButton_clicked();
    void restartVirtualMachine();

    void addVM(QString name, QString distro, int storage, int ram, int videoMemory);
    void changeVMData(QString name, int cores, int ram, int videoMemory);
    void enableWindow();
    void changeCredentials(QString newUsername, QString newPassword);

    void handleResponseCredentialsChanged(uint32_t retCode);
    void updateChart(qreal runningValue, qreal stoppedValue);
protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    std::vector<QPushButton *> buttons;
    QPushButton* selectedButton = nullptr;
    static int buttonIndex;

    QString styleSheetVMButton = "QPushButton{height:100px;color:black; font-size: 25px; font-weight: bold; background-color: rgba(230,230,230, 0.7); border-radius: 15px;}QWidget:hover{background-color: rgba(230,230,230, 1);}";
    QString styleSheetVMButtonPressed = "QPushButton{height:100px;color:black; font-size: 25px; font-weight: bold; background-color: rgba(230,230,230, 1); border-radius: 15px;}";

    QString styleSheetUserButton = "QPushButton{height:100px;color:black; font-size: 25px; font-weight: bold; background-color: rgba(249,170,51, 0.7); border-radius: 15px;}QWidget:hover{background-color: rgb(249,170,51);}";
    QString styleSheetUserButtonPressed = "QPushButton{height:100px;color:black; font-size: 25px; font-weight: bold; background-color: rgb(249,170,51); border-radius: 15px;}";

    QPieSlice *runningSlice = nullptr;
    QPieSlice *stoppedSlice = nullptr;
};
#endif // MAINWINDOW_H
