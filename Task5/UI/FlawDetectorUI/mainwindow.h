#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QProcess>
#include <QTextEdit>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void run_clang();
    void initSignalSlots();
private slots:
    void on_exec_clang_stdout();
    void on_exec_clang_stderr();
    void on_clang_over(int,QProcess::ExitStatus);

    void on_pushButton_SelectFile_clicked();

    void on_exitButton_clicked();

    void on_runButton_clicked();

    void on_pushButton_showResult_clicked();

    void on_pushButton_showRule_clicked();

    void on_pushButton_showItemsets_clicked();

private:
    QStringList resList[5];
    QTextEdit *out;
    QProcess *proc;
    QString filePath, runtimeRes;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
