#ifndef DIALOGCREATEVM_H
#define DIALOGCREATEVM_H

#include "ui_dialogcreatevm.h"
#include <QDialog>
#include <QStackedWidget>

namespace Ui {
class DialogCreateVM;
}

class DialogCreateVM : public QDialog
{
    Q_OBJECT

public:

    DialogCreateVM(QWidget *parent = nullptr)
        : QDialog(parent), ui(new Ui::DialogCreateVM)
    {
        // Setarea dimensiunilor și stilului fereastră
        resize(1000, 500);
        setMinimumSize(QSize(1000, 500));
        setStyleSheet("background-color: rgb(45, 45, 45);");

        // Crearea widget-ului central și a layout-ului vertical principal
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        // Crearea barei de sus (Top Bar)
        QFrame *topBar = new QFrame(this);
        topBar->setObjectName("Top_Bar");
        topBar->setMaximumSize(QSize(16777215, 40));
        topBar->setStyleSheet("background-color: rgb(35, 35, 35);");
        topBar->setFrameShape(QFrame::NoFrame);
        topBar->setFrameShadow(QFrame::Raised);
        QHBoxLayout *topBarLayout = new QHBoxLayout(topBar);
        topBarLayout->setSpacing(0);
        topBarLayout->setContentsMargins(0, 0, 0, 0);

        // Crearea butonului de toggle
        QPushButton *toggleButton = new QPushButton("TOGGLE", this);
        toggleButton->setObjectName("Btn_Toggle");
        toggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        toggleButton->setStyleSheet("color: rgb(255, 255, 255); border: 0px solid;");
        connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleButtonClick);

        // Adăugarea butonului de toggle la bara de sus
        topBarLayout->addWidget(toggleButton);

        // Adăugarea barei de sus la layout-ul principal
        mainLayout->addWidget(topBar);

        // Crearea conținutului și a layout-ului orizontal
        QFrame *contentFrame = new QFrame(this);
        contentFrame->setObjectName("Content");
        contentFrame->setFrameShape(QFrame::NoFrame);
        contentFrame->setFrameShadow(QFrame::Raised);
        QHBoxLayout *contentLayout = new QHBoxLayout(contentFrame);
        contentLayout->setSpacing(0);
        contentLayout->setContentsMargins(0, 0, 0, 0);

        // Crearea meniului din stânga
        QFrame *leftMenuFrame = new QFrame(this);
        leftMenuFrame->setObjectName("frame_left_menu");
        leftMenuFrame->setMinimumSize(QSize(70, 0));
        leftMenuFrame->setMaximumSize(QSize(70, 16777215));
        leftMenuFrame->setStyleSheet("background-color: rgb(35, 35, 35);");
        leftMenuFrame->setFrameShape(QFrame::StyledPanel);
        leftMenuFrame->setFrameShadow(QFrame::Raised);
        QVBoxLayout *leftMenuLayout = new QVBoxLayout(leftMenuFrame);
        leftMenuLayout->setSpacing(0);
        leftMenuLayout->setContentsMargins(0, 0, 0, 0);

        // Crearea butoanelor de pagină în meniul din stânga
        QPushButton *page1Button = new QPushButton("Page 1", this);
        page1Button->setObjectName("btn_page_1");
        page1Button->setMinimumSize(QSize(0, 40));
        page1Button->setStyleSheet("QPushButton { color: rgb(255, 255, 255); background-color: rgb(35, 35, 35); border: 0px solid; } QPushButton:hover { background-color: rgb(85, 170, 255); }");
        connect(page1Button, &QPushButton::clicked, this, &MainWindow::page1ButtonClick);
        leftMenuLayout->addWidget(page1Button);

        QPushButton *page2Button = new QPushButton("Page 2", this);
        page2Button->setObjectName("btn_page_2");
        page2Button->setMinimumSize(QSize(0, 40));
        page2Button->setStyleSheet("QPushButton { color: rgb(255, 255, 255); background-color: rgb(35, 35, 35); border: 0px solid; } QPushButton:hover { background-color: rgb(85, 170, 255); }");
        connect(page2Button, &QPushButton::clicked, this, &MainWindow::page2ButtonClick);
        leftMenuLayout->addWidget(page2Button);

        QPushButton *page3Button = new QPushButton("Page 3", this);
        page3Button->setObjectName("btn_page_3");
        page3Button->setMinimumSize(QSize(0, 40));
        page3Button->setStyleSheet("QPushButton { color: rgb(255, 255, 255); background-color: rgb(35, 35, 35); border: 0px solid; } QPushButton:hover { background-color: rgb(85, 170, 255); }");
        connect(page3Button, &QPushButton::clicked, this, &MainWindow::page3ButtonClick);
        leftMenuLayout->addWidget(page3Button);

        // Adăugarea meniului din stânga la layout-ul conținutului
        contentLayout->addWidget(leftMenuFrame);

        // Crearea paginilor și adăugarea lor la stacked widget
        stackedWidget = new QStackedWidget(this);

        QWidget *page1 = new QWidget(this);
        QLabel *label1 = new QLabel("PAGE 1", this);
        label1->setStyleSheet("color: #FFF;");
        label1->setAlignment(Qt::AlignCenter);
        QVBoxLayout *page1Layout = new QVBoxLayout(page1);
        page1Layout->addWidget(label1);
        stackedWidget->addWidget(page1);

        QWidget *page2 = new QWidget(this);
        QLabel *label2 = new QLabel("PAGE 2", this);
        label2->setStyleSheet("color: #FFF;");
        label2->setAlignment(Qt::AlignCenter);
        QVBoxLayout *page2Layout = new QVBoxLayout(page2);
        page2Layout->addWidget(label2);
        stackedWidget->addWidget(page2);

        QWidget *page3 = new QWidget(this);
        QLabel *label3 = new QLabel("PAGE 3", this);
        label3->setStyleSheet("color: #FFF;");
        label3->setAlignment(Qt::AlignCenter);
        QVBoxLayout *page3Layout = new QVBoxLayout(page3);
        page3Layout->addWidget(label3);
        stackedWidget->addWidget(page3);

        // Adăugarea stacked widget-ului la layout-ul conținutului
        contentLayout->addWidget(stackedWidget);

        // Adăugarea layout-ului conținutului la layout-ul principal
        mainLayout->addWidget(contentFrame);

        // Setarea paginii curente
        stackedWidget->setCurrentIndex(0);
    }

private slots:
    void toggleButtonClick() {
        // Implementarea acțiunii pentru butonul de toggle
    }

    void page1ButtonClick() {
        stackedWidget->setCurrentIndex(0);
    }

    void page2ButtonClick() {
        stackedWidget->setCurrentIndex(1);
    }

    void page3ButtonClick() {
        stackedWidget->setCurrentIndex(2);
    }

private:
    QStackedWidget *stackedWidget;
};

#endif // DIALOGCREATEVM_H
