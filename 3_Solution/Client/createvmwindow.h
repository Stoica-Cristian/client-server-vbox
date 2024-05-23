#ifndef CREATEVMWINDOW_H
#define CREATEVMWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QLabel>

namespace Ui {
class CreateVMWindow;
}

class CreateVMWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateVMWindow(QWidget *parent = nullptr);
    ~CreateVMWindow();

private:
    Ui::CreateVMWindow *ui;
    QStackedWidget *stackedWidget;
};

#endif // CREATEVMWINDOW_H
