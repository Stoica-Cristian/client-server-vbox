#include "fileexplorer.h"
#include "ui_fileexplorer.h"

FileExplorer::FileExplorer(QString path, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileExplorer)
{
    ui->setupUi(this);

    computePath(path);
    this->setWindowTitle(titlePath);
    this->setWindowIcon(QIcon(":/Icons100/Icons100/folder (1).png"));

    dirModel = new QFileSystemModel(this);
    dirModel->setReadOnly(false);
    dirModel->setRootPath(path);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    ui->treeView->setModel(dirModel);

    QModelIndex index = dirModel->index(path);

    ui->treeView->setRootIndex(index);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    fileModel = new QFileSystemModel(this);
    fileModel->setReadOnly(false);
    fileModel->setRootPath(path);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    ui->listView->setModel(fileModel);
    QString sPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));


}

FileExplorer::~FileExplorer()
{
    delete ui;
}

void FileExplorer::computePath(QString path)
{
    QDir dir(path);

    QStringList dirs = dir.path().split("/", Qt::SkipEmptyParts);
    username = dirs.at(dirs.size() - 2);
    titlePath = "File Explorer | home/" + username;
    vmDesciption = dirs.last();
}

void FileExplorer::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));

    QDir dir(sPath);
    QStringList dirs = dir.path().split("/", Qt::SkipEmptyParts);

    titlePath = "File Explorer | home/" + username;
    int indexStart = dirs.indexOf(vmDesciption);
    for(int i = indexStart + 1; i < dirs.size(); ++i)
        titlePath += "/" + dirs.at(i);

    this->setWindowTitle(titlePath);
}

