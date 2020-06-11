#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QTextEdit>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    explicit ResultDialog(QWidget *parent, QStringList ls);
    ~ResultDialog();

private:
    Ui::ResultDialog *ui;
    QStringList result;
    QTextEdit *out;
};

#endif // RESULTDIALOG_H
