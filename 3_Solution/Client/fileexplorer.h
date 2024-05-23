#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>

namespace Ui {
class FileExplorer;
}

class FileExplorer : public QDialog
{
    Q_OBJECT

public:
    explicit FileExplorer(QString path, QWidget *parent = nullptr);
    ~FileExplorer();

private slots:

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::FileExplorer *ui;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    QString username;
    QString titlePath;
    QString vmDesciption;

    void computePath(QString path);
};

#endif // FILEEXPLORER_H
