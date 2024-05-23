#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "MyClient.h"

int MainWindow::buttonIndex = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(" ");
    setWindowIcon(QIcon(":/Icons100/Icons100/categories.png"));

    connect(this, &MainWindow::windowClosed, MyClient::getInstanceClient(), &MyClient::SendUserData);
    connect(MyClient::getInstanceClient(), &MyClient::credentialsChanged, this, &MainWindow::handleResponseCredentialsChanged);


    User* user = MyClient::getInstanceClient()->getUser();
    ui->UserButton->setText(user->name());
    ui->UserButton->setStyleSheet(styleSheetUserButtonPressed);
    buttons.push_back(ui->UserButton);
    connect(ui->UserButton, &QPushButton::clicked, this, [=]() {
        toggleButtons(ui->UserButton);
    });
    selectedButton = ui->UserButton;
    ui->RemoveButton->setDisabled(true);
    ui->StartButton->setDisabled(true);

    for(auto& vm : user->getVirtualMachines())
    {
        QPushButton *button = new QPushButton(vm->name() + "-" + vm->type() + "-" + vm->distribution());

        button->setIcon(QIcon(":/Icons100/Icons100/linux.png"));
        button->setIconSize(QSize(50,50));
        button->setStyleSheet(styleSheetVMButton);
        ui->left->layout()->addWidget(button);

        buttons.push_back(button);

        connect(button, &QPushButton::clicked, this, [=]() {
            toggleButtons(button);
        });
    }

    ui->stackedWidget->setCurrentIndex(1);

    ui->Nr_Vms->setText(QString::number(buttons.size() - 1));
    ui->Nr_Vms_Opened->setText("0");
    ui->Nr_Vms_Onsleep->setText(QString::number(buttons.size() - 1));


    // Piechart
    QPieSeries *series = new QPieSeries();
    runningSlice = series->append("Running", 0);
    stoppedSlice = series->append("Stopped", 100);

    QFont font;
    font.setPointSize(14);
    runningSlice->setLabelFont(font);
    stoppedSlice->setLabelFont(font);

    runningSlice->setLabelVisible(true);
    stoppedSlice->setLabelVisible(true);

    // runningSlice->setColor(Qt::green);
    // stoppedSlice->setColor(Qt::red);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Virtual Machines Stats");
    chart->legend()->hide();
    chart->setVisible(true);
    chart->setTitleFont(font);

    QColor backgroundColor = QColor(204,204,204);
    chart->setBackgroundBrush(QBrush(backgroundColor));

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setVisible(true);

    chartView->setFixedSize(800, 600);

    ui->InfoUser->layout()->setAlignment(Qt::AlignTop);

    ui->InfoUser->layout()->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleButtons(QPushButton *clickedButton)
{
    selectedButton = clickedButton;

    if(selectedButton == ui->UserButton){
        ui->RemoveButton->setDisabled(true);
        ui->StartButton->setDisabled(true);
        ui->stackedWidget->setCurrentIndex(1);

        int cnt = 0;
        for(auto vm : MyClient::getInstanceClient()->getUser()->getVirtualMachines())
        {
            if(vm->status() == "Running")
                cnt++;
        }

        updateChart(cnt * 10, (buttons.size() - cnt - 1) * 10);

        ui->Nr_Vms->setText(QString::number(buttons.size() - 1));
        ui->Nr_Vms_Opened->setText(QString::number(cnt));
        ui->Nr_Vms_Onsleep->setText(QString::number(buttons.size() - cnt - 1));
    }
    else
    {
        ui->RemoveButton->setDisabled(false);
        ui->StartButton->setDisabled(false);
        ui->stackedWidget->setCurrentIndex(0);
    }

    for (auto button : buttons) {
        if (button == clickedButton) {
            if(button == ui->UserButton)
                button->setStyleSheet(styleSheetUserButtonPressed);
            else
                button->setStyleSheet(styleSheetVMButtonPressed);
        } else {
            if(button == ui->UserButton)
                button->setStyleSheet(styleSheetUserButton);
            else
                button->setStyleSheet(styleSheetVMButton);
        }
    }

    if(clickedButton != ui->UserButton)
        toggleData(clickedButton);
}

void MainWindow::toggleData(QPushButton *clickedButton)
{
    int index = 0;
    for(auto button : buttons)
    {
        if(index == 0){
            index++;
            continue;
        }
        if(button == clickedButton)
            break;
        index++;
    }
    index -= 1; // first button is UserButton
    VirtualMachine* currentVM = MyClient::getInstanceClient()->getUser()->getVirtualMachines().at(index);

    ui->Name->setText(currentVM->name());
    ui->OS->setText(currentVM->type() + "-" + currentVM->distribution());
    ui->VideoMemory->setText(currentVM->videoMemory() + " MB");
    ui->RAM->setText(currentVM->ram() + " MB");
    ui->Cores->setText(currentVM->cores());
    ui->HDD->setText(currentVM->storage() + " GB");
}

void MainWindow::on_AddButton_clicked()
{
    AddVMDialog* addvm = new AddVMDialog;
    connect(addvm, &AddVMDialog::vmData, this, &MainWindow::addVM);
    connect(addvm, &AddVMDialog::cancel, this, &MainWindow::enableWindow);
    connect(addvm, &AddVMDialog::windowClosed, this, &MainWindow::enableWindow);
    addvm->show();
    this->setEnabled(false);
}

void MainWindow::on_RemoveButton_clicked()
{
    if(selectedButton == ui->UserButton)
        return;

    int index = 0;
    for(auto button : buttons)
    {
        if(button == selectedButton)
        {
            ui->left->layout()->removeWidget(selectedButton);
            selectedButton->deleteLater();
            break;
        }
        index++;
    }
    buttons.erase(buttons.begin() + index);
    MyClient::getInstanceClient()->getUser()->removeVM(index - 1);
    selectedButton = ui->UserButton;
    ui->UserButton->click();

    ui->AddButton->setEnabled(true);
}

void MainWindow::on_SettingsButton_clicked()
{
    if(selectedButton == ui->UserButton)
    {
        settingsDialogUser* settings = new settingsDialogUser(MyClient::getInstanceClient()->getUser()->name());
        connect(settings, &settingsDialogUser::request, this, &MainWindow::changeCredentials);
        connect(settings, &settingsDialogUser::cancel, this, &MainWindow::enableWindow);
        connect(settings, &settingsDialogUser::windowClosed, this, &MainWindow::enableWindow);
        settings->show();
        this->setEnabled(false);
    }
    else
    {
        int index = 0;
        for(auto button : buttons)
        {
            if(button == selectedButton)
                break;
            index++;
        }

        VirtualMachine* vm = MyClient::getInstanceClient()->getUser()->getVirtualMachines().at(index - 1);

        settingsDialogVM* settings = new settingsDialogVM(vm->ram(), vm->cores(), vm->videoMemory());
        connect(settings, &settingsDialogVM::vmData, this, &MainWindow::changeVMData);
        connect(settings, &settingsDialogVM::cancel, this, &MainWindow::enableWindow);
        connect(settings, &settingsDialogVM::windowClosed, this, &MainWindow::enableWindow);
        settings->show();
        this->setEnabled(false);
    }
}

void MainWindow::on_StartButton_clicked()
{
    int index = 0;
    for(auto button : buttons)
    {
        if(button == selectedButton)
            break;
        index++;
    }
    VirtualMachine* currentVM = MyClient::getInstanceClient()->getUser()->getVirtualMachines().at(index - 1);

    if(currentVM->status() == "Running")
        return;

    currentVM->setStatus("Running");

    OperatingSystemWindow* osw = new OperatingSystemWindow(currentVM->getPath());
    connect(osw, &OperatingSystemWindow::restart, this, &MainWindow::restartVirtualMachine);
    connect(osw, &OperatingSystemWindow::close, this, [=]() {
        currentVM->setStatus("Stopped");
        selectedButton->setIcon(QIcon(":/Icons100/Icons100/linux.png"));
    });
    osw->showMaximized();

    selectedButton->setIcon(QIcon(":/Icons100/Icons100/play.png"));
}

void MainWindow::restartVirtualMachine()
{
    int index = 0;
    for(auto button : buttons)
    {
        if(button == selectedButton)
            break;
        index++;
    }
    VirtualMachine* currentVM = MyClient::getInstanceClient()->getUser()->getVirtualMachines().at(index - 1);

    currentVM->setStatus("Running");

    OperatingSystemWindow* osw = new OperatingSystemWindow(currentVM->getPath());
    connect(osw, &OperatingSystemWindow::restart, this, &MainWindow::restartVirtualMachine);
    connect(osw, &OperatingSystemWindow::close, this, [=]() {
        currentVM->setStatus("Stopped");
    });
    osw->showMaximized();
}

void MainWindow::addVM(QString name, QString distro, int storage, int ram, int videoMemory)
{
    setEnabled(true);

    VirtualMachine* vm = new VirtualMachine(MyClient::getInstanceClient()->getUser()->name(), name, "Linux", distro);
    vm->setStorage(QString::number(storage));
    vm->setCores("1");
    vm->setRam(QString::number(ram));
    vm->setVideoMemory(QString::number(videoMemory));

    if(MyClient::getInstanceClient()->getUser()->addVM(vm) == 1)
    {
        // dialog to notify about that

        vm->deleteLater();
        return;
    }
    QPushButton *button = new QPushButton(name + "-" + "Linux" + "-" + distro);

    button->setIcon(QIcon(":/Icons100/Icons100/linux.png"));
    button->setIconSize(QSize(50,50));
    button->setStyleSheet(styleSheetVMButton);
    ui->left->layout()->addWidget(button);

    buttons.push_back(button);

    connect(button, &QPushButton::clicked, this, [=]() {
        toggleButtons(button);
    });

    toggleButtons(button);
    if(MyClient::getInstanceClient()->getUser()->getVirtualMachines().size() == MyClient::getInstanceClient()->getUser()->getMaximum_number_of_vms())
        ui->AddButton->setEnabled(false);
}

void MainWindow::changeVMData(QString name, int cores, int ram, int videoMemory)
{
    setEnabled(true);

    int index = 0;
    for(auto button : buttons)
    {
        if(button == selectedButton)
            break;
        index++;
    }

    VirtualMachine* currentVM = MyClient::getInstanceClient()->getUser()->getVirtualMachines().at(index - 1);
    if(!name.isEmpty())
    {
        currentVM->setName(name);
        selectedButton->setText(name + "-" + currentVM->type() + "-" + currentVM->distribution());
    }
    currentVM->setCores(QString::number(cores));
    currentVM->setRam(QString::number(ram));
    currentVM->setVideoMemory(QString::number(videoMemory));

    toggleData(selectedButton);
}

void MainWindow::enableWindow()
{
    setEnabled(true);
}

void MainWindow::changeCredentials(QString newUsername, QString newPassword)
{
    setEnabled(true);
    std::string credentials = newUsername.toStdString() + " " + newPassword.toStdString();
    MyClient::getInstanceClient()->RequestChangeUserData(credentials);
    ui->UserButton->setText(newUsername);
    MyClient::getInstanceClient()->getUser()->setName(newUsername);
}

void MainWindow::handleResponseCredentialsChanged(uint32_t retCode)
{
    ResponseForChangedCredentialsWindow* response = new ResponseForChangedCredentialsWindow(retCode);
    response->show();
}

void MainWindow::updateChart(qreal runningValue, qreal stoppedValue)
{
    if (runningSlice && stoppedSlice) {
        runningSlice->setValue(runningValue);
        stoppedSlice->setValue(stoppedValue);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    event->accept();
}

