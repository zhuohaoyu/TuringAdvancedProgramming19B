#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QTextEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include <QList>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QDebug>
#include <QItemSelectionModel>
#include <QVariant>
#include <QAbstractItemModel>
#include <QFile>
#include <QByteArray>
#include <QTextCursor>
#include <QTextBlock>
#include <QChar>
#include <QTextFormat>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    explicit ResultDialog(QWidget *parent, QStringList ls, QString filePath);
    ~ResultDialog();

private slots:
    void on_current_column_changed(const QModelIndex& current, const QModelIndex& previous);

private:
    int previousSelectedLine;
    void gotoline(int line);
    Ui::ResultDialog *ui;
    QStringList result;
    QString filep;
    QStandardItemModel *modelRES;
    QTextEdit *code;
};

#endif // RESULTDIALOG_H
