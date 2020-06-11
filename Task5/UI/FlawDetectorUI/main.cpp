#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("LLVM Data-Mining Flaw Detector");
    w.show();
    return a.exec();
}
