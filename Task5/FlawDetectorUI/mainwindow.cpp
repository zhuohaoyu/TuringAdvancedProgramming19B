#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "qmessagebox.h"
#include "QString"
#include "QStringList"
#include <string>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    filePath = "";
    ui->setupUi(this);
    initSignalSlots();
    ui->lineEdit_Filepath->setText(filePath);

}

void MainWindow::initSignalSlots() {
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_SelectFile_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select Source File"), ".", tr("C/C++ Source Code(*.c *.cpp)"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Failed loading file"), tr("You didn't select any file."));
        filePath = "";
    }
    else {
        QMessageBox::information(NULL, tr("Successfully loaded file"), tr("Selected File: \n") + path);
        filePath = path;
    }
    ui->lineEdit_Filepath->setText(filePath);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_runButton_clicked()
{
    run_clang();
}


void MainWindow::run_clang()
{
    out = new QTextEdit();
    ui->scrollArea->setWidget(out);
    ui->scrollArea->setWidgetResizable(true);
    proc = new QProcess(this);
    connect(proc, SIGNAL(readyRead()), this, SLOT(on_exec_clang()));
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_clang_over(int, QProcess::ExitStatus)));
    QStringList args;// build the arguments for the executable
    QString mis =QString::number(ui->spinBox_MIS->value()), mfs = QString::number(ui->spinBox_MFS->value()),
            mcf = QString::number(ui->doubleSpinBox_MCF->value());
//    args << "--help";
    args << "-mllvm" << "-mfs=" + mfs << "-mllvm" << "-mis=" + mis <<"-mllvm" << "-min_conf=" + mcf;
    args << "-Xclang" << "-load" << "-Xclang"  << "/home/me/TuringAdvancedProgramming19B/Task5/MyPass.so";
    args << "-g" << "-c";
    args << filePath;
/*
MyPass=./MyPass.so
clang -mllvm -mfs=10 -mllvm -mis=5 -mllvm -min_conf=85 -Xclang -load -Xclang $MyPass -g -c TestMe.c
*/
//    proc->start("/home/me/TuringAdvancedProgramming19B/Task5/hello", args);
    proc->setReadChannel(QProcess::StandardOutput);
    proc->start("clang", args);
}

void MainWindow::on_exec_clang()
{
//    out->setText("Hiiiii\n");
    QString result = QString::fromLocal8Bit(proc->readAll());
    out->append(result);
}

// after the clang process is finished, this function is invoked.
void MainWindow::on_clang_over(int p,QProcess::ExitStatus es)
{
//    out->append("the process is over!!\n");
}
