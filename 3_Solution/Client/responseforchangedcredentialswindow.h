#ifndef RESPONSEFORCHANGEDCREDENTIALSWINDOW_H
#define RESPONSEFORCHANGEDCREDENTIALSWINDOW_H

#include <QMainWindow>

namespace Ui {
class ResponseForChangedCredentialsWindow;
}

class ResponseForChangedCredentialsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResponseForChangedCredentialsWindow(uint32_t retCode, QWidget *parent = nullptr);
    ~ResponseForChangedCredentialsWindow();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ResponseForChangedCredentialsWindow *ui;
};

#endif // RESPONSEFORCHANGEDCREDENTIALSWINDOW_H
