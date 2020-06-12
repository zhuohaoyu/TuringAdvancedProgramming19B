#include "itemsetdialog.h"
#include "ui_itemsetdialog.h"
#include <iostream>
ItemsetDialog::ItemsetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemsetDialog)
{
    ui->setupUi(this);
}

ItemsetDialog::ItemsetDialog(QWidget *parent, QStringList l1, QStringList l2) :
    QDialog(parent), FIS(l1), IIS(l2),
    ui(new Ui::ItemsetDialog)
{
    ui->setupUi(this);
    modelFIS = new QStandardItemModel(ui->treeView);
    modelIIS = new QStandardItemModel(ui->treeView_2);
    modelFIS->setHorizontalHeaderLabels(QStringList()<<"ID"<< "Contents");
    modelIIS->setHorizontalHeaderLabels(QStringList()<<"ID"<< "Contents");
    int indx = 0;
    for(auto i: FIS) {
        ++indx;
        QStringList split1 = i.split('|');
        QString supportVal = split1[1];
        QStringList split2 = split1[0].split('#');
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem("Item Set " + QString::number(indx));
        QStandardItem* item2 = new QStandardItem("Support = " + supportVal);
        items1.append(item1), items1.append(item2);
        modelFIS->appendRow(items1);
        int itemIndx = 0;
        for(auto j: split2) {
            ++itemIndx;
            QList<QStandardItem*> items2;
            QStandardItem* jitem1 = new QStandardItem("Item " + QString::number(itemIndx));
            QStandardItem* jitem2 = new QStandardItem(j);
            items2.append(jitem1), items2.append(jitem2);
            item1->appendRow(items2);
        }
    }
    indx = 0;
    for(auto i: IIS) {
        ++indx;
        QStringList split1 = i.split('|');
        QString supportVal = split1[1];
        QStringList split2 = split1[0].split('#');
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem("Item Set " + QString::number(indx));
        QStandardItem* item2 = new QStandardItem("Support = " + supportVal);
        items1.append(item1), items1.append(item2);
        modelIIS->appendRow(items1);
        int itemIndx = 0;
        for(auto j: split2) {
            ++itemIndx;
            QList<QStandardItem*> items2;
            QStandardItem* jitem1 = new QStandardItem("Item " + QString::number(itemIndx));
            QStandardItem* jitem2 = new QStandardItem(j);
            items2.append(jitem1), items2.append(jitem2);
            item1->appendRow(items2);
        }
    }
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setModel(modelFIS);
    ui->treeView->expandAll();

    ui->treeView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView_2->setModel(modelIIS);
    ui->treeView_2->expandAll();

}

ItemsetDialog::~ItemsetDialog()
{
    delete modelFIS;
    delete modelIIS;
    delete ui;
}
