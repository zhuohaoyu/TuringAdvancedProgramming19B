#include "mainwindow.h"
#include "itemsetdialog.h"
#include "ruledialog.h"
#include "resultdialog.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    filePath = "";
    ui->setupUi(this);

    ui->lineEdit_Filepath->setText(filePath);
    ui->pushButton_showItemsets->setEnabled(false);
    ui->pushButton_showResult->setEnabled(false);
    ui->pushButton_showRule->setEnabled(false);
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
    ui->pushButton_showItemsets->setEnabled(false);
    ui->pushButton_showResult->setEnabled(false);
    ui->pushButton_showRule->setEnabled(false);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_runButton_clicked()
{
    ui->pushButton_SelectFile->setEnabled(false);
    ui->spinBox_MFS->setEnabled(false);
    ui->spinBox_MIS->setEnabled(false);
    ui->doubleSpinBox_MCF->setEnabled(false);
    ui->pushButton_showItemsets->setEnabled(false);
    ui->pushButton_showResult->setEnabled(false);
    ui->pushButton_showRule->setEnabled(false);

    run_clang();
}


void MainWindow::run_clang()
{
    out = new QTextEdit();
    out->setReadOnly(true);
    ui->scrollArea->setWidget(out);
    ui->scrollArea->setWidgetResizable(true);
    proc = new QProcess(this);
    runtimeRes.clear();
    for(int i = 0; i < 5; ++i) {
        resList[i].clear();
    }
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(on_exec_clang_stdout()));
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(on_exec_clang_stderr()));
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_clang_over(int, QProcess::ExitStatus)));
    QStringList args;
    QString mis =QString::number(ui->spinBox_MIS->value()), mfs = QString::number(ui->spinBox_MFS->value()),
            mcf = QString::number(ui->doubleSpinBox_MCF->value());
    args << "-mllvm" << "-mfs=" + mfs << "-mllvm" << "-mis=" + mis <<"-mllvm" << "-min_conf=" + mcf;
    args << "-Xclang" << "-load" << "-Xclang"  << "./MyPass.so";
    args << "-g" << "-c";
    args << filePath;
    proc->setReadChannel(QProcess::StandardOutput);
    proc->start("clang", args);
}

void MainWindow::on_exec_clang_stdout()
{
    QString result = QString::fromLocal8Bit(proc->readAllStandardOutput());
    runtimeRes.append(result);
}

void MainWindow::on_exec_clang_stderr()
{
    QString result = QString::fromLocal8Bit(proc->readAllStandardError());
    out->append(result);
}


void MainWindow::on_clang_over(int p,QProcess::ExitStatus es)
{
    ui->pushButton_SelectFile->setEnabled(true);
    ui->spinBox_MFS->setEnabled(true);
    ui->spinBox_MIS->setEnabled(true);
    ui->doubleSpinBox_MCF->setEnabled(true);

    QStringList resLines = runtimeRes.split("\n");
    int curResID = -1;
    bool checkerSuccess = false;
    for(auto i:resLines) {
        if(i == "$FIS") {
            curResID = 0;
            continue;
        }
        else if(i == "$IIS") {
            curResID = 1;
            continue;
        }
        else if(i == "$PAR") {
            curResID = 2;
            continue;
        }
        else if(i == "$NAR") {
            curResID = 3;
            continue;
        }
        else if(i == "$RES") {
            curResID = 4;
            continue;
        }
        else if(i == "$$FIS" || i == "$$IIS" || i == "$$PAR" || i == "$$NAR" || i == "$$RES") {
            curResID = -1;
            continue;
        }
        else if(i == "$$SUCCESS") {
            checkerSuccess = true;
        }
        if(curResID >= 0) resList[curResID].append(i);
//        if(curResID < 0) std::cout << i.toStdString() << std::endl;
//        else resList[curResID].append(i);
    }
    if(checkerSuccess) {
        ui->pushButton_showItemsets->setEnabled(true);
        ui->pushButton_showResult->setEnabled(true);
        ui->pushButton_showRule->setEnabled(true);
    }
    delete proc;
    proc = nullptr;
}

void MainWindow::on_pushButton_showResult_clicked()
{
    ResultDialog *dlg = new ResultDialog(nullptr, resList[4]);
    dlg->setWindowTitle("Show Results");
    dlg->show();
}

void MainWindow::on_pushButton_showRule_clicked()
{
    RuleDialog *dlg = new RuleDialog(nullptr, resList[2], resList[3]);
    dlg->setWindowTitle("Show Rules");
    dlg->show();
}

void MainWindow::on_pushButton_showItemsets_clicked()
{
    ItemsetDialog *dlg = new ItemsetDialog(nullptr, resList[0], resList[1]);
    dlg->setWindowTitle("Show Itemsets");
    dlg->show();
}
