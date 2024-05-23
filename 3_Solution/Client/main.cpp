#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include "worker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogInWindow logIn;
    Worker *worker = new Worker();

    QThread thread;
    QObject::connect(&thread,&QThread::started,worker,&Worker::run);
    QObject::connect(worker, &Worker::destroyed, &thread, &QThread::quit);

    thread.start();
    worker->moveToThread(&thread);

    logIn.show();

    return a.exec();
}
