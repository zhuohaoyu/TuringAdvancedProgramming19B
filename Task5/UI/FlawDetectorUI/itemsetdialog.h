#ifndef ITEMSETDIALOG_H
#define ITEMSETDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QTreeView>
#include <QStandardItemModel>
#include <QList>
#include <QStandardItem>
#include <QAbstractItemView>

namespace Ui {
class ItemsetDialog;
}

class ItemsetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemsetDialog(QWidget *parent = nullptr);
    explicit ItemsetDialog(QWidget *parent, QStringList l1, QStringList l2);
    ~ItemsetDialog();

private:
    Ui::ItemsetDialog *ui;
    QStringList FIS, IIS;
    QStandardItemModel *modelFIS, *modelIIS;
//    QStringListModel *FISlistModel, *IISlistModel;

};

#endif // ITEMSETDIALOG_H
